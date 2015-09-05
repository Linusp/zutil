/**
 * @file   str_util.cpp
 * @author Linusp <linusp1024@gmail.com>
 * @date   Tue Sep  1 11:49:05 2015
 *
 * @brief  util package for string
 *
 */
#include <cstdio>
#include <cstdarg>

#include "zutil.hpp"

/**
 * Convert arbitrary type data to string
 *
 * @param format format string
 *
 * @return string you want
 */
std::string Z::format_to_string(const std::string &fmt, ...)
{
    int size = ((int)fmt.size()) * 2 + 50;
    std::string str;
    va_list ap;
    while (1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}
