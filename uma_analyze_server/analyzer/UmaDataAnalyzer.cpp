#include "UmaDataAnalyzer.hpp" 

bool uda::GetWinSample::compare(const Horse& h1, const Horse& h2) const
{
	return h1.rank < h2.rank;
};
bool uda::GetLoseSample::compare(const Horse& h1, const Horse& h2) const
{
	return h1.rank > h2.rank;
};