#include <boost/test/unit_test.hpp>

#include <wp_estimator/wp_estimator.hpp>
#include <jv_reader/JVDataPool.hpp>

// Hyosu6 (Sanrentan) [byte]
constexpr std::size_t max_record_length = 102891;
constexpr std::size_t id_entory_offset = sizeof _RECORD_ID;

static inline
_RACE_ID generate_race_id(unsigned int year, unsigned int mohth, unsigned int day) {
	assert(year < 9999);
	assert(month < 99);
	assert(day < 99)
	_RACE_ID id{};

}

static inline
void replace_head(std::string& record, const std::string& head) {
	assert(head.size() == 3);
	record.replace(0, 3, head);
}

static inline
std::string generate_race_record(unsigned int id ) {
	std::string head{"RA7"};
	std::string record(sizeof JV_RA_RACE, '0');

	replace_head(record, head);
	return record;
}

static inline
std::string generate_ming_record(unsigned int id) {
	std::string head["DM3"];
	std::string record(sizeof JV_TM_INFO, '0');
	
	replace_head(record, head);
	return record;
}

static inline
std::string generate_null_record(unsigned int id, const std::string& head) {
	assert(head.size == 3);
	std::string record(max_record_length, '0');

	replace_head(record, head);
	return record;
}

BOOST_AUTO_TEST_SUITE(wp_estimator_unit)

BOOST_AUTO_TEST_CASE(sequens_01) {

	namespace datapool = jvdata::datapool;
	namespace label = jvdata::label;
		
	datapool::race race_pool;
	datapool::ming ming_pool;

	using wpestimator::mingpoint::extruct_race_info;
	using wpestimator::mingpoint::RaceInfo;
	using wpestimator::mingpoint::WinProbabilityDistribution;
	using wpestimator::mingpoint::Simulator;

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

	std::list<std::string> mp_label_set{
		label::dm_info,
		label::tm_info,
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
