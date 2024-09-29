#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <filesystem>
#include <cassert>

namespace fs = std::filesystem;

class str : public std::string
{
public:
    str() : std::string() {}
    str(const std::string &s) : std::string(s) {}
    str(const char *s) : std::string(s) {}

    bool starts_with(const std::string &prefix) const;
    std::string cpp_str() const;

    str &operator=(const str &s);
};

/* added namespace to make managment easier */
namespace utils
{
    enum class fn_attempt {
        SUCCESS,
        FAILURE
    };

    /* relative executable bath thing */
    void set_arg0(const str& arg0);
    const str get_arg0();

    void exec(const str &cmd);
    void tryexec(const str &cmd);

    str get_exec(const str &cmd);
    str read_file(const str &filename);
    str read_file_newline(const str &filename);
}