#ifndef IMPL_JV_RECORD_READER_IPP
#define IMPL_JV_RECORD_READER_IPP

template<typename T, char C1, char C2, char C3> struct JVRecordFilter;
template<class... Filters> class JVFilterArray;

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
template <int... N> struct _seq {};

/**
 * @brief Metafunc for generate seq
 * 
 * @tparam N 
 * @tparam Ns 
 */
template <int N, int... Ns>
struct _seq_generator : _seq_generator<N - 1, N - 1, Ns...> {};
template <int... Ns>
struct _seq_generator<0, Ns...> : _seq<Ns...> {};

template <class T>
struct _false_value
{
    static constexpr bool val = false;    
};

template<class Tuple, int N>
struct _Validator {

    template<class Head, class... Tail>
    bool operator()(const _RACE_ID& id, const Tuple& filters, const Head& h, const Tail&... tail) 
    {
		bool res = true;

        for(const auto& fal : std::get<Head::val>(filters).fallen_list) {
            res &= !std::memcmp(&(fal->id), &id, sizeof(_RACE_ID));
        }

        return res & (*this)(id, filters, tail...);
    };

    template<class... T>
    bool operator()(const _RACE_ID& id, const Tuple& filters, const val_helper<N - 1>&) 
    {
        return true;
    };

};

template<class T, int Head, int... Tail>
bool _is_valid(const T& fa, const _seq<Head, Tail...>& dum)
{
    const typename T::tuple_type& t = fa.get_filters();
    return _Validator<typename T::tuple_type, T::filter_size>{}
                        (std::get<0>(t).get().front()->id,
                         t, 
                         val_helper<Head>{}, 
                         val_helper<Tail>{}...);
}



#endif