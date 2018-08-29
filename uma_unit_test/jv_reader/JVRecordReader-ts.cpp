#include <boost/test/unit_test.hpp>

#include <jv_reader/JVDataConstants.hpp>
#include <jv_reader/JVDataHandling.hpp>
#include <jv_reader/JVRecordReader.hpp>

#include <string>

static const std::string id_dummy_str = "0123456789012345";

BOOST_AUTO_TEST_SUITE(jv_record_reader_sequens_test)

BOOST_AUTO_TEST_CASE(jv_record_reader_sequens_test_case1)
{	
    jvdata::id_type id1 = {
		/*.Year =*/ {0, 1, 2, 3},
        /*.MonthDay =*/ {4, 5, 6, 7},
        /*.JyoCD =*/ {8, 9},
        /*.Kaiji =*/ {10, 11},
        /*.Nichiji =*/ {12, 13},
        /*.RaceNum =*/ {14, 15}
    };

    jvdata::id_type id2 = {
		/*.Year =*/ {0, 1, 2, 3},
		/*.MonthDay =*/ {4, 5, 6, 7},
		/*.JyoCD =*/ {8, 9},
		/*.Kaiji =*/ {10, 11},
		/*.Nichiji =*/ {12, 13},
		/*.RaceNum =*/ {14, 15}
    };

    struct test_struct{ 
        char a[5];
		jvdata::id_type id;
    };

    typedef jvdata::JVRecordFilter<test_struct, 'A', 'B', 'C'> filter1;
    jvdata::JVFilterArray<filter1> filter_array;
    filter_array(std::string("ABCDE") + id_dummy_str);
    BOOST_CHECK_EQUAL(filter_array.is_caught_all(), true);
}

BOOST_AUTO_TEST_CASE(jv_record_reader_sequens_test_case2)
{
    struct test_struct1{ 
        char a[5];
		jvdata::id_type id;
    };

    struct test_struct2{ 
        char a[10];
		jvdata::id_type id;
    };

    struct test_struct3{ 
        char a[11];
		jvdata::id_type id;
    };
	
    using filter1 = jvdata::JVRecordFilter<test_struct1, 'A', 'B', 'C'>;
    using filter2 = jvdata::JVRecordFilter<test_struct2, 'C', 'D', 'E'>;
    using filter3 = jvdata::JVRecordFilter<test_struct3, 'E', 'D', 'E'>;

    jvdata::JVFilterArray<filter1, filter2, filter3> filter_array;

    // first filtering
    BOOST_CHECK_EQUAL(filter_array(std::string("ABCDE") + id_dummy_str) == boost::none, false);
	BOOST_CHECK_EQUAL(filter_array(std::string("CDEFGHIJKL") + id_dummy_str) == boost::none, false);
    BOOST_CHECK_EQUAL(filter_array(std::string("CEFGHIJKL") + id_dummy_str) == boost::none, true);
    BOOST_CHECK_EQUAL(filter_array(std::string("EDEFGHIJKLa") + id_dummy_str) == boost::none, false);

    // check result
    const filter1& f1 = filter_array.get<filter1>();
    const filter2& f2 = filter_array.get<filter2>();
    const filter3& f3 = filter_array.get<filter3>();

    BOOST_CHECK_EQUAL(std::string(f1.get().front()->a, 5) == std::string("ABCDE"), true);
    BOOST_CHECK_EQUAL(std::string(f2.get().front()->a, 10) == std::string("CDEFGHIJKL"), true);
    BOOST_CHECK_EQUAL(std::string(f3.get().front()->a, 11) == std::string("EDEFGHIJKLa"), true);
    
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

    BOOST_CHECK_EQUAL(filter_array(std::string("CDEFGHIJKL") + id_dummy_str) == boost::none, false);

    BOOST_CHECK_EQUAL(filter_array.is_caught(0), false);
    BOOST_CHECK_EQUAL(filter_array.is_caught(1), true);
    BOOST_CHECK_EQUAL(filter_array.is_caught(2), false);

    jvdata::JVFilterArray<filter1, filter2, filter3> filter_array2(std::move(filter_array));

    BOOST_CHECK_EQUAL(filter_array2.is_caught_all(), false); 

    // death test
    BOOST_CHECK_EQUAL(filter_array2(std::string("CDEFGHIJKL") + id_dummy_str) == boost::none, false);
    BOOST_CHECK_EQUAL(filter_array2.get<filter2>().fallen_list.size(), 2);
    
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(jv_record_reader_is_same_id)

BOOST_AUTO_TEST_CASE(is_same_id_case1)
{
    BOOST_CHECK(sizeof(jvdata::id_type) == 16);
}

BOOST_AUTO_TEST_CASE(is_same_id_case2)
{
    using jvdata::is_same_id;
    using id_type = jvdata::id_type;
    id_type id = {
        /*.Year =*/ {0, 1, 2, 3},
		/*.MonthDay =*/ {4, 5, 6, 7},
        /*.JyoCD =*/ {8, 9},
        /*.Kaiji =*/ {10, 11},
        /*.Nichiji =*/ {12, 13},
        /*.RaceNum =*/ {14, 15}
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

BOOST_AUTO_TEST_CASE(is_same_id_case3)
{
    using jvdata::is_same_id;
	using jvdata::operator==;
	
    jvdata::id_type id1 = {
		/*.Year =*/ {0, 1, 2, 3},
        /*.MonthDay =*/ {4, 5, 6, 7},
        /*.JyoCD =*/ {8, 9},
        /*.Kaiji =*/ {10, 11},
        /*.Nichiji =*/ {12, 13},
        /*.RaceNum =*/ {14, 15}
    };

    jvdata::id_type id2 = {
        /*.Year =*/{ 0, 1, 2, 3 },
        /*.MonthDay =*/{ 4, 5, 6, 7 },
        /*.JyoCD =*/{ 8, 9 },
        /*.Kaiji =*/{ 10, 11 },
        /*.Nichiji =*/{ 12, 13 },
        /*.RaceNum =*/{ 14, 15 }
    };

    BOOST_CHECK(is_same_id(id1, id2));
    BOOST_CHECK(id1 == id2);
}

BOOST_AUTO_TEST_CASE(is_same_id_case4)
{
    using jvdata::is_same_id;
	using jvdata::operator==;

    jvdata::id_type id1 = {
        /*.Year =*/{ 0, 1, 2, 3 },
        /*.MonthDay =*/{ 4, 5, 6, 7 },
        /*.JyoCD =*/{ 8, 9 },
        /*.Kaiji =*/{ 10, 11 },
        /*.Nichiji =*/{ 12, 13 },
        /*.RaceNum =*/{ 14, 15 }
    };

    jvdata::id_type id2 = {
        /*.Year =*/{ 0, 0, 0, 0 },
        /*.MonthDay =*/{ 4, 5, 6, 7 },
        /*.JyoCD =*/{ 8, 9 },
        /*.Kaiji =*/{ 10, 11 },
        /*.Nichiji =*/{ 12, 13 },
        /*.RaceNum =*/{ 14, 15 }
    };

    BOOST_CHECK(is_same_id(id1, id2) == false);
    BOOST_CHECK((id1 == id2) == false);
}

BOOST_AUTO_TEST_SUITE_END()
