#ifndef JV_RECORD_READER_HPP
#define JV_RECORD_READER_HPP
#include <jv_reader/JVDataConstants.hpp>

#include <boost/optional.hpp>

#include <string>
#include <memory>
#include <tuple>
#include <list>
#include <array>
#include <cassert>



namespace jvdata {

    #include "impl/JVRecordReader.ipp"

    /**
     * @brief 
     * 
     * @param id1 
     * @param id2 
     * @return true id is same
     * @return false id is not same
     */
    static inline
    bool is_same_id(const _RACE_ID& id1, const _RACE_ID& id2){
        return (std::memcmp(&(id1), &(id2), sizeof(_RACE_ID)) == 0 ? true : false); 
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
        static_assert(std::is_same<decltype(a.id), _RACE_ID>::value, "invalid type");
        static_assert(std::is_same<decltype(b.id), _RACE_ID>::value, "invalid type");
        return (std::memcmp(&(a.id), &(b.id), sizeof(_RACE_ID)) == 0 ? true : false);
    };

    // class T1 or T2 has no member named id.
    template<class T1 = void, class T2 = void>
    auto is_same_id(...) -> bool
    {
        static_assert(std::false_type::value, "type has no member named id");
        return false;
    };

    /**
     * @brief Do Data in Filter array contain same id
     * @todo This function does not work
     *  because of the class that is not have the member named id.
     * 
     * @tparam Head 
     * @tparam Tail 
     * @param fa filter array
     * @return true all data in filter array has same id
     * @return false all data in filter array has not same id 
     */
    template<class Head, class... Tail>
    bool is_valid(const JVFilterArray<Head, Tail...>& fa) 
    {
        return _is_valid(fa, _seq_generator<1 + sizeof...(Tail)>{});
    };

    /**
     * @brief record value reader
     * 
     * @tparam T 
     * @tparam C1 
     * @tparam C2 
     */
    template<typename T, char C1, char C2, char C3>
    struct JVRecordFilter {
    public:
        using result_type = T;

    public:
        std::list< std::unique_ptr<T> > fallen_list;

    public:
        JVRecordFilter() : fallen_list() {};
		
		JVRecordFilter(const JVRecordFilter&) = delete;

        JVRecordFilter(JVRecordFilter&& rf) noexcept
            : fallen_list(std::move(rf.fallen_list)) 
        {
            rf.reset();
        };

        auto read(const std::string str) -> boost::optional<const id_type&>
		{

            if (str.c_str()[0] != C1 || str.c_str()[1] != C2 || str.c_str()[2] != C3) { 
                return boost::none;
            }
            
            assert(sizeof(T) == str.size());

            // catch inputed data
			std::unique_ptr<T> tp(new T());
            std::memcpy(tp.get(), str.c_str(), sizeof(T));
            fallen_list.push_back(std::move(tp));

			return fallen_list.back()->id;
        };

		const std::list< std::unique_ptr<T> >& get() const 
		{ 
			return fallen_list;  
		};

        void reset() 
		{ 
            fallen_list.clear();
        };
    };

    template<class... Filters>
    class JVFilterArray {
    private:
        static constexpr std::size_t filter_size_ = sizeof...(Filters);
        std::array<bool, filter_size_> caught_;
        std::tuple<Filters...> filter_tuple_;

    public:
        static constexpr std::size_t filter_size = filter_size_;
        using tuple_type = std::tuple<Filters...>;

    private:
        /**
         * @brief Metafunc for getting first found index of target class.
         * 
         * @tparam Index result
         * @tparam Target 
         * @tparam Head 
         * @tparam Tail 
         */
        template<int Index, class Target, class Head, class... Tail >
        struct type_finder
        {
            typedef typename type_finder<Index + 1, Target, Tail...>::type type;
            static constexpr int index = Index;
        };

        template<int Index, class Target, class... Types>
        struct type_finder<Index, Target, Target, Types...>
        {
            typedef type_finder<Index, Target, Target, Types...> type;
            static constexpr int index = Index;
        };

        /**
         * @brief Metafunc for fowarding string to next filter
         * 
         * @tparam Head 
         * @tparam Tail 
         */
        template<class Head, class... Tail>
        struct string_fowarder
        {
            auto operator()(std::array<bool, filter_size_>& res,
                            std::tuple<Filters...>& fs,
                            const std::string& str) noexcept(false) 
				-> boost::optional<const id_type&>
            {
				boost::optional<const id_type&> ret;

                if (!(ret = std::get<Head::val>(fs).read(str)))
                    ret = string_fowarder<Tail...>{}(res, fs, str);
					
                else 
                    res[Head::val] = true;

                return ret;
            };
        };

        template <>
        struct string_fowarder< val_helper<filter_size_ - 1> >
        {
            auto operator()(std::array<bool, filter_size_>& res,
                            std::tuple<Filters...>& fs,
                            const std::string& str) noexcept(false)
				-> boost::optional<const id_type&> 
            {
                boost::optional<const id_type&> ret = std::get<filter_size_ - 1>(fs).read(str);

                if (ret) {
                    res[filter_size_ - 1] = true;
                }

                return ret; 
            };
        };

        /**
         * @brief Input str to each filters in the tuple.
         * 
         * @tparam Ns 
         * @param str 
         * @param dummy 
         */
        template<int Head, int... Tail>
        auto filter_all_(const std::string& str, const _seq<Head, Tail...>& dummy) noexcept(false)
			-> boost::optional<const id_type&> 
        {
            // filter all
            return string_fowarder<val_helper<Head>, val_helper<Tail>...>{}
                            (
                                this->caught_,
                                this->filter_tuple_,
                                str
                            );
        }

        template<class Head, class... Tail>
        struct resetter
        {
            void operator()(std::tuple<Filters...>& fs)
            {
                std::get<Head::val>(fs).reset();
                resetter<Tail...>{}(fs);
            }
        };

        template<>
        struct resetter< val_helper<filter_size_ - 1> >
        {
            void operator()(std::tuple<Filters...>& fs)
            {
                std::get<filter_size_ - 1>(fs).reset();
            }
        };

        template<int Head, int... Tail>
        void reset_all_filter_(const _seq<Head, Tail...>& dum)
        {
            resetter< val_helper<Head>, val_helper<Tail>... >{}(filter_tuple_);
        };

    public:
        JVFilterArray() : caught_{{false}}, filter_tuple_() 
        {
            static_assert(filter_size_ > 0, "parameter size must greater than 0");
        };
		
		JVFilterArray(const JVFilterArray &) = delete;

        JVFilterArray(JVFilterArray&& fa) noexcept
            : caught_(std::move(fa.caught_)), filter_tuple_(std::move(fa.filter_tuple_))
        {
            fa.reset();
        };

        /**
         * @brief filter input string
         * 
         */
        auto operator()(const std::string& str) noexcept(false)
			-> boost::optional<const id_type&>
        {
            return filter_all_(str, _seq_generator<filter_size_>{});
        };

        /**
         * @brief clean all filter
         * 
         */
        void reset() 
        {
            for (auto& a : caught_) a = false;
            reset_all_filter_(_seq_generator<filter_size_>{});
        };

        bool is_caught_all() const
        {
            for (auto a : caught_) {
                if (!a) return false;
            } 
            return true;
        };

        bool is_caught(std::size_t i) const 
        {
            return caught_[i];
        } 

        const std::tuple<Filters...>& get_filters() const 
        {
            return filter_tuple_;
        };

        template<class RequestType>
        auto get() const -> decltype(std::get< type_finder<0, RequestType, Filters...>::type::index >(filter_tuple_))
        {
            return std::get< type_finder<0, RequestType, Filters...>::type::index >(filter_tuple_);
        };

    };

	namespace filter
	{
		using ra_race             = JVRecordFilter <JV_RA_RACE,             'R', 'A', '7'>;
		using se_race_uma         = JVRecordFilter <JV_SE_RACE_UMA,         'S', 'E', '7'>;
		using hr_pay              = JVRecordFilter <JV_HR_PAY,              'H', 'R', '2'>;
		using h1_hyosu_zenkake    = JVRecordFilter <JV_H1_HYOSU_ZENKAKE,    'H', '1', '5'>;
		using h6_hyosu_sanrentan  = JVRecordFilter <JV_H6_HYOSU_SANRENTAN,  'H', '6', '5'>;
		using o1_odds_tanfukuwaku = JVRecordFilter <JV_O1_ODDS_TANFUKUWAKU, 'O', '1', '5'>;
		using o2_odds_umaren      = JVRecordFilter <JV_O2_ODDS_UMAREN,      'O', '2', '5'>;
		using o3_odds_wide        = JVRecordFilter <JV_O3_ODDS_WIDE,        'O', '3', '5'>;
		using o4_odds_umatan      = JVRecordFilter <JV_O4_ODDS_UMATAN,      'O', '4', '5'>;
		using o5_odds_sanren      = JVRecordFilter <JV_O5_ODDS_SANREN,      'O', '5', '5'>;
		using o6_odds_sanrentan   = JVRecordFilter <JV_O6_ODDS_SANRENTAN,   'O', '6', '5'>;
		using wf_info             = JVRecordFilter <JV_WF_INFO,             'W', 'F', '7'>;
		using jg_jogaiba          = JVRecordFilter <JV_JG_JOGAIBA,          'J', 'G', '1'>;
		using dm_info             = JVRecordFilter <JV_DM_INFO,             'D', 'M', '3'>;
		using tm_info             = JVRecordFilter <JV_TM_INFO,             'T', 'M', '3'>;
	};

	namespace filter_array
	{

		/**
		* @brief filter array for data sort at "RACE"
		*
		*/
		typedef JVFilterArray<  
			filter::ra_race,
			filter::se_race_uma,
			filter::hr_pay,
			filter::h1_hyosu_zenkake,
			filter::h6_hyosu_sanrentan,
			filter::o1_odds_tanfukuwaku,
			filter::o2_odds_umaren,
			filter::o3_odds_wide,
			filter::o4_odds_umatan,
			filter::o5_odds_sanren,
			filter::o6_odds_sanrentan,
			filter::wf_info,
			filter::jg_jogaiba 
		> race;

		/**
		* @brief filter array for data sort at "MING"
		*
		*/
		typedef JVFilterArray<  
			filter::dm_info,
			filter::tm_info 
		> ming;
	}
};
#endif