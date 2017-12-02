#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include <cstdint>
#include <iostream>
#include <list>

struct MiningData {
	int16_t record;
	int16_t error_plus;
	int16_t error_minus;
};

struct Horse
{
	std::string date;
	std::string name;
	std::string pedigree;
	std::string jockey;
	//! Numero of horse 
	uint8_t no;
	uint8_t age;
	//! result of the race
	uint8_t rank;
	//! jra's forecast
	uint8_t forecasted_place;
	int16_t weight;
	//! weight difference from previous race [Kg]
	int16_t weight_diff;
	//! record [s]
	int16_t record;
	//! jra's forecast about record
	MiningData mining_data;
};

struct Race
{
	std::string name;
	std::string year;
	std::string date;
	std::string track_code;
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

std::ostream& operator<<(std::ostream& os, const Race& r);
std::ostream& operator<<(std::ostream& os, const Horse& h);
std::ostream& operator<<(std::ostream& os, const TrackInfo& t);

namespace strout
{
	struct HorseDescription{};
	struct Line{};
	struct DoubleLine{};
};

std::ostream& operator<<(std::ostream& os, const strout::HorseDescription& h);
std::ostream& operator<<(std::ostream& os, const strout::Line& h);
std::ostream& operator<<(std::ostream& os, const strout::DoubleLine& h);

#endif
