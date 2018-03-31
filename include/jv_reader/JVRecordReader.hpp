#ifndef JV_RECORD_READER_HPP
#define JV_RECORD_READER_HPP

#include <string>
#include <memory>
#include <tuple>
#include <list>
#include <array>
#include <cassert>

#include "JVData_Structure.h"

namespace jvdata {

    #include "impl/JVRecordReader.ipp"

    /**
     * @brief 
     * 
     * @param id1 
     * @param id2 
     * @return true id is same
     * @return false id is not same
     */
    static inline
    bool is_same_id(const _RACE_ID& id1, const _RACE_ID& id2){
        return (std::memcmp(&(id1), &(id2), sizeof(_RACE_ID)) == 0 ? true : false); 
    }

    /**
     * @brief check id
     * 
     * @tparam T1 
     * @tparam T2 
     * @param a 
     * @param b 
     * @return true 
     * @return false 
     */
    template<class T1, class T2>
    auto is_same_id(const T1& a, const T2& b) -> decltype(a.id, b.id, bool())
    {
        static_assert(std::is_same<decltype(a.id), _RACE_ID>::value, "invalid type");
        static_assert(std::is_same<decltype(b.id), _RACE_ID>::value, "invalid type");
        return (std::memcmp(&(a.id), &(b.id), sizeof(_RACE_ID)) == 0 ? true : false);
    };

    // class T1 or T2 has no member named id.
    template<class T1 = void, class T2 = void>
    auto is_same_id(...) -> bool
    {
        static_assert(_false_value<T1>::val, "type has no member named id");
        return false;
    };

    /**
     * @brief Do Data in Filter array contain same id
     * @todo This function does not work
     *  because of the class that is not have the member named id.
     * 
     * @tparam Head 
     * @tparam Tail 
     * @param fa filter array
     * @return true all data in filter array has same id
     * @return false all data in filter array has not same id 
     */
    template<class Head, class... Tail>
    bool is_valid(const JVFilterArray<Head, Tail...>& fa) 
    {
        return _is_valid(fa, _seq_generator<1 + sizeof...(Tail)>{});
    };

    /**
     * @brief record value reader
     * 
     * @tparam T 
     * @tparam C1 
     * @tparam C2 
     */
    template<typename T, char C1, char C2, char C3>
    struct JVRecordFilter {
    public:
        using result_type = T;

    public:
        bool caught;
        std::unique_ptr<T> result_ptr;
        std::list< std::unique_ptr<T> > fallen_list;

    public:
        JVRecordFilter() : caught(false), result_ptr(nullptr), fallen_list() {};
        JVRecordFilter(JVRecordFilter&& rf) noexcept
            : caught(std::move(rf.caught)),
            result_ptr(std::move(rf.result_ptr)), 
            fallen_list(std::move(rf.fallen_list)) 
        {
            rf.reset();
        };

        bool read(const std::string str) {

            if (str.c_str()[0] != C1 || str.c_str()[1] != C2 || str.c_str()[2] != C3) { 
                return false;
            }
            
            assert(sizeof(T) == str.size());
            // if filter already has data, data will fall to list
            if(result_ptr) fallen_list.push_back(std::move(result_ptr));

            // catch inputed data
            result_ptr.reset(new T());
            std::memcpy(result_ptr.get(), str.c_str(), sizeof(T));

            return (caught = true);

        };

        void reset() { 
            caught = false;
            result_ptr.reset(nullptr);
            fallen_list.clear();
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
        using tuple_type = std::tuple<Filters...>;

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
         * @brief Metafunc for fowarding string to next filter
         * 
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
                    res[Head::val] = true;
                    rval = true;
                }

                return rval;
            };
        };

        template <>
        struct string_fowarder< val_helper<filter_size_ - 1> >
        {
            bool operator()(std::array<bool, filter_size_>& res,
                            std::tuple<Filters...>& fs,
                            const std::string& str) noexcept(false)
            {
                bool rval = std::get<filter_size_ - 1>(fs).read(str);

                if (rval) {
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
        bool filter_all_(const std::string& str, const _seq<Head, Tail...>& dummy) noexcept(false)
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

        template<>
        struct resetter< val_helper<filter_size_ - 1> >
        {
            void operator()(std::tuple<Filters...>& fs)
            {
                std::get<filter_size_ - 1>(fs).reset();
            }
        };

        template<int Head, int... Tail>
        void reset_all_filter_(const _seq<Head, Tail...>& dum)
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
            return filter_all_(str, _seq_generator<filter_size_>{});
        };

        /**
         * @brief clean all filter
         * 
         */
        void reset() 
        {
            for (auto& a : caught_) a = false;
            reset_all_filter_(_seq_generator<filter_size_>{});
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

};
#endif