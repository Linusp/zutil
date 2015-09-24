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

using std::string;
using std::vector;

/**
 * Convert arbitrary type data to string
 *
 * @param format: format string
 *
 * @return string you want
 */
string Z::format_to_string(const string &fmt, ...)
{
    int size = ((int)fmt.size()) * 2 + 50;
    string str;
    va_list ap;
    while (1) {             // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char *)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) { // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)             // Needed size returned
            size = n + 1;       // For null char
        else
            size *= 2;         // Guess at a larger size (OS specific)
    }
    return str;
}


/**
 * Split string to a vector
 *
 * @param str: string to be splited
 * @param delims: delimiters used to split string, each character
 *               in it is treated as a delimiter.
 *
 * @return
 */
vector<string> Z::split_to_vector(const string &str, const string &delims)
{
    vector<string> res;

    string::size_type prev_pos = 0;
    string::size_type now_pos = str.find_first_of(delims, prev_pos);

    // string::npos is a static constant, so it may be not thread safe
    const string::size_type npos = -1;
    while (now_pos != npos) {
        res.push_back(str.substr(prev_pos, now_pos - prev_pos));

        prev_pos = now_pos + 1;
        now_pos = str.find_first_of(delims, prev_pos);
    }

    // if prev_pos is not the last pos of string
    // add this remain substring to vector
    if (prev_pos < str.size()) {
        res.push_back(str.substr(prev_pos, str.size() - prev_pos));
    }

    return res;
}


/**
 * Join a sequence of strings
 *
 * @param vec: a sequence of strings to be joined
 * @param joiner: as it says.
 *
 * @return a string you want
 */
string Z::join(const vector<string> &vec, const string &joiner)
{
    string res;

    for (int i = 0; i < vec.size(); ++i) {
        if (res.size() > 0) {
            res += joiner;
        }
        res += vec[i];
    }

    return res;
}
