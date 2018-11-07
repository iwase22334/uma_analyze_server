/**
 * @brief 
 * 
 * @file JVDataHandling.hpp
 * @author hajime iwase
 * @date 2018-03-14
 */
#ifndef JV_DATA_HANDLING_HPP
#define JV_DATA_HANDLING_HPP

#include <jv_reader/JVDataConstants.hpp>
#include <algorithm>
#include <vector>
#include <list>
#include <iostream>
#include <cassert>
#include <string>

namespace jvdata {

	namespace {

		inline bool operator==(const id_type& a, const id_type& b) {
			return std::memcmp(&a, &b, sizeof(id_type)) == 0 ? true : false;
		}

		/**
		* @brief
		*
		* @param id1
		* @param id2
		* @return true id is same
		* @return false id is not same
		*/
		inline bool is_same_id(const id_type& id1, const id_type& id2) {
			return (std::memcmp(&(id1), &(id2), sizeof(id_type)) == 0 ? true : false);
		}

		/**
		* @brief check id
		*
		* @tparam T1
		* @tparam T2
		* @param a
		* @param b
		* @return true
		* @return false
		*/
		template<class T1, class T2>
		auto is_same_id(const T1& a, const T2& b) -> decltype(a.id, b.id, bool())
		{
			static_assert(std::is_same<decltype(a.id), id_type>::value, "invalid type");
			static_assert(std::is_same<decltype(b.id), id_type>::value, "invalid type");
			return (std::memcmp(&(a.id), &(b.id), sizeof(id_type)) == 0 ? true : false);
		}

		// class T1 or T2 has no member named id.
		template<class T1 = void, class T2 = void>
		auto is_same_id(...) -> bool
		{
			static_assert(std::false_type::value, "type has no member named id");
			return false;
		}

        /**
         * @brief make id string
         * 
         * @param id 
         * @return std::string 
         */
        inline std::string to_string(const id_type& id)
        {
            char c[RACE_ID_LENGTH];
            std::memcpy(c, &id, RACE_ID_LENGTH);
            return std::string(c, RACE_ID_LENGTH);
        }

        inline id_type to_id_type(const std::string& id_str) {
            assert(id_str.size() == sizeof(id_type));
            id_type id;
            std::memcpy(&id, id_str.c_str(), sizeof(id_type));
            return id;
        }

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
        }
    
        template<int N>
        std::string to_string(const char (&c)[N]) 
        {
            return std::string(c, N);
        }

        template<int N>
        int to_integer(const char (&c)[N]) 
        {
            std::string str = to_string(c);
            if (str.size() == 0) 
                return 0;

            std::size_t boundary = str.find_first_not_of('0');
            std::size_t first_digit = str.size() - 1;

            // remove zero padding
            str.erase(0, boundary < first_digit ? boundary : first_digit);

            if (std::all_of(str.cbegin(), str.cend(), isdigit))
            {
                return std::stoi(str);
            }

            // not digit
            return 0;
        }
    
        /**
         * @brief 
         * 
         * @param id 
         * @return true 
         * @return false 
         */
        inline bool is_valid(const id_type& id) 
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
         * @param race 
         * @return int 
         */
        inline int get_syusso_num(const JV_RA_RACE& race)
        {
            //return std::stoi( std::string(race.SyussoTosu, 2) );
            return to_integer(race.SyussoTosu) ;
        }
    
        /**
         * @brief get ijyo code from se_race_uma
         * 
         * @param uma 
         * @return int 
         */
        inline int get_ijyo_code(const JV_SE_RACE_UMA& uma)
		{
            return to_integer(uma.IJyoCD);
		}
    
  	};
 };
#endif