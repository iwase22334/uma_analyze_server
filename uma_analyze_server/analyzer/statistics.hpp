#ifndef STATISTICS_HPP
#define STATISTICS_HPP
#include <list>
#include <cmath>

namespace sta{
	//----------------------------------------------------------
	// Epanechnikov Kernel
	// Template parameters:
	// 		T         Input class
	// Parameters:
	// 		x   	  function input
	//----------------------------------------------------------
	template<class T>
	struct EpanechnikovKernel
	{
		double operator()(const T& x)
		{
			return 1.0 / sqrt(2 * M_PI) * exp(- norm(x) * norm(x) / 2 );
		};
	};
	template<>
	class EpanechnikovKernel<double>
	{
		double operator()(const double& x)
		{
			return 1.0 / sqrt(2 * M_PI) * exp(- x * x / 2 );
		};
	};

	//----------------------------------------------------------
	// Kernel Density Estimation 
	// Template parameters:
	// 		I         Input class
	// 		T_Kernel  Kernel function object
	// Parameters:
	// 		h         band width
	// 		x   	  function input
	//		sample    sample list
	//----------------------------------------------------------
	template<class I, class T_Kernel>
	struct KerDensityEstimation
	{
		double operator()(const double h, const I x, const std::list<I>& sample)
		{
			double sum = 0;
			for_each(sample.begin(), sample.end(), [&](const I& xi){ sum += T_Kernel()( (x - xi) / h ); });
			return sum / (sample.size() * h);
		};
	};
};
#endif