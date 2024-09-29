#include "commands/package/get.hpp"

utils::fn_attempt commands::package::get(const str &git_repo)
{
    if (utils::get_exec("git ls-remote " + git_repo) ==
        "remote: Repository not found.\nfatal: repository '"
        + git_repo + "' not found")
    {
        std::cout << "hello" << std::endl;
    } else std::cout << "goodbyr\n";
}