#include "DataStructure.hpp"
#include <iomanip>
#include <string>
#include <iostream>
std::ostream& operator<<(std::ostream& os, const strout::HorseDescription& h)
{
	os << strout::Line();
	os << std::setw(2) << "No" << " ";
	os << std::setw(2) << "SI" << " ";
	os << std::setw(2) << "CI" << " ";
	os << std::setw(5) << "Pay" << " ";
	os << std::setw(2) << "Pl" << " ";
	os << "name" << std::endl;
	os << strout::Line();
	return os;
};
std::ostream& operator<<(std::ostream& os, const strout::Line& l)
{
	os << "-------------------------------------" << std::endl;
	return os;
};
std::ostream& operator<<(std::ostream& os, const strout::DoubleLine& l)
{
	os << "=====================================" << std::endl;
	return os;
};
std::ostream& operator<<(std::ostream& os, const Horse& h)
{
	os << std::setw(2) << h.no << " ";
	os << std::setw(2) << h.over_all_index << " ";
	os << std::setw(2) << h.curr_index << " ";
	os << std::setw(5) << h.pay_rate << " ";
	os << std::setw(2) << h.place << " ";
	os << h.name << std::endl;
	return os;
};
std::ostream& operator<<(std::ostream& os, const Race& r)
{
	os << r.name << " " << "horse number: " << r.horse_list.size() << std::endl;
	return os;
};
std::ostream& operator<<(std::ostream& os, const TrackInfo& t)
{
	os << t.date << " " << "race number: " << t.race_list.size() << std::endl;
	return os;
};
