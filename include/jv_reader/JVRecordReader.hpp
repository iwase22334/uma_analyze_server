#ifndef JV_RECORD_READER_HPP
#define JV_RECORD_READER_HPP

#include <string>
#include <memory>
#include <tuple>
#include <cassert>
#include "JVData_Structure.h"

/**
 * @brief record value reader
 * 
 * @tparam T 
 * @tparam C1 
 * @tparam C2 
 */
template<typename T, char C1, char C2>
struct JVRecordFilter {
    std::unique_ptr<T> result_ptr;
    
    JVRecordFilter() : result_ptr(new T()) {};

    bool read(const std::string str) {

        if (str.c_str()[0] == C1 && str.c_str()[1] == C2) {
            assert(sizeof(T) == str.size());
            result_ptr.reset(new T());
            std::memcpy(result_ptr.get(), str.c_str(), sizeof(T));
            return true;
        }
        
        return false;
    };

};

template<class... Filters>
class JVFilterArray {
private:
    static constexpr std::size_t filter_size_ = sizeof...(Filters);
    std::tuple<Filters...> filter_tuple_;
    std::array<bool, filter_size_> caught_;

private:
    /**
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

private:
    /**
     * @brief Metafunc for getting first found index of target class.
     * 
     * @tparam Index result
     * @tparam Target 
     * @tparam Head 
     * @tparam Tail 
     */
    template<int Index, class Target, class Head, class... Tail >
    struct type_finder
    {
        typedef typename type_finder<Index + 1, Target, Tail...>::type type;
        static constexpr int index = Index;
    };

    template<int Index, class Target, class... Types>
    struct type_finder<Index, Target, Target, Types...>
    {
        typedef type_finder<Index, Target, Target, Types...> type;
        static constexpr int index = Index;
    };

    /**
     * @brief support struct for string fowarder
     * 
     * @tparam Ns 
     */
    template<int... Ns> struct pack {};

    template<int N> struct val_helper
    {
        static constexpr int val = N;
    };

    /**
     * @brief Metafunc for fowarding string to next filter
     * 
     * @tparam T 
     * @tparam Head 
     * @tparam Tail 
     */
    template<class Head, class... Tail>
    struct string_fowarder
    {
        void operator()(std::array<bool, filter_size_>& res, 
                        std::tuple<Filters...>& fs,
                        const std::string& str)
        {
            res[Head::val] = std::get<Head::val>(fs).read(str) ? true : res[Head::val];
            string_fowarder<Tail...>{}(res, fs, str);
        };
    };

    template <class... Tail>
    struct string_fowarder< val_helper<filter_size_ - 1>, Tail...>
    {
        void operator()(std::array<bool, filter_size_>& res,
                        std::tuple<Filters...>& fs,
                        const std::string& str)
        {
            res[filter_size_ - 1] = std::get<filter_size_ - 1>(fs).read(str) ? true : res[filter_size_ - 1]; 
        };
    };

    /**
     * @brief Input str to each filters in the tuple.
     * 
     * @tparam Ns 
     * @param str 
     * @param dummy 
     */
    template<int Head, int... Tail>
    void filter_all_(const std::string& str, const seq<Head, Tail...> dummy)
    {
        // filter all
        string_fowarder<val_helper<Head>, val_helper<Tail>...>
                        {}(
                            this->caught_,
                            this->filter_tuple_,
                            str
                        );
    };
    
public:
    JVFilterArray() : filter_tuple_(), caught_{{false}} 
    {
        static_assert(filter_size_ > 0, "parameter size must greater than 0");
    };

    void operator()(const std::string& str) 
    {
        filter_all_(str, seq_generator<filter_size_>());
    };

    void reset() 
    {
        for (auto a : caught_) a = false;
    };

    bool is_caught_all() const
    {
        for (auto a : caught_) {
            if (!a) return false;
        } 
        return true;
    };

    bool is_caught(int i) const 
    {
        return caught_[i];
    } 
    
    const std::tuple<Filters...>& get_filters() const 
    {
        return filter_tuple_;
    };

    template<class RequestType>
    auto get() const -> decltype(std::get< type_finder<0, RequestType, Filters...>::type::index >(filter_tuple_))
    {
        return std::get< type_finder<0, RequestType, Filters...>::type::index >(filter_tuple_);
    };

};
#endif