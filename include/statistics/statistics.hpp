#ifndef STATISTICS_HPP
#define STATISTICS_HPP
#include <list>
#include <cmath>

#ifndef M_PI
#define M_PI 3.1415927
#endif

namespace statistics{

	/**
	 * @brief Kernel for KerDensityEstimation
	 * 
	 * @tparam T 
	 */
	template<class T, class F_Type = std::false_type>
	struct EpanechnikovKernel
	{
		
		auto operator()(const T& x) -> decltype(norm(x), double())
		{
			return 1.0 / sqrt(2 * M_PI) * exp(- norm(x) * norm(x) / 2 );
		};

		auto operator()(...) -> double
		{
			static_assert(F_Type::value, "Not defined norm function");
		};

	};

	/**
	 * @brief Kernel for KErDensityEstimation
	 * 
	 * @tparam  
	 */
	template<>
	class EpanechnikovKernel<double>
	{
		double operator()(const double& x)
		{
			return 1.0 / sqrt(2 * M_PI) * exp(- x * x / 2 );
		};
	};

	/**
	 * @brief kernel density estimation
	 * 
	 * @tparam I  
	 * @tparam T_Kernel 
	 */
	template<class I, class T_Kernel>
	struct KDensityEstimator
	{
		double operator()(const double h, const I& x, const std::list<I>& sample)
		{
			double sum = 0;
			for_each(sample.begin(), sample.end(), [&](const I& xi){ sum += T_Kernel{}( (x - xi) / h ); });
			return sum / (sample.size() * h);
		};

		double operator()(const double h, const I& x, const I& xi) 
		{
			return T_Kernel{}( (x - xi) / h );
		};
	};
};
#endif