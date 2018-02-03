#define BOOST_TEST_MAIN
#include <iostream>
#include <jv_reader/JVDataPool.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(jv_data_pool_sequens_test)

BOOST_AUTO_TEST_CASE(case1)
{
    struct test_struct1 { 
        char a[5];
    };

    struct test_struct2 { 
        char a[10];
    };

    struct test_struct3 { 
        char a[11];
    };

    using filter1 = JVRecordFilter<test_struct1, 'A', 'B'>;
    using filter2 = JVRecordFilter<test_struct2, 'C', 'D'>;
    using filter3 = JVRecordFilter<test_struct3, 'E', 'F'>;
    using filter_array = JVFilterArray<filter1, filter2, filter3>;

    // test target
    using filter_pool = jv_data::JVDataPool<filter_array>;
    using result_type = filter_pool::FilterResult;
    filter_pool test_pool;

    // first filtering
    BOOST_CHECK(test_pool(std::string("ABCDE")) == result_type::E_Caught);
    BOOST_CHECK(test_pool(std::string("CDEFGHIJKL")) == result_type::E_Caught);
    BOOST_CHECK(test_pool(std::string("EEFGHIJKL")) == result_type::E_Pass);
    BOOST_CHECK(test_pool.get_data_size() == 0);
    BOOST_CHECK(test_pool(std::string("EFEFGHIJKLa")) == result_type::E_Complete);
    BOOST_CHECK(test_pool.get_data_size() == 1);

    BOOST_CHECK(test_pool(std::string("EFEFGHIJKLa")) == result_type::E_Caught);
    BOOST_CHECK(test_pool(std::string("EEFGHIJKL")) == result_type::E_Pass);
    BOOST_CHECK(test_pool(std::string("ABCDE")) == result_type::E_Caught);
    BOOST_CHECK(test_pool(std::string("CDEFGHIJKL")) == result_type::E_Complete);
    BOOST_CHECK(test_pool.get_data_size() == 2);

    for (auto a = test_pool.get_data_list().begin(); a != test_pool.get_data_list().end(); ++ a) {
        BOOST_CHECK(std::string(a->get<filter1>().result_ptr->a, 5) == "ABCDE");
        BOOST_CHECK(std::string(a->get<filter2>().result_ptr->a, 10) == "CDEFGHIJKL");
        BOOST_CHECK(std::string(a->get<filter3>().result_ptr->a, 11) == "EFEFGHIJKLa");
    }

    test_pool.reset();
    BOOST_CHECK(test_pool.get_data_size() == 0);

    // death test
    BOOST_CHECK(test_pool(std::string("CDEFGHIJKL")) == result_type::E_Caught);
    BOOST_REQUIRE_THROW(test_pool(std::string("CDEFGHIJKL")), std::runtime_error);

}

BOOST_AUTO_TEST_SUITE_END()