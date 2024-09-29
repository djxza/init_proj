#include "commands/package/update.hpp"

void commands::package::update(const str &project_name)
{
    fs::path path_in_projs = fs::path(utils::get_arg0().cpp_str())
        .parent_path().parent_path() / "projects" / project_name.cpp_str();

    if (fs::exists(path_in_projs))
    {
        commands::package::reg(project_name);
    }
    else
        std::cout << "No such project " << project_name << ", try package register first.";
}