#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP
#include <random>
#include <vector>
#include <algorithm>
#include "../analyzer/DataStructure.hpp"
#include "../analyzer/statistics.hpp"
#include "../linear/linear.hpp"

class Simulator{
private:
	static const int maximum_horse_num;
	static std::random_device seed_gen;
	static std::mt19937 rand_engine;

private:
	unsigned int simulation_num_;
	unsigned int horse_num_;
	Race race_;
	std::vector<std::vector<double> > win_rate_table_;

public:
	//----------------------------------------------------------
	// constructor
	// Parameters:
	// 		num 	simulatino num
	//		win_list 	win horse's value list
	//		lose_list	lose horse's value list
	//----------------------------------------------------------
	Simulator(const unsigned int num, 
				const Race& r,
				const std::list<la::Vec2d>& win_list, 
				const std::list<la::Vec2d>& lose_list);
	void set_simulation_num(const unsigned int num);
	//----------------------------------------------------------
	// 		montecarlo simulaton
	//		Predict the order probability 
	// Parameters:
	// 		place_prob 	return value. probability array
	//					A 1 2 3 4 ...
	//					B 1 2 3 4 ...
	//					. . .
	//----------------------------------------------------------
	void run(std::vector<std::vector<double> > & place_prob);
private:
	//----------------------------------------------------------
	// make_win_rate_table
	//		win 			
	// Parameters:
	// 		win_rate_table 	return value. probability array
	//					   A   B   C   D
	//					A  -  0.3 0.3 0.4 ...
	//					B 0.7  -  0.5 0.7 ...
	//					. . .
	//		win_list 	win horse's value list
	//		lose_list	lose horse's value list
	//----------------------------------------------------------
	void make_win_rate_table(const Race& race,
							 const std::list<la::Vec2d>& win_list, 
							 const std::list<la::Vec2d>& lose_list);
	// Used in run() compete by win_rate
	bool compete(const double win_rate) const;
	// replace front and behind
	void swap(std::vector<int>& place, const unsigned int i) const;
	void random_start(std::vector<int>& place) const;
	void check_place(std::vector<int>& place,
				 const std::vector<int>& win_num_vector) const;
};
#endif
