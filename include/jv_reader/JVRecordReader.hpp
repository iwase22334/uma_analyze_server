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
public:
    using result_type = T;

public:
    bool caught;
    std::unique_ptr<T> result_ptr;
    
public:
    JVRecordFilter() : caught(false), result_ptr(new T()) {};
    JVRecordFilter(JVRecordFilter&& rf) noexcept
        : caught(std::move(rf.caught)), result_ptr(std::move(rf.result_ptr)){};

    bool read(const std::string str) {

        if (str.c_str()[0] == C1 && str.c_str()[1] == C2) {
            assert(sizeof(T) == str.size());
            result_ptr.reset(new T());
            std::memcpy(result_ptr.get(), str.c_str(), sizeof(T));
            return (caught = true);
        }
        
        return false;
    };

    void reset() { 
        caught = false;
        result_ptr.reset(new T());
    };
};

template<class... Filters>
class JVFilterArray {
private:
    static constexpr std::size_t filter_size_ = sizeof...(Filters);
    std::array<bool, filter_size_> caught_;
    std::tuple<Filters...> filter_tuple_;

public:
    static constexpr std::size_t filter_size = filter_size_;

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
        bool operator()(std::array<bool, filter_size_>& res,
                        std::tuple<Filters...>& fs,
                        const std::string& str) noexcept(false)
        {
            bool rval = false;

            if (!std::get<Head::val>(fs).read(str))
                rval |= string_fowarder<Tail...>{}(res, fs, str);

            else {
                if(res[Head::val]) throw std::runtime_error("Filter value overwrited."); // filter value is replaced by new one
                res[Head::val] = true;
                rval = true;
            }

            return rval;
        };
    };

    template <class... Tail>
    struct string_fowarder< val_helper<filter_size_ - 1>, Tail...>
    {
        bool operator()(std::array<bool, filter_size_>& res,
                        std::tuple<Filters...>& fs,
                        const std::string& str)
        {
            bool rval = std::get<filter_size_ - 1>(fs).read(str);

            if (rval) {
                if(res[filter_size_ - 1]) throw std::runtime_error("Filter value overwrited.");; // filter value is replaced by new one
                res[filter_size_ - 1] = true;
            }

            return rval; 
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
    bool filter_all_(const std::string& str, const seq<Head, Tail...>&& dummy) noexcept(false)
    {
        // filter all
        return string_fowarder<val_helper<Head>, val_helper<Tail>...>{}
                        (
                            this->caught_,
                            this->filter_tuple_,
                            str
                        );
    }

    template<class Head, class... Tail>
    struct resetter
    {
        void operator()(std::tuple<Filters...>& fs)
        {
            std::get<Head::val>(fs).reset();
            resetter<Tail...>{}(fs);
        }
    };

    template<class... Tail>
    struct resetter< val_helper<filter_size_ - 1>, Tail... >
    {
        void operator()(std::tuple<Filters...>& fs)
        {
            std::get<filter_size_ - 1>(fs).reset();
        }
    };

    template<int Head, int... Tail>
    void reset_all_filter_(const seq<Head, Tail...>&& dum)
    {
        resetter< val_helper<Head>, val_helper<Tail>... >{}(filter_tuple_);
    };

public:
    JVFilterArray() : caught_{{false}}, filter_tuple_() 
    {
        static_assert(filter_size_ > 0, "parameter size must greater than 0");
    };

    JVFilterArray(JVFilterArray&& fa) noexcept
        : caught_(std::move(fa.caught_)), filter_tuple_(std::move(fa.filter_tuple_))
    {
        fa.reset();
    };

    /**
     * @brief filter input string
     * 
     */
    bool operator()(const std::string& str) noexcept(false)
    {
        return filter_all_(str, seq_generator<filter_size_>{});
    };

    /**
     * @brief clean all filter
     * 
     */
    void reset() 
    {
        for (auto& a : caught_) a = false;
        reset_all_filter_(seq_generator<filter_size_>{});
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
    
    int remaining() const 
    {
        int res = filter_size_;
        
        for (auto a : caught_) {
            if (a) { -- res; }
        }

        return res;
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