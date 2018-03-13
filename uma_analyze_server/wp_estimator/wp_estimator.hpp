#ifndef WP_ESTIMATOR_HPP
#define WP_ESTIMATOR_HPP

#include <jv_reader/JVDataPool.hpp>
#include <jv_reader/JVDataHandling.hpp>

#include <list>
#include <vector>

namespace wp_estimator
{

    namespace ming_point {

        /**
         * @brief Compressed Race information
         * 
         */
        struct RaceInfo {
            private:
                constexpr std::size_t id_size_ = jvdata::RACE_ID_LENGTH;

            public:
                std::array<char, id_size_> id;
                std::vector<int> result;
                std::vector<int> ming_point;

            public:
                RaceInfo() =delete;
                RaceInfo(int size) : id{0}, result(size), ming_point(size) {};
                RaceInfo(int size, const id_type& iid) : result(size), ming_point(size) 
                { 
                    assert(sizeof(id_type) == id_size_);
                    char c[id_size_];
                    std::memcpy(c, iid, id_size_);
                    std::copy_n(c, id_size, id);
                };
                RaceInfo( RaceInfo && ri ) noexcept : 
                            id(std::move(ri.id))
                            result(std::move(ri.result)), 
                            ming_point(std::move(ri.ming_point))
                {};
        };

        std::list< RaceInfo > extruct_race_info(const jvdata::race_pool& rp, const jvdata::ming_pool& mp);
        RaceInfo extruct_race_info(const jvdata::filter_array::race& r, const jvdata::filter_array::ming& m);

        class WinProbabilityDistribution {
            using wp_estimator::ming_point::RaceInfo;

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
            static std::list< win_pair_t > extruct_win_pair(const std::list<RaceInfo>& ri);
            static std::list< lose_pair_t > extruct_lose_pair(const std::list<RaceInfo>& ri);

        };
    
        class Simulator {
        private:
            class RealRandGenerator {
            public:
                constexpr double def_min_value = 0.0;
                constexpr double def_max_value = 1.0;

            private:
                std::mt19937 mt_rand_;
                std::uniform_real_distribution<double> urd_;
            
            public:
                RealRandGenerator() : 
                    mt_rand_(std::random_device{}()),
                    urd_(def_min_value, def_max_value) {};                
                
                double operator()() { return urd_(mt_rand_); };

            };

            class IntRandGenerator {
            public:
                constexpr int def_min_value = 0;
                constexpr int def_max_value = 1;

            private:
                std::mt19937 mt_rand_;
                std::uniform_int_distribution<int> uid_;
            
            public:
                IntRandGenerator() : 
                    mt_rand_(std::random_device{}()),
                    uid_(def_min_value, def_max_value) {};                

                double operator()(int min, int max) 
                {
                    uid_.a = min; uid_.b = max;
                    return uid_(mt_rand_); 
                };

            };

        private:
            RealRandGenerator real_rand_;
            IntRandGenerator int_rand_;
        
        private:
            constexpr int replsce_try_default_ = 100;
            constexpr int restart_try_default_ = 200000;

        private:
            std::size_t horse_num_;
            unsigned int replace_try_num_;
            unsigned int restart_try_num_;
        
        public:
            using win_prob_list_t = std::vector<double>;

        public:
            Simulator() = delete;
            Simulator(std::size_t n) : 
                horse_num_(n), 
                replace_try_num_(replace_try_default_), 
                restart_try_num(restart_try_default_),
                real_rand_(),
                int_rand_() {};

            std::vector<win_prob_list_t> operator()( const WinProbabilityDistribution& wp_dist,
                                                     const RaceInfo& r_info);
            std::vector<win_prob_list_t> operator()( const WinProbabilityDistribution& wp_dist,
                                                     const std::vector<int>& ming_point);

            void set_replace_try_num(unsigned int a) { replace_try_num_ = a; };
            void set_restart_try_num(unsigned int a) { restart_try_num_ = a; };
        
        private:
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
                    std::assert(false);
                    return 0;
                };

                for (std::size_t i = 0; i < horse_num_; ++ i) {
                   int r = int_rand_(0, horse_num_ - i - 1);
                   initial_rank[find_empty(r)] = i + 1;
                }

                return initial_rank;
            };

            bool compete(double a) const { return rand_() < a ? true : false; };

            std::pair<int, int> random_pair() 
            { 
                std::pair<int, int> p;

                p.first = p.second = int_rand_(0, horse_num_ - 1);
                while(p.first == p.second) p.second = int_rand_();

                return p;
            };

            double pickup_from_table(const std::vector<int>& table, const std::pair<int, int> p) const 
            {
                std::assert(table.size() == horse_num_ * horse_num_);
                std::assert(p.first != p.second);

                return table[p.first + p.second * horse_num_];
            }

            void swap(std::vector<int>& rank, std::pair<int, int> target) const 
            { 
                std::assert(rank.size() == horse_num_);
                std::assert(p.first != p.second);

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