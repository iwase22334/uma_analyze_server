/**
 * @brief 
 * 
 * @file wp_estimator.cpp
 * @author hajime iwase
 * @date 2018-03-14
 * @todo fix validate function for JVFilterArray
 */
#include <cassert>
#include <statistics/statistics.hpp>
#include <linear/linear.hpp>
#include "wp_estimator.hpp"

namespace {
    using jvdata::id_type;

    inline 
    la::Vec2d to_vec2d(std::pair<int, int> p) {
        return la::Vec2d((double)p.first, (double)p.second);
    }

    namespace mp = wp_estimator::ming_point;
    mp::RaceInfo mearge(const jvdata::filterarray::race& fa_race,
                        const jvdata::filterarray::ming& fa_ming) 
    {
        const int syusso_num = jvdata::get_syusso_num(fa_race.get<jvdata::filter::ra_race>());
        mp::RaceInfo ri(syusso_num, 
                        std::get<0>(fa_race.get_filters()).get().front()->id);

        for (int i = 0; i < syusso_num; ++ i) {

            const int jyuni = jvdata::get_kakutei_jyuni(i, fa_race.get<jvdata::filter::se_race_uma>());
            const int ming_point = jvdata::get_ming_point(i, fa_ming.get<jvdata::filter::tm_info>());

            if (jyuni <= 0 || jyuni > syusso_num) 
                throw std::runtime_error( std::string("invalid kakutei jyuni : ") + std::to_string(jyuni) );
            if (ming_point < 0 || ming_point == 9999) 
                throw std::runtime_error( std::string("invalid ming point : ") + std::to_string(ming_point) );
            
            ri.result[i] = jyuni;
            ri.ming_point[i] = ming_point;
            
        }

        return ri;
    };

    template <class Comp>
    std::list< std::pair<int, int> > get_all_ming_point_pair(const mp::RaceInfo& ri, Comp comp_func) 
    {
        std::list< std::pair<int, int> > pair_list;
        
        for (std::size_t i = 0; i < ri.result.size() - 1; ++ i) {
            for (std::size_t j = i + 1; j < ri.result.size(); ++ j) {
                if (comp_func(ri.result[i], ri.result[j])) {
                    pair_list.push_back(std::pair<int, int>(ri.ming_point[i], ri.ming_point[j]));
                }

                else {
                    pair_list.push_back(std::pair<int, int>(ri.ming_point[j], ri.ming_point[i]));
                }
            }
        } 

        return pair_list;
    };

};

namespace wp_estimator { namespace ming_point{

    auto extruct_race_info(const jvdata::filterarray::race& r, 
                               const jvdata::filterarray::ming& m) -> RaceInfo
    {
        return mearge(r, m);
    };

    auto extruct_race_info(const jvdata::datapool::race& dp_race, const jvdata::datapool::ming& dp_ming) -> std::list< RaceInfo >
    {
        std::list< mp::RaceInfo > res;

        // find same id
        for (const auto& umap_elem: dp_race.data()) {
			const auto& it = dp_ming.data().find(umap_elem.first);
			
			if (it != dp_ming.data().end()) {
				mp::RaceInfo ri = mearge(umap_elem.second, it->second);
				res.push_back(std::move(ri));
			}

        }

        return res;
    };

    using wpd = WinProbabilityDistribution;
    wpd::WinProbabilityDistribution(const std::list<RaceInfo>& ri_list) :
        wp_list_(extruct_win_pair(ri_list)),
        lp_list_(extruct_lose_pair(ri_list))
    {};

    double wpd::operator()(const std::pair<int, int> point) const 
    {
        assert(wp_list_.size() == lp_list_.size());

        using kernel = statistics::EpanechnikovKernel<la::Vec2d>;
        using estimator = statistics::KDensityEstimator<la::Vec2d, kernel>;

        auto get_csum = [&point]
                        (const std::list< std::pair<int, int> >& pl, double bw) -> double {
            double sum = 0;
            for (const auto& a : pl) {
                sum += estimator{}(bw, to_vec2d(point), to_vec2d(a));
            }
            return sum;
        };

        double w_estimated = get_csum(wp_list_, band_width_) / (wp_list_.size() * band_width_);
        double l_estimated = get_csum(lp_list_, band_width_) / (lp_list_.size() * band_width_);

        return w_estimated / l_estimated; 
    };

    auto wpd::extruct_win_pair(const std::list<RaceInfo>& ri_list) -> std::list< wpd::win_pair_t >
    {
        std::list< wpd::win_pair_t > wp_list;

        auto is_higher = [](int a, int b) -> bool {
            return a > b;
        };

        for(const auto& ri : ri_list) {
            std::list< win_pair_t > wpl = get_all_ming_point_pair<decltype(is_higher)>(ri, is_higher);
            wp_list.insert(wp_list.end(), wpl.begin(), wpl.end());  
        }

        return wp_list;
    };

    auto wpd::extruct_lose_pair(const std::list<RaceInfo>& ri_list) -> std::list< wpd::win_pair_t >
    {
        std::list< wpd::lose_pair_t > lp_list;

        auto is_lower = [](int a, int b) -> bool {
            return a < b;
        };

        for(const auto& ri : ri_list) {
            std::list< lose_pair_t > lpl = get_all_ming_point_pair<decltype(is_lower)>(ri, is_lower);
            lp_list.insert(lp_list.end(), lpl.begin(), lpl.end());  
        }

        return lp_list;
    };

};};

namespace wp_estimator { namespace ming_point{

	auto Simulator::operator()(const WinProbabilityDistribution& wp_dist,
		                       const RaceInfo& r_info)->std::vector<Simulator::win_prob_list_t>
    {
        return (*this)(wp_dist, r_info.ming_point);
    };

    auto Simulator::operator()(const WinProbabilityDistribution& wp_dist, 
                               const std::vector<int>& ming_point) -> std::vector<win_prob_list_t>
    {
        assert(ming_point.size() == horse_num_);
    
        std::vector<double> compete_table(horse_num_ * horse_num_);

        // initialize compete table
        for (std::size_t i = 0; i < horse_num_; ++ i) {
            for (std::size_t j = 0; j < horse_num_; ++ j) {
                compete_table[i * ming_point.size() + j] = wp_dist(std::pair<int, int>{ming_point[i], ming_point[j]});
            } 
        }

        // simulate race 
        std::vector< std::vector<int> > rank_total(horse_num_, std::vector<int>(horse_num_, 0));
        for (unsigned int race = 0; race < restart_try_num_; ++ race) {
            std::vector<int> rank = random_start();

            for (unsigned int replace = 0; replace < replace_try_num_; ++ replace) {
                std::pair<int, int> p = random_pair();
                double swap_prob = pickup_from_table(compete_table, p); 

                if (compete(swap_prob)) {
                    swap(rank, p);
                }
            }

            add_result(rank_total, rank);
        } 

        // aggregate result
        std::vector<win_prob_list_t> win_prob_list(horse_num_, win_prob_list_t(horse_num_, 0.0));
        for (std::size_t i = 0; i < horse_num_; ++ i) {
            for (std::size_t j = 0; j < horse_num_; ++ j) {
                win_prob_list[i][j] = (double)rank_total[i][j] / restart_try_num_;  
            } 
        }

        return win_prob_list;
    };

};};