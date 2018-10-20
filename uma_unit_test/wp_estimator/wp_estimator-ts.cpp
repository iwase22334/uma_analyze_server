#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iomanip>

#include <jv_reader/JVDataPool.hpp>

#include "wp_estimator/wp_estimator.hpp"

namespace {

	namespace {

		inline void _replace_head(std::string& record, const std::string& head) {
			constexpr std::size_t head_size = 3;
			assert(head.size() == head_size);

			record.replace(0, head_size, head);
		}

		inline void _replace_id(std::string& record, const std::string& id) {
			assert(id.size() == sizeof(_RACE_ID));

			std::size_t offset = sizeof(_RECORD_ID);
			record.replace(offset, sizeof(_RACE_ID), id);
		}

        inline void _replace_syusso_num(std::string& record, unsigned int syusso_num) {
            constexpr std::size_t syusso_num_str_length = 2;
            constexpr std::size_t syusso_num_offset = 
                sizeof(_RECORD_ID) +
                sizeof(_RACE_ID) +
                sizeof(_RACE_INFO) +
                1 +
                1 +
                sizeof(_RACE_JYOKEN) +
                60 +
                4 +
                4 +
                2 +
                2 +
                2 +
                2 +
                7 * 8 +
                5 * 8 +
                5 * 8 +
                3 * 8 +
                4 +
                4 +
                2;

            std::stringstream ss;
            ss << std::setfill('0');
            ss << std::setw(syusso_num_str_length) << syusso_num;

            record.replace(syusso_num_offset, syusso_num_str_length, ss.str());
        }

		inline void _replace_umaban_jyuni(std::string& record, const std::pair<unsigned int, unsigned int>& umaban_jyuni) {
			assert(umaban_jyuni.first <= 99);
			assert(umaban_jyuni.second <= 99);

			// replace umaban
			{
				constexpr std::size_t umaban_str_length = 2;
				constexpr std::size_t umaban_offset = sizeof(_RECORD_ID) + sizeof(_RACE_ID) + 1;

				std::stringstream ss;
				ss << std::setfill('0');
				ss << std::setw(umaban_str_length) << umaban_jyuni.first;

				record.replace(umaban_offset, umaban_str_length, ss.str());
			}

			// replace jyuni
			{
				constexpr std::size_t jyuni_str_length = 2;
				constexpr std::size_t jyuni_offset = sizeof(_RECORD_ID) + sizeof(_RACE_ID)
					+ 1 + 2 + 10 + 36 + 2 + 1 + 1 + 2 + 2 + 1 + 5 + 8 + 6 + 64 + 60 
					+ 60 + 3 + 3 + 1 + 1 + 5 + 5 + 8 + 8 + 1 + 1 + 3 + 1 + 3 + 1 + 2;

				std::stringstream ss;
				ss << std::setfill('0');
				ss << std::setw(jyuni_str_length) << umaban_jyuni.second;

				record.replace(jyuni_offset, jyuni_str_length, ss.str());
			}

		}

		inline void _replace_tm_score(std::string& record, const std::vector< std::pair<unsigned int, unsigned int> >& umaban_score_vec) {
			assert(umaban_score_vec.size() < 19);
			for (const auto& umaban_score : umaban_score_vec) {
				assert(umaban_score.first <= 99);
				assert(umaban_score.second <= 9999);
			}
			
			constexpr std::size_t umaban_str_length = 2;
			constexpr std::size_t score_str_length = 4;
			constexpr std::size_t tminfo_str_length = umaban_str_length + score_str_length;
			constexpr std::size_t base_offset = sizeof(_RECORD_ID) + sizeof(_RACE_ID) + sizeof(_HM);

			int count = 0;
			for (const auto& umaban_score : umaban_score_vec) {
				std::stringstream ss;
				ss << std::setfill('0');
				ss << std::setw(umaban_str_length) << umaban_score.first;
				ss << std::setw(score_str_length) << umaban_score.second;
				
				record.replace(base_offset + count * tminfo_str_length, tminfo_str_length, ss.str());

				++ count;
			}

		}
	}

	inline std::string to_id_string(unsigned int year, unsigned int month, unsigned int day) {
		assert(year <= 9999 && year > 999);
		assert(month <= 99);
		assert(day <= 99);

		std::stringstream ss;

		ss << std::setfill('0');
		ss << std::setw(4) << year;
		ss << std::setw(2) << month;
		ss << std::setw(2) << day; 
		ss << std::string(8, '0');

		return ss.str();
	}
	
	std::vector< std::pair<unsigned int, unsigned int> > generate_umaban_jyuni_vec(unsigned int horse_num) {
		std::vector< std::pair<unsigned int, unsigned int> > uj_vec;
		
		for (unsigned int i = 1; i <= horse_num; ++i) {
			const unsigned int umaban = i;
			const unsigned int jyuni = horse_num - i + 1;
			uj_vec.push_back({ umaban, jyuni });
		}

		return uj_vec;
	}

	std::vector< std::pair<unsigned int, unsigned int> > generate_umaban_score_vec(unsigned int horse_num) {
		std::vector< std::pair<unsigned int, unsigned int> > us_vec;
		
		for (unsigned int i = 1; i <= horse_num; ++i) {
			const unsigned int umaban = i;
			const unsigned int score = i * 10u;
			us_vec.push_back({ umaban, score });
		}
		
		return us_vec;
	}
	
    std::string generate_ra_race_record(const std::string& id) {
        std::string head{ "RA7" };
		std::string record(sizeof JV_RA_RACE, '0');
        constexpr unsigned int syusso_num = 17;

		_replace_head(record, head);
		_replace_id(record, id);
		_replace_syusso_num(record, syusso_num);

		return record;
    }

	std::string generate_se_race_uma_record(const std::string& id, const std::pair<unsigned int, unsigned int>& umaban_jyuni) {
		std::string head{ "SE7" };
		std::string record(sizeof JV_SE_RACE_UMA, '0');

		_replace_head(record, head);
		_replace_id(record, id);
		_replace_umaban_jyuni(record, umaban_jyuni);

		return record;
	}

	
	std::string generate_tm_info_record(const std::string& id, const std::vector< std::pair<unsigned int, unsigned int> >& umaban_score_vec) {
		std::string head{ "TM3" };
		std::string record(sizeof JV_TM_INFO, '0');

		_replace_head(record, head);
		_replace_id(record, id);
		_replace_tm_score(record, umaban_score_vec);

		return record;
	}
	
	std::string generate_null_record(const std::string& id, const std::string& head, std::size_t size) {
		assert(head.size() == 3);
		std::string record(size, '0');

		_replace_head(record, head);
		_replace_id(record, id);

		return record;
	}

    template<class T>
    std::ostream& operator<<(std::ostream& ost, const std::vector<T>& vec) {
        ost << "[ ";
        for (const auto& val : vec) {
            ost << val << " ";
        }
        ost << "]" ;
        return ost;
    }

}

BOOST_AUTO_TEST_SUITE(wp_estimator_unit)

BOOST_AUTO_TEST_CASE(sequens_01) {

	// shorten namespace name
	namespace datapool = jvdata::datapool;
	namespace label = jvdata::label;
		
	using wpestimator::mingpoint::extruct_race_info;
	using wpestimator::mingpoint::RaceInfo;
	using wpestimator::mingpoint::WinProbabilityDistribution;
	using wpestimator::mingpoint::Simulator;

	datapool::race race_pool;
	datapool::ming ming_pool;
	// Fill two pools 
	{
		using label_size_pair = std::pair<std::string, std::size_t>;
		// Set of record headder that will be hooked by race_pool
		std::list<label_size_pair> rp_label_set{

			{label::ra_race, sizeof(JV_RA_RACE)},
			{label::se_race_uma, sizeof(JV_SE_RACE_UMA)},
			{label::hr_pay, sizeof(JV_HR_PAY)},
			{label::h1_hyosu_zenkake, sizeof(JV_H1_HYOSU_ZENKAKE)},
			{label::h6_hyosu_sanrentan, sizeof(JV_H6_HYOSU_SANRENTAN)},
			{label::o1_odds_tanfukuwaku, sizeof(JV_O1_ODDS_TANFUKUWAKU)},
			{label::o2_odds_umaren, sizeof(JV_O2_ODDS_UMAREN)},
			{label::o3_odds_wide, sizeof(JV_O3_ODDS_WIDE)},
			{label::o4_odds_umatan, sizeof(JV_O4_ODDS_UMATAN)},
			{label::o5_odds_sanren, sizeof(JV_O5_ODDS_SANREN)},
			{label::o6_odds_sanrentan, sizeof(JV_O6_ODDS_SANRENTAN)},
			//{label::wf_info, sizeof(JV_WF_INFO)},
			{label::jg_jogaiba, sizeof(JV_JG_JOGAIBA)}

		};

		// Set of record hedder that will be hooked by ming_pool
		std::list<label_size_pair> mp_label_set{

			{label::dm_info, sizeof(JV_DM_INFO)},
			{label::tm_info, sizeof(JV_TM_INFO)}

		};
		
		constexpr unsigned int horse_num = 17;
		constexpr unsigned int year = 2001;
		constexpr unsigned int month = 1;
		constexpr unsigned int day = 1;
		constexpr unsigned int loop_max = 30;
		
		for (int i = 0; i < loop_max; ++i) {
			std::string curr_id = to_id_string(year, month, day + i);
			std::vector< std::pair<unsigned int, unsigned int> > umaban_jyuni_vec = generate_umaban_jyuni_vec(horse_num);
			std::vector< std::pair<unsigned int, unsigned int> > umaban_score_vec = generate_umaban_score_vec(horse_num);

			// Flow records into race_pool 
			for (const auto& rp_label : rp_label_set) {

                if (std::strcmp(rp_label.first.c_str(), label::se_race_uma) == 0) {
                    for (const auto& umaban_jyuni : umaban_jyuni_vec) {
                        auto res = race_pool(generate_se_race_uma_record(curr_id, umaban_jyuni));
                        BOOST_CHECK(jvdata::is_same_id(res.get(), jvdata::to_id_type(curr_id)) );
                    }
                }
                
                else if (std::strcmp(rp_label.first.c_str(), label::ra_race) == 0) {
                    auto res = race_pool(generate_ra_race_record(curr_id));
                    BOOST_CHECK(jvdata::is_same_id(res.get(), jvdata::to_id_type(curr_id)) );
                }

				// when head_label is not same to the label se_race_uma
                else {
					auto res = race_pool(generate_null_record(curr_id, rp_label.first, rp_label.second));
                    BOOST_CHECK(res);
                    BOOST_CHECK(jvdata::is_same_id(*res, jvdata::to_id_type(curr_id) ));

				}


			}

			// Flow records into ming_pool 
			for (const auto& mp_label : mp_label_set) {
				if (std::strcmp(mp_label.first.c_str(), label::tm_info) == 0) {
                    ming_pool(generate_tm_info_record(curr_id, umaban_score_vec));
				}

				else {
                    ming_pool(generate_null_record(curr_id, mp_label.first, mp_label.second));
				}

			}

		}

	}
	
	// Extruct race info from datapool 
	auto race_info_list = extruct_race_info(race_pool, ming_pool);

	// Make distribution
	WinProbabilityDistribution wp_distribution(race_info_list);
	
	constexpr unsigned int horse_num = 17;
	Simulator simulator(horse_num);
	std::vector<unsigned int> ming_point_vec{1, 7, 13, 19, 25, 31, 37, 33, 39, 45, 51, 57, 63, 70, 76, 82, 88};

	auto result = simulator(wp_distribution, ming_point_vec);
   
    std::cout << std::fixed;
    std::cout << "ming_point_vec: " << std::setprecision(1) << ming_point_vec << std::endl;
    std::cout << "result        : " << std::endl;
    
    for (const auto& r : result) {
        std::cout << std::setprecision(2) << r << std::endl;
    }

    constexpr unsigned int step = 5;
    constexpr unsigned int point_num = 20;
    for (unsigned int i = 0; i < point_num; ++i) {
        for (unsigned int j = 0; j < point_num; ++j) {
            std::cout << std::setprecision(4) << wp_distribution({ i * step, j * step }) << ' ';
        }
        std::cout << std::endl;
    }

    BOOST_CHECK(1 == 1);
}

BOOST_AUTO_TEST_SUITE_END()

