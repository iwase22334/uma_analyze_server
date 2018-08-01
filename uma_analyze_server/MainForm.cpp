#include "MainForm.h"

jvdata::datapool::race umaanalyzeserver::heap::race_pool;
jvdata::datapool::ming umaanalyzeserver::heap::ming_pool;

std::list<wpestimator::mingpoint::RaceInfo> umaanalyzeserver::heap::race_info_list;
std::unique_ptr<wpestimator::mingpoint::WinProbabilityDistribution> umaanalyzeserver::heap::wp_distribution_ptr;

