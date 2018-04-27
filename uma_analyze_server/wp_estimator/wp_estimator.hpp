/**
 * @brief 
 * 
 * @file wp_estimator.hpp
 * @author hajime iwase 
 * @date 2018-03-14
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

namespace wp_estimator
{

    namespace ming_point {

        /**
         * @brief Compressed Race information
         * 
         */
        struct RaceInfo {
            private:
                static constexpr std::size_t id_size_ = jvdata::RACE_ID_LENGTH;

            public:
                std::array<char, id_size_> id;
                std::vector<int> result;
                std::vector<int> ming_point;

            public:
                RaceInfo() =delete;
                RaceInfo(int size) : id{{0}}, result(size), ming_point(size) {};
                RaceInfo(int size, const jvdata::id_type& iid) : result(size), ming_point(size) 
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
        auto extruct_race_info(const jvdata::filterarray::race& r, const jvdata::filterarray::ming& m) -> RaceInfo;
        auto extruct_race_info(const jvdata::datapool::race& dp_race, const jvdata::datapool::ming& dp_ming) -> std::list< RaceInfo >;

        class WinProbabilityDistribution {
            using RaceInfo = wp_estimator::ming_point::RaceInfo;

        public:
            using win_pair_t = std::pair<int, int>;
            using lose_pair_t = std::pair<int, int>;

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
            double operator()(const std::pair<int, int> p) const;

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
            static constexpr int replace_try_default_ = 100;
            static constexpr int restart_try_default_ = 200000;

        private:
            std::size_t horse_num_;
            unsigned int replace_try_num_;
            unsigned int restart_try_num_;
        
        public:
            using win_prob_list_t = std::vector<double>;

        public:
            Simulator() = delete;
            Simulator(std::size_t n) :
                mt_rand_(std::random_device{}()), 
                horse_num_(n), 
                replace_try_num_(replace_try_default_), 
                restart_try_num_(restart_try_default_){};

            auto operator()( const WinProbabilityDistribution& wp_dist,
                             const RaceInfo& r_info) -> std::vector<win_prob_list_t>;

            auto operator()( const WinProbabilityDistribution& wp_dist,
                             const std::vector<int>& ming_point) ->std::vector<win_prob_list_t>;

            void set_replace_try_num(unsigned int a) { replace_try_num_ = a; };
            void set_restart_try_num(unsigned int a) { restart_try_num_ = a; };
        
        private:
            int uniform_int_rand(int min, int max) const
            {
                return std::uniform_int_distribution<int>{min, max}(mt_rand_);
            }

            double uniform_double_rand(double min, double max) const
            {
                return std::uniform_real_distribution<double>{min, max}(mt_rand_);   
            }

            std::vector<int> random_start() const
            {
                std::vector<int> initial_rank(horse_num_, 0);
                
                auto find_empty = [&initial_rank](int rem) -> int { 
                    for (std::size_t i = 0; i < initial_rank.size(); ++i) {
                        if(initial_rank[i] == 0) {
                            if(rem == 0) return i;
                            -- rem;
                        }
                    }
                    assert(false);
                    return 0;
                };

                for (std::size_t i = 0; i < horse_num_; ++ i) {
                   int r = uniform_int_rand(0, horse_num_ - i - 1);
                   initial_rank[find_empty(r)] = i + 1;
                }

                return initial_rank;
            };

            bool compete(double a) const { return uniform_double_rand(0.0, 1.0) < a ? true : false; };

            std::pair<int, int> random_pair() 
            { 
                std::pair<int, int> p;

                p.first = p.second = uniform_int_rand(0, horse_num_ - 1);
                while(p.first == p.second) p.second = uniform_int_rand(0, horse_num_ - 1);

                return p;
            };

            double pickup_from_table(const std::vector<double>& table, const std::pair<int, int> p) const 
            {
                assert(table.size() == horse_num_ * horse_num_);
                assert(p.first != p.second);

                return table[p.first + p.second * horse_num_];
            }

            void swap(std::vector<int>& rank, std::pair<int, int> target) const 
            { 
                assert(rank.size() == horse_num_);
                assert(target.first != target.second);

                int tmp = rank[target.first];
                rank[target.first] = rank[target.second];
                rank[target.second] = tmp;
            };

            void add_result(std::vector< std::vector<int> >& rank_total, const std::vector<int>& rank) const 
            {
                for (std::size_t i = 0; i < horse_num_; ++ i) {
                    ++ (rank_total[rank[i]])[i];
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