#include <boost/test/unit_test.hpp>

#include <iostream>
#include <algorithm>
#include "buyer/buyer.hpp"

namespace {
    
    template<class T>
    std::ostream& operator<<(std::ostream& ost, const std::vector<T>& vec) {
        ost << "[ ";
        for (const auto& val : vec) {
            ost << val << " ";
        }
        ost << "]" ;
        return ost;
    }

}

BOOST_AUTO_TEST_SUITE(buyer_sequence_test) 

BOOST_AUTO_TEST_CASE(buyer_sequence_test_case1) {
    constexpr unsigned int horse_num = 15;

    std::vector<double> win_prob {
       0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 
    };

    std::vector<double> rate {
       2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0
    };

    std::vector<unsigned int> expected_vote{
         0,   0,   0,   0,   0,   1,   1,   1,   1,   1
    };

    Buyer<strategy::Simple> buyer;
    auto vote = buyer(rate, win_prob);
    
    std::cout << win_prob << std::endl;
    std::cout << rate << std::endl;
    std::cout << vote << std::endl;

    BOOST_CHECK(std::equal(vote.begin(), vote.end(), expected_vote.begin()));
}

BOOST_AUTO_TEST_SUITE_END()