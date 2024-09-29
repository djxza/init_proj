
#include "commands/package/register.hpp"

using namespace utils;

void commands::package::reg(const str& project_name) {
    fs::path projects = fs::path(utils::get_arg0().cpp_str())
        .parent_path().parent_path() / "projects";
    // exec("mkdir " + fs::path(projects / project_name.cpp_str()).string());

    fs::copy(fs::path(project_name.cpp_str()), projects);
}