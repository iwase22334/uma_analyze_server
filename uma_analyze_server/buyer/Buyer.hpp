#ifndef BUYER_BUYER_HPP
#define BUYER_BUYER_HPP

#include <cassert>
#include <vector>

namespace strategy {
	struct Simple {
	private:
		static constexpr double profit_rate_thresh_ = 1.01;

	public:
		void operator()(std::vector<int>& vote, const std::vector<double>& rate, const std::vector<double>& winprob) const {
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

		}
	
	private:
		int make_vote(double rate, double winprob) const {
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
	template<typename U>
	static auto check(U v) -> decltype(v(), std::true_type());
	static auto check(...) -> decltype(std::false_type());

public:
	void operator()(std::vector<int>& vote, const std::vector<double>& rate, const std::vector<double>& winprob) const
	{
		static_assert( check(std::decltype<T>())::value, "Invalid buyer strategy");

		assert(vote.size() == rate.size());
		assert(vote.size() == winprob.size());

		T{}(vote, rate, winprob);
	};

};

#endif