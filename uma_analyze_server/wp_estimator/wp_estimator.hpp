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
                            id(std::move(ri.id)),
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
            static std::list< win_pair_t > extruct_win_pair(const std::list<RaceInfo>& ri);
            static std::list< lose_pair_t > extruct_lose_pair(const std::list<RaceInfo>& ri);

        };

        class Simulator {
        public:
            using win_prob_list_t = std::list<double>;
            std::list<win_prob_list_t> operator()(const WinProbabilityDistribution& wp_dist, const RaceInfo r_info);
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