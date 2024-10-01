#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <exception>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

std::string get_exec(const std::string &cmd)
{
    std::array<char, 128> buffer; // Initialize a buffer with a fixed size
    std::string result;

    // Redirect stderr to stdout by appending '2>&1' to the command
    std::string full_cmd = cmd + " 2>&1";

    // Open a pipe to run the command and get the output
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(full_cmd.c_str(), "r"), pclose);

    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    // Read the command's output from the pipe
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}

int main(int ac, const char *argv[])
{
    // Check for valid argument count
    if (ac < 1)
    {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }

    const std::string interpreter = fs::path(fs::path(argv[0])
                                                 .parent_path() /
                                             "proj_interpreter")
                                        .string();

    std::string libs_str = get_exec(interpreter + " .proj/handle.json libs");
    std::vector<std::string> libs;

    // Remove unwanted characters at the start and end (adjust if needed)
    if (libs_str.size() > 2)
    {
        libs_str.erase(libs_str.begin()); // Erase the [
        libs_str.erase(libs_str.begin()); // Erase the \n
        libs_str.pop_back();              // Erase the \n
        libs_str.pop_back();              // Erase the ]
        libs_str.pop_back();              // Erase the \n
    }

    std::string lib_name;

    // Split the string by commas
    for (size_t i = 0; i < libs_str.size(); ++i)
    {
        if ((libs_str[i] == ' '))
        {
        }
        else
        {
            if (libs_str[i] != ',')
            {
                lib_name.push_back(libs_str[i]);
            }
            else
            {
                if (!lib_name.empty())
                {
                    libs.push_back(lib_name);
                    lib_name.clear();

                    ++i;
                }
            }
        }
    }

    // Add the last remaining lib_name (if any)
    if (!lib_name.empty())
    {
        libs.push_back(lib_name);
    }

    std::cout << "libraries are: " << std::endl;

        std::string flags;

    // clear the quotations
    for (auto &e : libs)
    {
        e.erase(e.begin());
        e.pop_back();

            std::string current_lib_flags = get_exec(interpreter
        + " ./libs/" + e + " link_flags");

        //remove quotations 
        current_lib_flags.erase(current_lib_flags.begin());
        current_lib_flags.pop_back();

        flags += current_lib_flags + " ";

        current_lib_flags.clear();
        
        // std::cout << '\t' << e << '\n';
    }

    std::cout << flags << std::endl;

    return 0;
}
