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

BOOST_AUTO_TEST_SUITE(jv_record_reader_is_same_id)

BOOST_AUTO_TEST_CASE(case1)
{
    using id_type = _RACE_ID;
    BOOST_CHECK(sizeof(id_type) == 16);
}

BOOST_AUTO_TEST_CASE(case2)
{
    using id_type = _RACE_ID;
    id_type id = {
        .Year = {0, 1, 2, 3},
        .MonthDay = {4, 5, 6, 7},
        .JyoCD = {8, 9},
        .Kaiji = {10, 11},
        .Nichiji = {12, 13},
        .RaceNum = {14, 15}
    };

    struct a_type {
        int hoge;
        id_type id;
    };

    struct b_type {
        id_type id;
        double fuge;
    };

    struct invalid_type_for_is_same_id { id_type bad_name; };

    a_type a;
    b_type b;

    // structure that not has a mamber named id is invalid argunemt
    // invalid_type_for_is_same_id c; 
    // BOOST_CHECK(is_same_id(a, c) == false); 

    std::memcpy(&(a.id), &id, 16);
    std::memcpy(&(b.id), &id, 16);
    
    const char comp1[]{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    BOOST_CHECK(is_same_id(a, b));
    BOOST_CHECK(std::memcmp(reinterpret_cast<const void*>(&a.id), &comp1, 16) == 0);

    const char comp2[]{0,1,2,3,4,5,6,7,0,9,10,11,12,13,14,15};
    b.id.JyoCD[0] = {0};
    BOOST_CHECK(!is_same_id(a, b));
    BOOST_CHECK(std::memcmp(reinterpret_cast<const void*>(&b.id), &comp2, 16) == 0);
    a.id.JyoCD[0] = {0};
    BOOST_CHECK(is_same_id(a, b));
}

BOOST_AUTO_TEST_CASE(case3)
{
    using id_type = _RACE_ID;
    id_type id1 = {
        .Year = {0, 1, 2, 3},
        .MonthDay = {4, 5, 6, 7},
        .JyoCD = {8, 9},
        .Kaiji = {10, 11},
        .Nichiji = {12, 13},
        .RaceNum = {14, 15}
    };

    id_type id2 = {
        .Year = {0, 1, 2, 3},
        .MonthDay = {4, 5, 6, 7},
        .JyoCD = {8, 9},
        .Kaiji = {10, 11},
        .Nichiji = {12, 13},
        .RaceNum = {14, 15}
    };

    BOOST_CHECK(is_same_id(id1, id2));
    BOOST_CHECK(id1 == id2);
}

BOOST_AUTO_TEST_CASE(case4)
{
    using id_type = _RACE_ID;
    id_type id1 = {
        .Year = {0, 1, 2, 3},
        .MonthDay = {4, 5, 6, 7},
        .JyoCD = {8, 9},
        .Kaiji = {10, 11},
        .Nichiji = {12, 13},
        .RaceNum = {14, 15}
    };

    id_type id2 = {
        .Year = {0, 0, 0, 0},
        .MonthDay = {4, 5, 6, 7},
        .JyoCD = {8, 9},
        .Kaiji = {10, 11},
        .Nichiji = {12, 13},
        .RaceNum = {14, 15}
    };

    BOOST_CHECK(is_same_id(id1, id2) == false);
    BOOST_CHECK((id1 == id2) == false);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(jv_record_reader_is_valid)

BOOST_AUTO_TEST_CASE(case1)
{
    using id_type = _RACE_ID;
    
    std::string id_data1("0123456789012345abcde");
    std::string id_data2("0000000000000000abcde");

    struct test_struct1{ 
        char sort[2];
        id_type id;
        char a[5];
    };

    struct test_struct2{
        char sort[2];
        id_type id;
        char a[5];
    };

    struct test_struct3{ 
        char sort[2];
        id_type id;
        char a[5];
    };

    typedef JVRecordFilter<test_struct1, 'A', 'B'> filter1;
    typedef JVRecordFilter<test_struct2, 'C', 'D'> filter2;
    typedef JVRecordFilter<test_struct3, 'E', 'F'> filter3;

    JVFilterArray<filter1, filter2, filter3> filter_array;

    // make data
    BOOST_CHECK(filter_array(std::string("AB") + id_data1));
    BOOST_CHECK(filter_array(std::string("CD") + id_data2));
    BOOST_CHECK(filter_array(std::string("EF") + id_data1));

    BOOST_CHECK(is_valid(filter_array) == false); 
}

BOOST_AUTO_TEST_CASE(case2)
{
    using id_type = _RACE_ID;
    
    std::string id_data1("0123456789012345abcde");
    std::string id_data2("0000000000000000abcde");

    struct test_struct1{ 
        char sort[2];
        id_type id;
        char a[5];
    };

    struct test_struct2{
        char sort[2];
        id_type id;
        char a[5];
    };

    struct test_struct3{ 
        char sort[2];
        id_type id;
        char a[5];
    };

    typedef JVRecordFilter<test_struct1, 'A', 'B'> filter1;
    typedef JVRecordFilter<test_struct2, 'C', 'D'> filter2;
    typedef JVRecordFilter<test_struct3, 'E', 'F'> filter3;

    JVFilterArray<filter1, filter2, filter3> filter_array;

    // make data
    BOOST_CHECK(filter_array(std::string("AB") + id_data1));
    BOOST_CHECK(filter_array(std::string("CD") + id_data1));
    BOOST_CHECK(filter_array(std::string("EF") + id_data1));

    BOOST_CHECK(is_valid(filter_array));
}

BOOST_AUTO_TEST_SUITE_END()
