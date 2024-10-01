#include "commands/package/get.hpp"

utils::fn_attempt commands::package::get(const str &git_repo)
{
    /*std::cout << git_repo << "starte\n";
    std::cout << str("git ls-remote ") + git_repo << "\n";

    std::cout << std::system(str("git ls-remote " + git_repo).c_str());
    std::cout << "funciton started\n";*/
    /*std::cout << git_repo << '\n';
    std::cout << str("remote: Repository not found.\nfatal: repository 'https://github.com/" + git_repo + "/' not found");
    std::cout << "\n\n\n\n\n";
    std::ofstream git("./test/git.output");
    std::ofstream my("./test/my.output");
   //std::cout << utils::get_exec("git ls-remote https://github.com/" + git_repo);*/

    const str interpreter_location = fs::path(fs::path(utils::get_arg0().cpp_str()).parent_path() / "proj_interpreter").string();

    const str lib = utils::get_exec(interpreter_location
        + " .proj/handle.json libdir").remove_char('\n');

    const str output = utils::get_exec
        ("git ls-remote https://github.com/" + git_repo);
    const str failure = "remote: Repository not found.\nfatal: repository 'https://github.com/"
        + git_repo + "/' not found";

    if (utils::trim(output) == utils::trim(failure))
    {
        std::cout << "No such git repo 'https://github.com/"
            << git_repo << "'\n";

        return utils::fn_attempt::FAILURE;
    }
    else
    {

        if (fs::exists(fs::path(".proj/handle.json")))
        {
            std::cout << "Fetching from '" << git_repo << "'\n";
            const str repo_name = git_repo.after(git_repo.find_last("/"));
            //std::cout << "git submodule add https://github.com/" + git_repo + " " + lib + "/" + repo_name + "\n";

            //std::cout << repo_name << "\n";

            utils::exec("mkdir -p " + lib);

            if (!fs::exists(fs::path(".gitmodules")))
            {
                std::cout << "Adding .gitmodules" << std::endl;
                utils::exec("touch .gitmodules");
            }

            utils::exec("git submodule add https://github.com/"
                + git_repo + " " + lib + "/" + repo_name);

            if(utils::get_exec(interpreter_location + " " + lib +
                "/" + repo_name + "/.proj/handle.json type") != "lib") {
                    std::cout << "Project " << git_repo << " not valid!\n";

                    utils::exec("rm -rf " + lib + "/" + repo_name);
                }

            utils::add_to_array("./.proj/handle.json", "libs", repo_name);
        }
    }

    return utils::fn_attempt::SUCCESS;
}