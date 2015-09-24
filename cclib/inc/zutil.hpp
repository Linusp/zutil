#ifndef ZUTIL_H
#define ZUTIL_H

#include <vector>
#include <string>

namespace Z {
//@{
    // memeory
//@}

//@{  string
    /**
     * Convert arbitrary type data to string
     *
     * @param format: format string
     *
     * @return string you want
     */
    std::string format_to_string(const std::string &fmt, ...);


    /**
     * Split string to a vector
     *
     * @param str: string to be splited
     * @param delims: delimiters used to split string, each character
     *               in it is treated as a delimiter.
     *
     * @return a vector you want
     */
    std::vector<std::string> split_to_vector(const std::string &str, const std::string &delims);


    /**
     * Join a sequence of strings
     *
     * @param vec: a sequence of strings to be joined
     * @param joiner: as it says.
     *
     * @return a string you want
     */
    std::string join(const std::vector<std::string> &vec, const std::string &joiner);
//@}
}

#endif /* ZUTIL_H */
