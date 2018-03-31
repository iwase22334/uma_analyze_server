/**
 * @brief 
 * 
 * @file JVDataHandling.hpp
 * @author hajime iwase
 * @date 2018-03-14
 */
#ifndef JV_DATA_HANDLING_HPP
#define JV_DATA_HANDLING_HPP

#include <iostream>
#include <cassert>

#include "JVData_Structure.h"
#include "JVRecordReader.hpp"
#include "JVDataPool.hpp"

namespace {

    template <class T>
    bool operator==(const T& a, const T& b) {
        return std::memcmp(&a, &b, sizeof(T)) == 0 ? true : false;
    }

};

namespace jvdata {
    static constexpr int RACE_ID_LENGTH = 16;
    using id_type = _RACE_ID;

    /**
     * @brief copy function for JVData_Structures
     * 
     * @tparam T 
     * @param dst 
     * @param src 
     */
    template<class T> 
    void copy(T& dst, const T& src) 
    {
        std::memcpy(&dst, &src, sizeof(T));
    };

    /**
     * @brief make id string
     * 
     * @param id 
     * @return std::string 
     */
    static inline
    std::string to_string(const id_type& id)
    {
        char c[RACE_ID_LENGTH];
        std::memcpy(c, &id, RACE_ID_LENGTH);
        return std::string(c, RACE_ID_LENGTH);
    }

    template<int N>
    std::string to_string(const char (&c)[N]) 
    {
        return std::string(c, N);
    }

    template<int N>
    int to_integer(const char (&c)[N]) 
    {
        return std::stoi(to_string(c));
    }

    /**
     * @brief 
     * 
     * @param id 
     * @return true 
     * @return false 
     */
    static inline
    bool is_valid(const id_type& id) 
    {

        const std::string s_Year(id.Year, 4);
        const std::string s_MonthDay(id.MonthDay, 4);
        //const std::string s_JyoCD(id.JyoCD, 2);
        const std::string s_Kaiji(id.Kaiji, 2);
        const std::string s_Nichiji(id.Nichiji, 2);
        //const std::string s_RaceNum(id.RaceNum, 2);

        const int i_Year = std::stoi(s_Year);
        const int i_MonthDay = std::stoi(s_MonthDay);
        const int i_Month = i_MonthDay / 100;
        const int i_Day = i_MonthDay - i_Month;
        //const int i_JyoCD = std::stoi(s_JyoCD);
        const int i_Kaiji = std::stoi(s_Kaiji);
        const int i_Nichiji = std::stoi(s_Nichiji);
        //const int i_RaceNum = std::stoi(s_RaceNum);
        
        if(i_Year < 1990 || i_Year > 2020)  return false;
        if(i_Month == 0 || i_Month > 12)    return false;
        if(i_Day == 0 || i_Month > 31)      return false;
        
        if(i_Kaiji == 0)                    return false;
        if(i_Nichiji == 0)                  return false;

        return true;
    };

    /**
     * @brief get syusso tosu from ra_race
     * 
     * @param ra_race 
     * @return int 
     */
    static inline
    int get_syusso_num(const JV_RA_RACE& ra_race)
    {
        return std::stoi( std::string(ra_race.SyussoTosu, 2) );
    }

    static inline
    int get_syusso_num(const filter::ra_race& f_ra_race)
    {
        return get_syusso_num(*(f_ra_race.result_ptr));
    }

    static inline
    int get_kakutei_jyuni(int uma_ban_target, const jvdata::filter::se_race_uma& se_race_uma) 
    {  
        assert(uma_ban_target >= 0);

        if(to_integer(se_race_uma.result_ptr->Umaban) == uma_ban_target){
            return to_integer(se_race_uma.result_ptr->KakuteiJyuni);
        }

        for(const auto& a : se_race_uma.fallen_list){
            if(to_integer(a->Umaban) == uma_ban_target) {
                return to_integer(a->KakuteiJyuni);
            }
        }

        std::cerr << __FILE__ << __LINE__ << "uma_ban not exists" << std::endl;
        return 0;
    }

    static inline
    int get_ming_point(int uma_ban_target, const jvdata::filter::tm_info& tm_info)
    {
        assert(uma_ban_target >= 0);

        for (const auto& a : tm_info.result_ptr->TMInfo) {
            if (to_integer(a.Umaban) == uma_ban_target) {
                return to_integer(a.TMScore);
            }
        } 

        std::cerr << __FILE__ << __LINE__ << "uma_ban not exists" << std::endl;
        return 0;
    }

 };
#endif