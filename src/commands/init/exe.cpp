#include "commands/init/exe.hpp"

using namespace utils;

void init_exe(const fs::path &dest)
{
    /* Ovde za sada */
    const fs::path main = fs::path("/home/djozoleta/dev/init_proj/");

    str name = dest.filename().string();
    str author = "anonymous";
    str repo = "none";
    str license = "MIT";
    str desc;
    str incdir = "false";
    str executable = name; // Default executable name
    str lang = "c";        // Default to C
    str langver = "11";    // Default to C11
    str ext;
    std::ofstream handle;

    // Ask for the language
    std::cout << "Enter executable language (C or C++): ";
    std::getline(std::cin, lang);
    std::transform(lang.begin(), lang.end(), lang.begin(), ::tolower);

    if (lang != "c" &&
        lang != "C" &&
        lang != "c++" &&
        lang != "C++" &&
        lang != "cpp" &&
        lang != "CPP")
    {
        std::cerr << "Error: Unsupported language. Only C and C++ are allowed." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set language version defaults
    if (lang == "c++" || lang == "cpp")
    {
        ext = ".cpp";
        lang = "c++";
        langver = "17";
    }
    else
        ext = ".c";

    std::cout << "Author [" << author << "]: ";
    std::getline(std::cin, author);
    if (author.empty())
        author = "anonymous";

    std::cout << "Repo [" << repo << "]: ";
    std::getline(std::cin, repo);
    if (repo.empty())
        repo = "none";

    std::cout << "License [" << license << "]: ";
    std::getline(std::cin, license);
    if (license.empty())
        license = "MIT";

    std::cout << "Description: ";
    std::getline(std::cin, desc);

    std::cout << "Use Include dir? [Y/n] ";
    str in;
    std::getline(std::cin, in); // No std::cin.ignore() here

    if ((in == "n" || in == "N") && !in.empty())
    {
        incdir = "false";
    }
    else
    {
        std::cout << " |-> Include dir [./include]: ";
        std::getline(std::cin, incdir);
        if (incdir.empty())
            incdir = "./include";
    }

    std::cout << "Library name [" << executable << "]: ";
    std::getline(std::cin, executable);
    if (executable.empty())
        executable = name;

    // JSON template updated with language
    str project = "{\n"
                  "   \"name\": \"" +
                  name + "\",\n"
                         "   \"author\": \"" +
                  author + "\",\n"
                           "   \"version\": 0,\n"
                           "   \"repo\": \"" +
                  repo + "\",\n"
                         "   \"license\": \"" +
                  license + "\",\n"
                            "   \"description\": \"" +
                  desc + "\",\n"
                         "\n"
                         "   \"CC\": \"gcc\",\n"
                         "   \"lang\": \"" +
                  lang + "\",\n"
                         "   \"langver\": \"" +
                  langver + "\",\n"
                            "   \"gnu_ext\": false,\n"
                            "   \"flags\": \"\",\n"
                            "   \"bin\": \"./bin\",\n"
                            "   \"src\": \"./src\",\n"
                            "   \"projdir\": \"./.proj\",\n"
                            "   \"libdir\": \"lib\",\n"
                            "   \"incdir\": \"" +
                  incdir + "\",\n"
                           "\n"
                           "   \"executable\": \"" +
                  executable + "\",\n"
                               "\n"
                               "   \"libs\": [\n"
                               "      \n"
                               "   ]\n"
                               "}\n";

    // Initialize the project directory and files
    exec("mkdir -p " + dest.string());
    exec("rm -rf " + dest.string() + "/*");
    exec("mkdir -p " + dest.string() + "/src");
    exec("mkdir -p " + dest.string() + "/bin");
    exec("mkdir -p " + dest.string() + "/.proj");

    if (incdir != "false")
    {
        exec("mkdir -p " + (dest / fs::path(incdir.cpp_str())).string());
    }

    tryexec("git init " + name);

    // Write the handle.json
    handle = std::ofstream(dest / ".proj/handle.json", std::ios::trunc);
    handle << project << std::endl;

    str make = read_file_newline((main / "defaults/exe.mk").string());
    handle = std::ofstream(dest / "makefile", std::ios::trunc);
    handle << make << std::endl;

    str start = read_file_newline((main / "defaults" / str("main" + ext)).string());
    handle = std::ofstream(dest / "src" / str("main" + ext), std::ios::trunc);
    handle << start;

    // Make sure directories exist
    assert(fs::exists(dest / "src"));
    assert(fs::exists(dest / "bin"));
    assert(fs::exists(dest / ".proj"));

    std::cout << "Project initialized successfully in " << name << "!" << std::endl;
}