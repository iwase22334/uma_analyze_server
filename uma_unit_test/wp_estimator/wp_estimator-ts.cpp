#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iomanip>

#include <wp_estimator/wp_estimator.hpp>
#include <jv_reader/JVDataPool.hpp>

namespace {

	// Hyosu6 (Sanrentan) [byte]
	constexpr std::size_t max_record_length = 102891;
	constexpr std::size_t record_id_size = sizeof _RECORD_ID;
	constexpr std::size_t id_entory_offset = sizeof _RECORD_ID;
	constexpr std::size_t race_id_size = sizeof _RACE_ID;

	inline void _replace_head(std::string& record, const std::string& head) {
		constexpr std::size_t head_size = 3;
		assert(head.size() == head_size);

		record.replace(0, head_size, head);
	}

	inline void _replace_id(std::string& record, const std::string& id) {
		assert(id.size() == race_id_size);

		std::size_t offset = record_id_size;
		record.replace(offset, race_id_size, id);
	}

	inline void _replace_umaban_jyuni(std::string& record, const std::pair<unsigned int, unsigned int>& umaban_jyuni) {
		assert(umaban_jyuni.first <= 99);
		assert(umaban_jyuni.second <= 99);

		// replace umaban
		{
			constexpr std::size_t umaban_str_length = 2;
			constexpr std::size_t umaban_offset = ;

			std::stringstream ss;
			ss << std::setfill('0');
			ss << std::setw(umaban_str_length) << umaban_jyuni.first;

			record.replace(umaban_offset, umaban_str_length, ss.str());
		}

		// replace jyuni
		{
			constexpr std::size_t jyuni_str_length= 2;
			constexpr std::size_t jyuni_offset = ;

			std::stringstream ss;
			ss << std::setfill('0');
			ss << std::setw(jyuni_str_length) << umaban_jyuni.second;

			record.replace(jyuni_offset, jyuni_str_length, ss.str());
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
	
	std::string generate_se_race_uma_record(const std::string& id, const std::pair<unsigned int, unsigned int>& umaban_jyuni) {
		std::string head{ "RA7" };
		std::string record(sizeof JV_SE_RACE_UMA, '0');

		_replace_head(record, head);
		_replace_id(record, id);
		_replace_umaban_jyuni(record, umaban_jyuni);

		return record;
	}

	
	std::string generate_dm_info_record(const std::string& id) {
		std::string head["DM3"];
		std::string record(sizeof JV_DM_INFO, '0');

		_replace_head(record, head);
		_replace_id(record, id);

		return record;
	}

	
	std::string generate_null_record(const std::string& id, const std::string& head) {
		assert(head.size == 3);
		std::string record(max_record_length, '0');

		_replace_head(record, head);
		_replace_id(record, id);

		return record;
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

		constexpr unsigned int year = 2001;
		constexpr unsigned int month = 1;
		constexpr unsigned int day = 1;
		constexpr unsigned int loop_max = 30;

		for (int i = 0; i < loop_max; ++i) {
			std::string curr_id = to_id_string(year, month, day + i);

			// Flow records into race_pool 
			for (const auto& head_label : rp_label_set) {
				// when head_label is not same to the label se_race_uma
				if (std::strcmp(*head_label, label::se_race_uma)) {
					race_pool(generate_null_record(curr_id, head_label));
				}

				else {
					race_pool(generate_se_race_uma_record(curr_id));
				}

			}

			// Flow records into ming_pool 
			for (const auto& head_label : mp_label_set) {
				if (std::strcmp(*head_label, label::dm_info)) {
					ming_pool(generate_null_record(curr_id, head_label));
				}

				else {
					ming_pool(generate_dm_info_record(curr_id));
				}

			}

		}

	}

	// Extruct race info from datapool 
	auto race_info_list = extruct_race_info(race_pool, ming_pool);

	// Make distribution
	WinProbabilityDistribution wp_distribution(race_info_list);
	
	constexpr unsigned int horse_num = 17;
	Simulator simulator(17);
	std::vector<int> ming_point_vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

	auto result = simulator(wp_distribution, ming_point_vec);

}

BOOST_AUTO_TEST_SUITE_END()
