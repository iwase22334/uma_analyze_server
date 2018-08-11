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

	// Hyosu6 (Sanrentan) [byte]
	constexpr std::size_t max_record_length = 102891;
	constexpr std::size_t record_id_size = sizeof _RECORD_ID;
	constexpr std::size_t id_entory_offset = sizeof _RECORD_ID;
	constexpr std::size_t race_id_size = sizeof _RACE_ID;

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
		std::vector< std::pair<unsigned int, unsigned int> > uj_vec(horse_num);
		
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
			const unsigned int score = i * 10;
			us_vec.push_back({ umaban, score });
		}
		
		return us_vec;
	}
	
	std::string generate_se_race_uma_record(const std::string& id, const std::pair<unsigned int, unsigned int>& umaban_jyuni) {
		std::string head{ "RA7" };
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

	
	std::string generate_null_record(const std::string& id, const std::string& head) {
		assert(head.size() == 3);
		std::string record(max_record_length, '0');

		_replace_head(record, head);
		_replace_id(record, id);

		return record;
	}
}

BOOST_AUTO_TEST_SUITE(wp_estimator_unit)

BOOST_AUTO_TEST_CASE(sequens_00)
{
	BOOST_CHECK(1 == 1);
}

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
		// Set of record headder that will be hooked by race_pool
		std::list<std::string> rp_label_set{
			label::ra_race,
			label::se_race_uma,
			label::hr_pay,
			label::h1_hyosu_zenkake,
			label::h6_hyosu_sanrentan,
			label::o1_odds_tanfukuwaku,
			label::o2_odds_umaren,
			label::o3_odds_wide,
			label::o4_odds_umatan,
			label::o5_odds_sanren,
			label::o6_odds_sanrentan,
			label::wf_info,
			label::jg_jogaiba,
		};

		// Set of record hedder that will be hooked by ming_pool
		std::list<std::string> mp_label_set{
			label::dm_info,
			label::tm_info,
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
			for (const auto& head_label : rp_label_set) {
				// when head_label is not same to the label se_race_uma
				if (std::strcmp(head_label.c_str(), label::se_race_uma)) {
					race_pool(generate_null_record(curr_id, head_label));
				}

				else {
					for (const auto& umaban_jyuni : umaban_jyuni_vec) {
						race_pool(generate_se_race_uma_record(curr_id, umaban_jyuni));
					}
				}

			}

			// Flow records into ming_pool 
			for (const auto& head_label : mp_label_set) {
				if (std::strcmp(head_label.c_str(), label::tm_info)) {
					ming_pool(generate_null_record(curr_id, head_label));
				}

				else {
					ming_pool(generate_tm_info_record(curr_id, umaban_score_vec));
				}

			}

		}

	}
	/*
	// Extruct race info from datapool 
	auto race_info_list = extruct_race_info(race_pool, ming_pool);

	// Make distribution
	WinProbabilityDistribution wp_distribution(race_info_list);
	
	constexpr unsigned int horse_num = 17;
	Simulator simulator(17);
	std::vector<unsigned int> ming_point_vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

	auto result = simulator(wp_distribution, ming_point_vec);
	*/
    BOOST_CHECK(1 == 1);
}

BOOST_AUTO_TEST_SUITE_END()
