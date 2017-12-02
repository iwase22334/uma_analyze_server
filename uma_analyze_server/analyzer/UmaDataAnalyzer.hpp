#ifndef UMA_DATA_ANALYZER_HPP
#define UMA_DATA_ANALYZER_HPP
//----------------------------------------------------------
// UmaDataAnalyzer
// Analyze race Data
//----------------------------------------------------------
#include <list>
#include "../linear/linear.hpp"
#include "DataStructure.hpp"
namespace uda{
	//----------------------------------------------------------
	// compare_all_sample
	// 		compare all inputed samples by class T.
	// Template parameters:
	// 		T 		       compared object 
	// 		T_Comp         class that Make Decision 
	// Parameters:
	// 		dst       destination 
	// 		src   	  sorce
	//----------------------------------------------------------
	template<class T, class T_Comp>
	static inline void compare_all_sample( std::list<std::pair<T, T> >& dst, const std::list<T>& src)
	{
		auto it1 = src.begin();
		auto it2 = std::next(it1);
		while(std::next(it1) != src.end() ){
			while(it2 != src.end()){
				if(T_Comp().compare(*it1, *it2)){ dst.push_back(std::pair<T, T>(*it1, *it2)); }
				if(T_Comp().compare(*it2, *it1)){ dst.push_back(std::pair<T, T>(*it2, *it1)); }
				++ it2;
			}
			++ it1;
			it2 = std::next(it1);	
		} 
	};

	//----------------------------------------------------------
	// GetWinSample
	// Parameters:
	// 		dst       destination 
	// 		src   	  sorce
	//----------------------------------------------------------
	struct GetWinSample
	{
		bool compare(const Horse&, const Horse&) const;
	};

	//----------------------------------------------------------
	// LoseWinSample
	// Parameters:
	// 		dst       destination 
	// 		src   	  sorce
	//----------------------------------------------------------
	struct GetLoseSample
	{
		bool compare(const Horse&, const Horse&) const;
	};

	//----------------------------------------------------------
	// pair_to_vec
	// Parameters:
	// 		dst       destination 
	// 		src   	  sorce
	//----------------------------------------------------------
	static inline void pair_to_vec( std::list<la::Vec2d>& dst, std::list<std::pair<Horse, Horse> >& src)
	{
		for(auto a : src){
			dst.push_back(la::Vec2d(a.first.curr_index, a.second.curr_index));
		}
	};
}
#endif