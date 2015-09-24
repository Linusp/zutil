#include <iostream>
#include <vector>
#include <string>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "zutil.hpp"

using namespace Z;
using std::vector;
using std::string;


TEST_GROUP(string_test_group)
{
    void setup()
    {
        //
    }

    void teardonw()
    {
        //
    }
};


TEST(string_test_group, format_ok)
{
    CHECK_EQUAL(format_to_string("%d_%.03f_%c", 3, 3.14, 'a'), "3_3.140_a");
}


TEST(string_test_group, split_ok)
{
    CHECK_EQUAL(split_to_vector("abc.edf/sss.", "./").size(), 3);
}


TEST(string_test_group, join_ok)
{
    vector<string> vec;
    vec.push_back("a");
    vec.push_back("b");
    vec.push_back("c");

    CHECK_EQUAL(join(vec, "."), "a.b.c");
}


int main(int argc, char *argv[])
{
    return RUN_ALL_TESTS(argc, argv);
}
