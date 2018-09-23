#ifndef BUYER_BUYER_HPP
#define BUYER_BUYER_HPP

#include <cassert>
#include <vector>

namespace strategy {

	struct Simple {
	private:
		static constexpr double profit_rate_thresh_ = 1.01;

	public:
		std::vector<unsigned int> operator()(const std::vector<double>& rate, const std::vector<double>& winprob) const {
            assert(rate.size() == winprob.size());

            std::vector<unsigned int> vote(rate.size());

			auto vote_it = vote.begin();
			auto rate_it = rate.cbegin();
			auto winprob_it = winprob.cbegin();

			auto next = [&]() {
				++ vote_it;
				++ rate_it;
				++ winprob_it;
			};
			
			// check all horse
			while (vote_it != vote.cend()) {
				*vote_it = make_vote(*rate_it, *winprob_it);
				next();
			}

            return vote;

		}
	
	private:
		unsigned int make_vote(double rate, double winprob) const {
			double excepted_profit_rate = rate * winprob;

			if (excepted_profit_rate > profit_rate_thresh_) {
				return 1;
			}

			return 0;
		}

	};
};

template<class T>
struct Buyer {
private:
    
    template<class U>
    struct strategy_checker {
        
        template<class U_>
	    static constexpr auto check(U_ v) -> 
            decltype(v(std::declval<std::vector<double> >(), std::declval<std::vector<double> >()), std::true_type());

	    static constexpr auto check(...) -> decltype(std::false_type());

        typedef decltype(check(std::declval<U>())) type;
        static constexpr bool value = type::value;

    };

public:
	std::vector<unsigned int> operator()(const std::vector<double>& rate, const std::vector<double>& winprob) const
	{
		static_assert( strategy_checker<T>::value, "Invalid buyer strategy");

		assert(rate.size() == winprob.size());

        for (auto a : rate) {
            assert(a >= 1.0);
        }

		return T{}(rate, winprob);
	};

};

#endif