/**
 * @brief 
 * 
 * @file wp_estimator.hpp
 * @author hajime iwase 
 */
#ifndef WP_ESTIMATOR_HPP
#define WP_ESTIMATOR_HPP

#include <jv_reader/JVDataHandling.hpp>
#include <jv_reader/JVDataPool.hpp>
#include <jv_reader/JVRecordReader.hpp>

#include <list>
#include <vector>
#include <array>
#include <cassert>
#include <random>

namespace wpestimator
{

    namespace mingpoint {

        /**
         * @brief Compressed Race information
         * 
         */
        struct RaceInfo {
            private:
                static constexpr std::size_t id_size_ = jvdata::RACE_ID_LENGTH;

            public:
                std::array<char, id_size_> id;
                std::vector<unsigned int> result;
                std::vector<unsigned int> ming_point;

            public:
                RaceInfo() =delete;
                RaceInfo(unsigned int size) : id{{0}}, result(size), ming_point(size) {};
                RaceInfo(unsigned int size, const jvdata::id_type& iid) : result(size), ming_point(size) 
                { 
                    assert(sizeof(jvdata::id_type) == id_size_);
                    char c[id_size_];
                    std::memcpy(c, &iid, id_size_);
                    std::copy(std::begin(c), std::end(c), id.begin());
                };
                RaceInfo( RaceInfo && ri ) noexcept : 
                            id(std::move(ri.id)),
                            result(std::move(ri.result)), 
                            ming_point(std::move(ri.ming_point))
                {};
        };

        /**
         * @brief Extruct race info for ming point estimation.
         * 
         * @param rp 
         * @param mp 
         * @return std::list< RaceInfo > 
         */
        auto extruct_race_info(const jvdata::datapool::race& dp_race, const jvdata::datapool::ming& dp_ming) -> std::list< RaceInfo >;
        void extruct_race_info(std::list< RaceInfo >& ri_list, const jvdata::datapool::race& dp_race, const jvdata::datapool::ming& dp_ming);

        class WinProbabilityDistribution {
            using RaceInfo = wpestimator::mingpoint::RaceInfo;

        public:
            using win_pair_t = std::pair<unsigned int, unsigned int>;
            using lose_pair_t = std::pair<unsigned int, unsigned int>;

        private:
            std::list<win_pair_t> wp_list_;
            std::list<lose_pair_t> lp_list_;

        public:
            WinProbabilityDistribution() = delete;
            WinProbabilityDistribution(const std::list<RaceInfo>& ri_list);
        
            /**
             * @brief return win probability on condition that ming_point pair
             * 
             */
            double operator()(const std::pair<unsigned int, unsigned int>& p) const;

        private:
            static constexpr double band_width_ = 10.0;

        private:
            static auto extruct_win_pair(const std::list<RaceInfo>& ri) -> std::list< win_pair_t >;
            static auto extruct_lose_pair(const std::list<RaceInfo>& ri) -> std::list< lose_pair_t >;

        };
    
        class Simulator {
        private:
            mutable std::mt19937 mt_rand_;
        
        private:
            static constexpr unsigned int replace_try_default_ = 70;
            static constexpr unsigned int restart_try_default_ = 10000;

        private:
            unsigned int horse_num_;
            unsigned int replace_try_num_;
            unsigned int restart_try_num_;
        
        public:
            using win_prob_list_t = std::vector<double>;

        public:
            Simulator() = delete;
            Simulator(unsigned int horse_num) :
                mt_rand_(std::random_device{}()), 
                horse_num_(horse_num), 
                replace_try_num_(replace_try_default_), 
                restart_try_num_(restart_try_default_){};

            auto operator()( const WinProbabilityDistribution& wp_dist,
                             const RaceInfo& r_info) -> std::vector<win_prob_list_t>;

            auto operator()( const WinProbabilityDistribution& wp_dist,
                             const std::vector<unsigned int>& ming_point) ->std::vector<win_prob_list_t>;

            void set_replace_try_num(unsigned int replace_try_num) { replace_try_num_ = replace_try_num; };
            void set_restart_try_num(unsigned int restart_try_num) { restart_try_num_ = restart_try_num; };
        
        private:
            unsigned int uniform_int_rand(unsigned int min, unsigned int max) const
            {
                return std::uniform_int_distribution<unsigned int>{min, max}(mt_rand_);
            }

            double uniform_double_rand(double min, double max) const
            {
                return std::uniform_real_distribution<double>{min, max}(mt_rand_);   
            }
            
            std::vector<unsigned int> random_start() const
            {
                std::vector<unsigned int> initial_rank(horse_num_, 0);
                
                auto find_empty = [&initial_rank](unsigned int rem) -> unsigned int { 
                    for (unsigned int i = 0; i < initial_rank.size(); ++i) {
                        if(initial_rank[i] == 0) {
                            if(rem == 0) return i;
                            -- rem;
                        }
                    }
                    assert(false);
                    return 0;
                };

                for (unsigned int i = 0; i < horse_num_; ++ i) {
                   unsigned int r = uniform_int_rand(0, horse_num_ - i - 1);
                   initial_rank[find_empty(r)] = i;
                }

                return initial_rank;
            };

            bool compete(double a) const { return uniform_double_rand(0.0, 1.0) < a ? true : false; };

            std::pair<unsigned int, unsigned int> random_pair() 
            { 
                std::pair<unsigned int, unsigned int> p;

                p.first = uniform_int_rand(0, horse_num_ - 2);
                p.second = p.first + 1;

                return p;
            };

            void swap(std::vector<unsigned int>& order, std::pair<unsigned int, unsigned int> target) const 
            { 
                assert(order.size() == horse_num_);
                assert(target.first != target.second);

                unsigned int tmp = order[target.first];
                order[target.first] = order[target.second];
                order[target.second] = tmp;
            };

            void add_result(std::vector< std::vector<unsigned int> >& rank_total, const std::vector<unsigned int>& rank) const 
            {
                for (std::size_t i = 0; i < horse_num_; ++ i) {
                    unsigned int horse_index = rank[i];
                    unsigned int rank_index = i;
                    rank_total[horse_index][rank_index] += 1;
                }
            }
        };

    };


    /**
     * @brief 
     * 
     */
    namespace time
    {

    };
};


#endif