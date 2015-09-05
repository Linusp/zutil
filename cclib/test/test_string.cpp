#include <iostream>
#include "zutil.hpp"

using namespace Z;

void test_to_string()
{
    std::string a = format_to_string("%d_%f_%u", 3, 3.14, -1);

    std::cout << a << '\n';
}


int main(int argc, char *argv[])
{
    test_to_string();
    return 0;
}
