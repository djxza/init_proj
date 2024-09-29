#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <filesystem>
#include <cinttypes>
#include <cassert>

namespace fs = std::filesystem;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using isize = ssize_t;
using usize = size_t;

class str : public std::string
{
public:
    str() : std::string() {}
    str(const std::string &s) : std::string(s) {}
    str(const char *s) : std::string(s) {}

    bool starts_with(const str &prefix) const;
    bool ends_with(const str &suffix) const;
    usize find_first(const str first) const;
    usize find_last(const str last) const;

    std::string cpp_str() const;

    str after(const usize i) const;

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

    str trim(const str& x);
    str get_exec(const str &cmd);
    str read_file(const str &filename);
    str read_file_newline(const str &filename);
}