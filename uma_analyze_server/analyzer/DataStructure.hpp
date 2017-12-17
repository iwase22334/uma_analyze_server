#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <list>
#include <array>
#include <utility>

typedef std::array<uint8_t, 2> pair_t;
typedef std::array<uint8_t, 3> trio_t;
typedef std::array<uint32_t, 2> range_t;

namespace data
{

	/**
	 * @brief mining data object
	 * mining data are published from JRA
	 * 
	 */
	struct MiningData 
	{
		int16_t record;
		int16_t error_plus;
		int16_t error_minus;
	};

	/**
	 * @brief Race identifier.
	 * 
	 */
	struct RaceIdentifier 
	{
		std::string year;
		std::string date;
		std::string track_code;
		std::string year_race_no;
		std::string daily_race_no;
	};

	/**
	 * @brief Horse
	 * 
	 */
	struct Horse
	{
		RaceIdentifier race_id;
		std::string name; 
		std::string pedigree_id;
		std::string jockey;
		//! Numero of horse 
		uint8_t no;
		uint8_t age;
		//! result of the race
		uint8_t rank;
		//! jra's forecast
		uint8_t forecasted_place;
		int16_t weight;
		int16_t weight_diff_sign;
		//! weight difference from previous race [Kg]
		int16_t weight_diff;
		//! record [s]
		int16_t record;
		//! [kg]
		float burden_weight;
		//! jra's forecast about record
		MiningData mining_data;
	};

	/**
	 * @brief Race
	 * 
	 */
	struct Race
	{
		RaceIdentifier race_id;
		std::string name;
		std::string grade_code;
		std::string race_type_code;
		std::string race_symbol_code;
		std::string weight_type_code;
		std::string track_type_code;
		std::string weather_code;
		std::string baba_state_code;
		//! [m]
		uint16_t length;
		std::list<Horse> horse_list;
	};

	/**
	 * @brief payout
	 * 
	 */
	namespace payout
	{

		struct Tanshou
		{
			bool valid;
			uint8_t no;
			uint32_t payout;
			uint8_t popularity;
		};

		struct Hukusyou
		{
			bool valid;
			uint8_t no;
			uint32_t payout;
			uint8_t popularity;
		};

		struct Wakuren
		{
			bool valid;
			uint8_t no;
			uint32_t payout;
			uint8_t popularity;
		};

		struct Umaren
		{
			bool valid;
			pair_t pair;
			uint32_t payout;
			uint16_t popularity;
		};

		struct Wide
		{
			bool valid;
			pair_t pair;
			uint32_t payout;
			uint16_t popularity;
		};

		struct Umatan
		{
			bool valid;
			pair_t pair;
			uint32_t payout;
			uint16_t popularity;
		};

		struct Sanrentan
		{
			bool valid;
			trio_t trio;
			uint32_t payout;
			uint16_t popularity;
		};

		struct Sanrenpuku
		{
			bool valid;
			trio_t trio;
			uint32_t payout;
			uint16_t popularity;
		};

	};

	struct PayoutManager
	{
		RaceIdentifier race_id;
		std::list<payout::Tanshou> tanshou;
		std::list<payout::Hukusyou> hukushou;
		std::list<payout::Wakuren> wakuren;
		std::list<payout::Umaren> umaren;
		std::list<payout::Wide> wide;
		std::list<payout::Umatan> umatan;
		std::list<payout::Sanrentan> sanrentan;
		std::list<payout::Sanrenpuku> sanrenpuku;
	};

	namespace vote
	{
		
		struct Tanshou
		{
			bool valid;
			uint8_t no;
			uint32_t vote;
			uint8_t popularity;
		};

		struct Hukusyou
		{
			bool valid;
			uint8_t no;
			uint32_t vote;
			uint8_t popularity;
		};

		struct Wakuren
		{
			bool valid;
			uint8_t no;
			uint32_t vote;
			uint8_t popularity;
		};

		struct Umaren
		{
			bool valid;
			pair_t pair;
			uint32_t vote;
			uint16_t popularity;
		};

		struct Wide
		{
			bool valid;
			pair_t pair;
			uint32_t vote;
			uint16_t popularity;
		};

		struct Umatan
		{
			bool valid;
			pair_t pair;
			uint32_t vote;
			uint16_t popularity;
		};

		struct Sanrentan
		{
			bool valid;
			trio_t trio;
			uint32_t vote;
			uint16_t popularity;
		};

		struct Sanrenpuku
		{
			bool valid;
			trio_t trio;
			uint32_t vote;
			uint16_t popularity;
		};

	};

	struct VoteManager 
	{
		RaceIdentifier race_id;
		std::list<vote::Tanshou> tanshou;
		std::list<vote::Hukusyou> hukushou;
		std::list<vote::Wakuren> wakuren;
		std::list<vote::Umaren> umaren;
		std::list<vote::Wide> wide;
		std::list<vote::Umatan> umatan;
		std::list<vote::Sanrentan> sanrentan;
		std::list<vote::Sanrenpuku> sanrenpuku;
	};

	namespace odds
	{
		
		struct Tanshou
		{
			bool valid;
			uint8_t no;
			uint32_t odds;
			uint8_t popularity;
		};

		struct Hukusyou
		{
			bool valid;
			uint8_t no;
			range_t odds_range;
			uint8_t popularity;
		};

		struct Wakuren
		{
			bool valid;
			uint8_t no;
			uint32_t odds;
			uint8_t popularity;
		};

		struct Umaren
		{
			bool valid;
			pair_t pair;
			uint32_t odds;
			uint16_t popularity;
		};

		struct Umatan
		{
			bool valid;
			pair_t pair;
			uint32_t odds;
			uint16_t popularity;
		};

		struct Wide
		{
			bool valid;
			pair_t pair;
			range_t odds_range;
			uint16_t popularity;
		};

		struct Sanrentan
		{
			bool valid;
			trio_t trio;
			uint32_t odds;
			uint16_t popularity;
		};

		struct Sanrenpuku
		{
			bool valid;
			trio_t trio;
			uint32_t odds;
			uint16_t popularity;
		};

	};

	struct OddsManager 
	{
		RaceIdentifier race_id;
		std::list<odds::Tanshou> tanshou;
		std::list<odds::Hukusyou> hukushou;
		std::list<odds::Wakuren> wakuren;
		std::list<odds::Umaren> umaren;
		std::list<odds::Wide> wide;
		std::list<odds::Umatan> umatan;
		std::list<odds::Sanrentan> sanrentan;
		std::list<odds::Sanrenpuku> sanrenpuku;
	};

	static std::ostream& operator<<(std::ostream& os, const Race& r)
	{
		os << r.name << std::endl;
		return os;
	};

	static std::ostream& operator<<(std::ostream& os, const Horse& h)
	{
		os << std::setw(2) << h.weight << " ";
		return os;
	};

}

namespace strout
{
	struct Line{};
	struct DoubleLine{};

	static std::ostream& operator<<(std::ostream& os, const strout::Line& h)
	{
		os << "-------------------------------------" << std::endl;
		return os;
	}

	static std::ostream& operator<<(std::ostream& os, const strout::DoubleLine& l)
	{
		os << "=====================================" << std::endl;
		return os;
	};
};

#endif
