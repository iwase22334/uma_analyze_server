#include <statistics/statistics.hpp>
#include <linear/linear.hpp>
#include <cassert>
#include "wp_estimator.hpp"

namespace {
    using jvdata::id_type;

    static inline 
    la::Vec2d to_vec2d(std::pair<int, int> p) {
        return la::Vec2d((double)p.first, (double)p.second);
    }

    namespace mp = wp_estimator::ming_point;
    mp::RaceInfo mearge(const jvdata::filter_array::race& r,
                        const jvdata::filter_array::ming& m) 
    {
        const int syusso_num = jvdata::get_syusso_num(r.get<JV_RA_RACE>());
        mp::RaceInfo ri(syusso_num, 
                        std::get<0>(r.get_filters()).result_ptr->id);

        for (int i = 0; i < syusso_num; ++ i) {

            const int jyuni = jvdata::get_kakutei_jyuni(i, r.get<JV_SE_RACE_UMA>());
            const int ming_point = jvdata::get_ming_point(i, m.get<JV_TM_INFO>());

            if (jyuni <= 0 || jyuni > syusso_num) 
                throw std::runtime_error( std::string("invalid kakutei jyuni : ") + std::to_string(jyuni) );
            if (ming_point < 0 || ming_point == 9999) 
                throw std::runtime_error( std::string("invalid ming point : ") + std::to_string(ming_point) )
            
            ri.result[i] = jyuni;
            ri.ming_point[i] = ming_point;
            
        }

        return ri;
    };

    template <class Comp>
    std::list< std::pair<int, int> > get_all_ming_point_pair(const RaceInfo& ri) 
    {
        std::list< std::pair<int, int> > pair_list;

        for (int i = 0; i < ri.result.size() - 1; ++ i) {
            for (int j = i + 1; j < ri.result.size(); ++ j) {
                if (Comp(ri.result[i], ri.result[j])) {
                    pair_list.push_back(std::pair(ri.ming_point[i], ri.ming_point[j]));
                }
                else {
                    pair_list.push_back(std::pair(ri.ming_point[j], ri.ming_point[i]));
                }
            }
        } 

        return pair_list;
    };

};

namespace wp_estimator { namespace ming_point{

    RaceInfo extruct_race_info(const jvdata::filter_array::race& r, 
                               const jvdata::filter_array::ming& m) 
    {
        return std::move(mearge(r, m));
    };

    std::list< RaceInfo > extruct_race_info(const jvdata::race_pool& rp, const jvdata::ming_pool& mp) 
    {
        const std::list<jv_data::filter_array::race>& race_fa_list = rp.get_data_list();
        const std::list<jv_data::filter_array::ming>& ming_fa_list = mp.get_data_list();

        std::list< RaceInfo > res;

        // find same id
        for (const auto& race_fa: race_fa_list) {

            // pool must be validated in other function
            assert(is_valid(race_fa));

            const jv_data::filter_array::race::tuple_type& f_tuple = race_fa.get_filters();  

            id_type target_id;
            jvdata::copy(target_id, std::get<0>(f_tuple).result_ptr->id);

            // function for find_if
            auto find_func = [&target_id](const jv_data::filter_array::ming& fa) {

                assert(is_valid(fa));

                const jv_data::filter_array::ming::tuple_type& f_tuple = fa.get_filters();
                return std::get<0>(f_tuple).result_ptr->id == target_id;

            };

            // find same id
            const auto& result_it = std::find_if(ming_fa_list.begin(), ming_fa_list.end(), find_func);
            
            if (result_ptr != ming_fa_list.end()) {
                mp::RaceInfo ri = mearge(race_fa, *result_ptr); 
                res.push_back(ri);
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
        std::assert(wp_list_.size() == lp_list_.size());

        using kernel = statistics::EpanechnikovKernel<la::Vec2d>;
        using estimator = statistics::KDensityEstimatior<la::Vec2d, kernel>;

        auto get_csum = [&band_width_, &point]
                        (const std::list< std::pair<int, int> >& pl) -> double {
            double sum = 0;
            for (const auto& a : pl) {
                sum += estimator{}(band_width_, to_vec2d(point), to_vec2d(a));
            }
        }

        double w_estimated = get_csum(wp_list_) / (wp_list_.size() * band_width_);
        double l_estimated = get_csum(lp_list_) / (lp_list_.size() * band_width_);

        return w_estimated / l_estimated; 
    };

    std::list< win_pair_t > wpd::extruct_win_pair(const std::list<RaceInfo>& ri_list)
    {
        std::list< win_pair_t > wp_list;

        auto is_higher = [](int a, int b) -> bool {
            return a > b;
        };

        for(const auto& ri : ri_list) {
            std::list< win_pair_t > wpl = get_all_ming_point_pair<is_higher>(ri);
            wp_list.insert(wp_list.end(), wpl.begin(), wpl.end());  
        }

        return wp_list;
    };

    std::list< lose_pair_t > wpd::extruct_lose_pair(const std::list<RaceInfo>& ri)
    {
        std::list< lose_pair_t > lp_list;

        auto is_lower = [](int a, int b) -> bool {
            return a < b;
        };

        for(const auto& ri : ri_list) {
            std::list< lose_pair_t > lpl = get_all_ming_point_pair<is_lower>(ri);
            lp_list.insert(lp_list.end(), lpl.begin(), lpl.end());  
        }

        return lp_list;
    };

};};

namespace wp_estimator { namespace ming_point{

    std::vector<win_prob_list_t> Simulator::operator()(const WinProbabilityDistribution& wp_dist, 
                                                       const RaceInfo& r_info) 
    {
        return (*this)(wp_dist, r_info.ming_point);
    };

    std::vector<win_prob_list_t> Simulator::operator()(const WinProbabilityDistribution& wp_dist, 
                                                       const std::vector<int>& ming_point) 
    {
        std::assert(ming_point.size() == horse_num_);
    
        std::vector<double> compete_table(horse_num_ * horce_num);

        // initialize compete table
        for (std::size_t i = 0; i < horse_num_; ++ i) {
            for (std::size_t j = 0; j < horse_num_; ++ j) {
                compete_table[i * ming_point.size() + j] = wp_dist(std::pair{ming_point[i], ming_point[j]});
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
        std::vector<win_prob_list_t> win_prob_list(horse_num_, win_problist_t(horse_num_, 0.0));
        for (std::size_t i = 0; i < horse_num_; ++ i) {
            for (std::size_t j = 0; j < horse_num_; ++ j) {
                win_prob_list[i][j] = (double)rank_total[i][j] / restart_try_num_;  
            } 
        }

        return win_prob_list;
    };

};};