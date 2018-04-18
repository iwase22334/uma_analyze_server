#ifndef SEQ_GENERATOR_META_HPP
#define SEQ_GENERATOR_META_HPP

namespace meta
{

	/**
	* @brief support struct for string fowarder
	*
	* @tparam Ns
	*/
	template<int N> struct val_helper
	{
		static constexpr int val = N;
	};
	
	/*
	* @brief Metafunc for express generated sequense.
	*  This struct will be made by seq_generator
	* @tparam N
	*/
	template <int... N> struct seq {};

	/**
	* @brief Metafunc for generate seq
	*
	* @tparam N
	* @tparam Ns
	*/
	template <int N, int... Ns>
	struct seq_generator : seq_generator<N - 1, N - 1, Ns...> {};
	template <int... Ns>
	struct seq_generator<0, Ns...> : seq<Ns...> {};

}



#endif