#include "MainForm.h"

namespace umaanalyzeserver{
namespace heap{

jvdata::datapool::race race_pool;
jvdata::datapool::ming ming_pool;

std::list<wpestimator::mingpoint::RaceInfo> race_info_list;
std::unique_ptr<wpestimator::mingpoint::WinProbabilityDistribution> wp_distribution_ptr;

}
}


