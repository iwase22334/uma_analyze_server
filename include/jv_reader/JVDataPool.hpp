#ifndef JV_DATA_POOL_HPP
#define JV_DATA_POOL_HPP

#include <jv_reader/JVRecordReader.hpp>
#include <jv_reader/JVData_Structure.h>

#include <list>
#include <tuple>
#include <utility>

namespace jvdata
{
    namespace filter
    {
        typedef JVRecordFilter <JV_RA_RACE,              'R', 'A', '7'> ra_race;
        typedef JVRecordFilter <JV_SE_RACE_UMA,          'S', 'E', '7'> se_race_uma;
        typedef JVRecordFilter <JV_HR_PAY,               'H', 'R', '2'> hr_pay;
        typedef JVRecordFilter <JV_H1_HYOSU_ZENKAKE,     'H', '1', '5'> h1_hyosu_zenkake;
        typedef JVRecordFilter <JV_H6_HYOSU_SANRENTAN,   'H', '6', '5'> h6_hyosu_sanrentan;
        typedef JVRecordFilter <JV_O1_ODDS_TANFUKUWAKU,  'O', '1', '5'> o1_odds_tanfukuwaku;
        typedef JVRecordFilter <JV_O2_ODDS_UMAREN,       'O', '2', '5'> o2_odds_umaren;
        typedef JVRecordFilter <JV_O3_ODDS_WIDE,         'O', '3', '5'> o3_odds_wide;
        typedef JVRecordFilter <JV_O4_ODDS_UMATAN,       'O', '4', '5'> o4_odds_umatan;
        typedef JVRecordFilter <JV_O5_ODDS_SANREN,       'O', '5', '5'> o5_odds_sanren;
        typedef JVRecordFilter <JV_O6_ODDS_SANRENTAN,    'O', '6', '5'> o6_odds_sanrentan;
        typedef JVRecordFilter <JV_WF_INFO,              'W', 'F', '7'> wf_info;
        typedef JVRecordFilter <JV_JG_JOGAIBA,           'J', 'G', '1'> jg_jogaiba;
        typedef JVRecordFilter <JV_DM_INFO,              'D', 'M', '3'> dm_info;
        typedef JVRecordFilter <JV_TM_INFO,              'T', 'M', '3'> tm_info;
    };
    
    namespace filter_array
    {
        using namespace jvdata::filter;

        /**
         * @brief filter array for data sort at "RACE"
         * 
         */
        typedef JVFilterArray<  ra_race,
                                se_race_uma,
                                hr_pay,
                                h1_hyosu_zenkake,
                                h6_hyosu_sanrentan,
                                o1_odds_tanfukuwaku,
                                o2_odds_umaren,
                                o3_odds_wide,
                                o4_odds_umatan,
                                o5_odds_sanren,
                                o6_odds_sanrentan,
                                wf_info,
                                jg_jogaiba > race;
        
         /**
         * @brief filter array for data sort at "MING"
         * 
         */
        typedef JVFilterArray<  dm_info,
                                tm_info > ming;
    }

    template<class T> 
    class JVDataPool
    {
    public:
        using data_type = T;
    public:

        enum struct FilterResult
        {
            E_Caught, // Any one caught string.
            E_Pass, // no one caught string
            E_Complete // Any one caught string and filter_array is filled by data.
        };

    private:
        T filter_array_;
        std::list< T > data_list_;

    public:
        /**
         * @brief filter stiring by using filter array
         * 
         * @param str input record string
         * @return FilterResult 
         */
        FilterResult operator()(const std::string& str);

        /**
         * @brief return data_list
         * 
         * @return const std::list< T >& get_data 
         */
        const std::list< T >& get_data_list() const;

        /**
         * @brief return buffed data size
         * 
         * @return std::size_t get_data_size 
         */
        std::size_t get_data_size() const { return data_list_.size(); };

        /**
         * @brief charge_
         * 
         */
        void next();

        /**
         * @brief initialize
         * 
         */
        void reset();

    private:

        void charge(T && t) 
        {
            data_list_.push_back(std::forward<decltype(t)>(t));
        }

    };
    
    template<class T>
    typename JVDataPool<T>::FilterResult JVDataPool<T>::operator()(const std::string& str)
    {
        if (filter_array_(str)) return JVDataPool<T>::FilterResult::E_Caught; 
        return JVDataPool<T>::FilterResult::E_Pass;
    };

    template<class T>
    const std::list<T>& JVDataPool<T>::get_data_list() const
    {
        return this->data_list_;
    };

    template<class T>
    void JVDataPool<T>::next()
    {
        this->charge(std::move(filter_array_));
    };

    template<class T>
    void JVDataPool<T>::reset()
    {
        this->filter_array_.reset();
        this->data_list_.clear();
    };

    typedef JVDataPool<filter_array::race> race_pool;
    typedef JVDataPool<filter_array::ming> ming_pool;

};




#endif