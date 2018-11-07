#ifndef JV_RECORD_READER_HPP
#define JV_RECORD_READER_HPP

#include <jv_reader/JVDataConstants.hpp>
#include <jv_reader/JVDataHandling.hpp>
#include <jv_reader/impl/seq_generator-meta.hpp>

#include <boost/optional.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <tuple>
#include <list>
#include <array>
#include <cassert>

namespace jvdata {

    /**
     * @brief record value reader
     * This class hooks the string that contains "C1C2C3" at the begining of it,
	 * and store it as type T.
	 * C1,C2,C3 is given as template parameter 
     * @tparam T 
     * @tparam C1 
     * @tparam C2 
	 * @tparam C3 
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
        struct string_fowarder< meta::val_helper<filter_size_ - 1> >
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
        auto filter_all_(const std::string& str, const meta::seq<Head, Tail...>& dummy) noexcept(false)
			-> boost::optional<const id_type&> 
        {
            // filter all
            return string_fowarder<meta::val_helper<Head>, meta::val_helper<Tail>...>{}
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
        struct resetter< meta::val_helper<filter_size_ - 1> >
        {
            void operator()(std::tuple<Filters...>& fs)
            {
                std::get<filter_size_ - 1>(fs).reset();
            }
        };

        template<int Head, int... Tail>
        void reset_all_filter_(const meta::seq<Head, Tail...>& dum)
        {
            resetter< meta::val_helper<Head>, meta::val_helper<Tail>... >{}(filter_tuple_);
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
            return filter_all_(str, meta::seq_generator<filter_size_>{});
        };

        /**
         * @brief clean all filter
         * 
         */
        void reset() 
        {
            for (auto& a : caught_) a = false;
            reset_all_filter_(meta::seq_generator<filter_size_>{});
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

	namespace label {
		namespace {
			constexpr char ra_race[]				= "RA7";
			constexpr char se_race_uma[]			= "SE7";
			constexpr char hr_pay[]					= "HR2";
			constexpr char h1_hyosu_zenkake[]		= "H15";
			constexpr char h6_hyosu_sanrentan[]		= "H65";
			constexpr char o1_odds_tanfukuwaku[]	= "O15";
			constexpr char o2_odds_umaren[]			= "O25";
			constexpr char o3_odds_wide[]			= "O35";
			constexpr char o4_odds_umatan[]			= "O45";
			constexpr char o5_odds_sanren[]			= "O55";
			constexpr char o6_odds_sanrentan[]		= "O65";
			constexpr char wf_info[]				= "WF7";
			constexpr char jg_jogaiba[]				= "JG1";
			constexpr char dm_info[]				= "DM3";
			constexpr char tm_info[]				= "TM3";
		}
	}

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
		//using wf_info             = JVRecordFilter <JV_WF_INFO,             'W', 'F', '7'>;
		using jg_jogaiba          = JVRecordFilter <JV_JG_JOGAIBA,          'J', 'G', '1'>;
		//using dm_info             = JVRecordFilter <JV_DM_INFO,             'D', 'M', '3'>;
		//using tm_info             = JVRecordFilter <JV_TM_INFO,             'T', 'M', '3'>;
		using dm_info             = JVRecordFilter <JV_DM_INFO,             'D', 'M', '7'>;
		using tm_info             = JVRecordFilter <JV_TM_INFO,             'T', 'M', '7'>;
	};

	namespace filterarray
	{

		/**
		* @brief filter array for data sort at "RACE"
		*
		*/
		/*using race = JVFilterArray<  
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
			//filter::wf_info,
			filter::jg_jogaiba 
		>;*/

        using race = JVFilterArray<  
			filter::ra_race,
			filter::se_race_uma,
			filter::hr_pay,
	    	filter::h1_hyosu_zenkake,
			//filter::h6_hyosu_sanrentan,
			filter::o1_odds_tanfukuwaku
			//filter::o2_odds_umaren,
			//filter::o3_odds_wide,
			//filter::o4_odds_umatan,
			//filter::o5_odds_sanren,
			//filter::o6_odds_sanrentan,
			//filter::wf_info,
			//filter::jg_jogaiba 
		>;
		/**
		* @brief filter array for data sort at "MING"
		*
		*/
		using ming = JVFilterArray<  
			filter::dm_info,
			filter::tm_info 
		>;
	}

	namespace {
	
		inline int get_syusso_num(const filter::ra_race& race)
		{
			return get_syusso_num(*(race.get().front()));
		}
	
		inline auto get_kakutei_jyuni(int uma_ban_target, const filter::se_race_uma& race_uma)
            -> boost::optional<int>
		{
			assert(uma_ban_target > 0);
	
			for (const auto& a : race_uma.fallen_list) {
				if (to_integer(a->Umaban) == uma_ban_target) {
					return to_integer(a->KakuteiJyuni);
				}
			}
	
			std::cout << __FILE__ << __LINE__ << "uma_ban not exists" << std::endl;
			return boost::none;
		}

		inline auto get_ijyo_code(int uma_ban_target, const filter::se_race_uma& race_uma)
            ->boost::optional<int>
		{
			assert(uma_ban_target > 0);
	
			for (const auto& a : race_uma.fallen_list) {
				if (to_integer(a->Umaban) == uma_ban_target) {
					return get_ijyo_code(*a);
				}
			}
	
			std::cout << __FILE__ << __LINE__ << "uma_ban not exists" << std::endl;
			return boost::none;
		}
	
		inline auto get_ming_point(int uma_ban_target, const filter::tm_info& tm_info)
            -> boost::optional<int>
		{
			assert(uma_ban_target > 0);
            assert(tm_info.get().size() == 1);

			for (const auto& a : tm_info.get().front()->TMInfo) {
				if (to_integer(a.Umaban) == uma_ban_target) {
					return to_integer(a.TMScore);
				}
			}

            // ! @todo remove
			std::cout << __FILE__ << __LINE__ << "uma_ban not exists" << std::endl;
			return boost::none;

		}

        inline auto get_odds_tansyou(int uma_ban_target, const filter::o1_odds_tanfukuwaku& odds_tanfukuwaku)
            -> boost::optional<double>
		{
			assert(uma_ban_target > 0);
            assert(odds_tanfukuwaku.get().size() == 1);

			for (const auto& a : odds_tanfukuwaku.get().front()->OddsTansyoInfo) {
                // "----": Invalid votes 
                // "****": Canceled before release 
                // "    ": Canceled after release
                // "0000": Not registered
                if (a.Umaban[0] == '-'
                    || a.Umaban[0] == '*'
                    || a.Umaban[0] == ' '
                    || to_integer(a.Umaban) == 0) {
                    
        			return boost::none;
                }

                if (to_integer(a.Umaban) == uma_ban_target) {
                    // Odds format : 9999 -> 999.9
                    return to_integer(a.Odds) * 0.1;
				}
			}

            //! @todo remove
			std::cout << __FILE__ << __LINE__ << "uma_ban not exists" << std::endl;
			return boost::none;
		}

        inline auto get_pay_tansyou(const filter::hr_pay& hrpay) 
            -> std::list< std::pair<int, int> > 
		{
            assert(hrpay.get().size() == 1);

            std::list< std::pair<int, int> > res;

            for (const auto& a: hrpay.get().front()->PayTansyo) {

                if (to_integer(a.Umaban) != 0) {
                    int umaban = to_integer(a.Umaban);
                    int payout = to_integer(a.Pay);

                    res.push_back({ umaban, payout });
                }

            }

            return res;
		}       

        bool is_valid(const filterarray::race& fa_race) {
            const auto& ra_race = fa_race.get<filter::ra_race>();
            const auto& se_race_uma = fa_race.get<filter::se_race_uma>();

            int syusson_num = get_syusso_num(ra_race);

            if (syusson_num != se_race_uma.get().size()) 
                return false;

            for (const auto& a : se_race_uma.get()) {
                if (get_ijyo_code(*a))
                    return false;
            }

            return true;
        }

        bool is_valid(const filterarray::ming& fa_ming) {
            const auto& dm_info = fa_ming.get<filter::dm_info>();
            const auto& tm_info = fa_ming.get<filter::tm_info>();

            if (dm_info.get().size() != 1)
                return false;

            if (tm_info.get().size() != 1)
                return false;

            return true;
        }

	}

};
#endif