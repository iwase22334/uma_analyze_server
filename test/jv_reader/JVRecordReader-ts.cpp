#define BOOST_TEST_MAIN
#include <iostream>
#include <jv_reader/JVRecordReader.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/execution_monitor.hpp>

BOOST_AUTO_TEST_SUITE(jv_record_reader_sequens_test)

BOOST_AUTO_TEST_CASE(case1)
{
    struct test_struct{ 
        char a[5];
    };
    typedef JVRecordFilter<test_struct, 'A', 'B'> filter1;
    JVFilterArray<filter1> filter_array;
    filter_array(std::string("ABCDE"));
    BOOST_CHECK_EQUAL(filter_array.is_caught_all(), true);
}

BOOST_AUTO_TEST_CASE(case2)
{
    struct test_struct1{ 
        char a[5];
    };

    struct test_struct2{ 
        char a[10];
    };

    struct test_struct3{ 
        char a[11];
    };

    typedef JVRecordFilter<test_struct1, 'A', 'B'> filter1;
    typedef JVRecordFilter<test_struct2, 'C', 'D'> filter2;
    typedef JVRecordFilter<test_struct3, 'E', 'D'> filter3;

    JVFilterArray<filter1, filter2, filter3> filter_array;

    // first filtering
    BOOST_CHECK_EQUAL(filter_array(std::string("ABCDE")), true);
    BOOST_CHECK_EQUAL(filter_array(std::string("CDEFGHIJKL")), true);
    BOOST_CHECK_EQUAL(filter_array(std::string("CEFGHIJKL")), false);
    BOOST_CHECK_EQUAL(filter_array(std::string("EDEFGHIJKLa")), true);

    // check result
    const filter1& f1 = filter_array.get<filter1>();
    const filter2& f2 = filter_array.get<filter2>();
    const filter3& f3 = filter_array.get<filter3>();

    BOOST_CHECK_EQUAL(std::string(f1.result_ptr->a, 5) == std::string("ABCDE"), true);
    BOOST_CHECK_EQUAL(std::string(f2.result_ptr->a, 10) == std::string("CDEFGHIJKL"), true);
    BOOST_CHECK_EQUAL(std::string(f3.result_ptr->a, 11) == std::string("EDEFGHIJKLa"), true);
    
    BOOST_CHECK_EQUAL(filter_array.is_caught(0), true);
    BOOST_CHECK_EQUAL(filter_array.is_caught(1), true);
    BOOST_CHECK_EQUAL(filter_array.is_caught(2), true);

    BOOST_CHECK_EQUAL(filter_array.is_caught_all(), true);

    // second filtering
    filter_array.reset();

    BOOST_CHECK_EQUAL(filter_array.is_caught(0), false);
    BOOST_CHECK_EQUAL(filter_array.is_caught(1), false);
    BOOST_CHECK_EQUAL(filter_array.is_caught(2), false);

    BOOST_CHECK_EQUAL(filter_array.is_caught_all(), false);

    BOOST_CHECK_EQUAL(filter_array(std::string("CDEFGHIJKL")), true);

    BOOST_CHECK_EQUAL(filter_array.is_caught(0), false);
    BOOST_CHECK_EQUAL(filter_array.is_caught(1), true);
    BOOST_CHECK_EQUAL(filter_array.is_caught(2), false);

    JVFilterArray<filter1, filter2, filter3> filter_array2(std::move(filter_array));

    BOOST_CHECK_EQUAL(filter_array2.is_caught_all(), false); 

    // death test
    BOOST_REQUIRE_THROW(filter_array2(std::string("CDEFGHIJKL")),
                        std::runtime_error);

}

BOOST_AUTO_TEST_SUITE_END()