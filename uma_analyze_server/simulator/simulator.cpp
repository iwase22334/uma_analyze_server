#include "simulator.hpp"
#include <iomanip>
const int Simulator::maximum_horse_num(8);
std::random_device Simulator::seed_gen;
std::mt19937 Simulator::rand_engine(Simulator::seed_gen());

Simulator::Simulator(const unsigned int num, 
					 const Race& r,
					 const std::list<la::Vec2d>& win_list, 
					 const std::list<la::Vec2d>& lose_list) 
: simulation_num_(num), horse_num_(r.horse_list.size()), race_(r)
{ 
	make_win_rate_table(r, win_list, lose_list);
};

void Simulator::set_simulation_num(const unsigned int num){ simulation_num_ = num; };
void Simulator::make_win_rate_table(const Race& race,
									const std::list<la::Vec2d>& vec_win_list, 
									const std::list<la::Vec2d>& vec_lose_list)
{
	std::cout << "make win rate table" << std::endl;
	unsigned int i = 0;
	for(auto a : race.horse_list){
		if(race.valid){
			std::vector<double> row;
			for(auto b : race.horse_list){
				const double band_width(10.0);
				la::Vec2d x(a.over_all_index, b.over_all_index);
				//la::Vec2d x(a.curr_index, b.curr_index);
				const double ans_win = sta::KerDensityEstimation<la::Vec2d, sta::EpanechnikovKernel<la::Vec2d> >()(band_width, x, vec_win_list);
				const double ans_lose = sta::KerDensityEstimation<la::Vec2d, sta::EpanechnikovKernel<la::Vec2d> >()(band_width, x, vec_lose_list);
				const double win_rate = ans_win / (ans_win + ans_lose);

				row.push_back(win_rate); 
				//std::cout << "estimate : " << i + 1 << " in " << horse_num_ * horse_num_ << "pairs" << std::endl;
				++ i;
			}
			win_rate_table_.push_back(row);
		}
	}

	std::cout << win_rate_table_.size() << std::endl;
};
void Simulator::run(std::vector<std::vector<double> > & place_prob)
{

	std::vector<std::vector<int> > place_count(horse_num_, std::vector<int>(horse_num_, 0));
	unsigned int simulation_end = simulation_num_;
	//std::cout << "simulation running ..." << std::endl;
	for(unsigned int i = 0; i < simulation_end; ++ i){
		//std::cout << "simulation num: " << i + 1 << " in " << simulation_num_ << std::endl;
		
		std::vector<int> place;
		
		random_start(place);
		
		// random swap
		const unsigned int replace_num(horse_num_ * horse_num_);
		for(unsigned int j = 0; j < replace_num; ++ j){
			std::uniform_int_distribution<int> distribution(0, horse_num_ - 2);
			const unsigned int comp = distribution(rand_engine);
			if(!compete(win_rate_table_[ place[comp] ][ place[comp + 1]] )){
				swap(place, comp);
			}
		}

		// update place count
		for(unsigned int j = 0; j < horse_num_; ++ j){
			++ place_count[ place[j] ][j];
		}
	}

	// calculate win probability
	place_prob.resize(horse_num_);
	for(auto& a: place_prob) a.resize(horse_num_);

	for(unsigned int i = 0; i < horse_num_; ++ i){
		for(unsigned int j = 0; j < horse_num_; ++ j){
			//std::cout << place_count[i][j] << " ";
			place_prob[i][j] = (double)place_count[i][j] / simulation_num_;
		}
		//std::cout << std::endl;
	}
}

void Simulator::random_start(std::vector<int>& place) const
{
	std::vector<bool> check_table(horse_num_, false);
	for(unsigned int i = 0; i < horse_num_; ++ i){
		std::uniform_int_distribution<int> distribution(0, horse_num_ - i - 1);
		const unsigned int num = distribution(rand_engine);

		unsigned int target(0);
		while(check_table[target]) ++ target;

		for(unsigned int j = 0; j < num; ++ j){
			++ target;
			while(check_table[target]) ++ target;
		}

		check_table[target] = true;
		place.push_back(target);
	};
};

bool Simulator::compete(const double win_rate) const
{
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	return win_rate > distribution(rand_engine);
};

void Simulator::swap(std::vector<int>& place, const unsigned int i) const
{
	const int buf = place[i];
	place[i] = place[i + 1];
	place[i + 1] = buf;
};
