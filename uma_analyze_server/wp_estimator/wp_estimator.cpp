#include "wp_estimator.hpp"
#include <cassert>

namespace {
    using jvdata::id_type;

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

        return std::move(ri);
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

        return std::move(res);
    };



};};