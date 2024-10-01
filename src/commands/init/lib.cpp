#include "commands/init/lib.hpp"

using namespace utils;

void commands::init::lib(const fs::path &dest)
{
    const fs::path main = fs::path("/home/djozoleta/dev/init_proj/");

    str name = dest.filename().string();
    str author = "anonymous";
    str repo = "none";
    str license = "MIT";
    str desc;
    str incdir = "./include";
    str internal_inc;
    str library = name; // Default library name
    str lang = "c";     // Default to C
    str langver = "11"; // Default to C11
    str hext;
    str cext;

    std::ofstream handle;

    // Ask for the language
    std::cout << "Enter library language (C or C++): ";
    std::getline(std::cin, lang);
    std::transform(lang.begin(), lang.end(), lang.begin(), ::tolower);

    // Validate the language input
    if (lang != "c" &&
        lang != "c++" &&
        lang != "cpp")
    {
        std::cerr << "Error: Unsupported language. Only C and C++ are allowed." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set language version defaults
    if (lang == "c++" || lang == "cpp")
    {
        hext = ".hpp";
        cext = ".cpp";
        lang = "c++";
        langver = "17";
    }
    else
    {
        hext = ".h";
        cext = ".c";
    }
    // std::cout << dest.string() << std::endl;

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

    std::cout << "Use include/custom_folder_name? [Y/n] ";
    str in;
    std::getline(std::cin, in); // No std::cin.ignore() here

    if ((in == "n" || in == "N") && !in.empty())
        ;
    else
    {
        std::cout << " |-> Folder name dir [include/custom_folder_name]: ";
        std::getline(std::cin, internal_inc);
        if (incdir.empty())
            incdir = "./include/" + name;
        else
            incdir = "./include/" + internal_inc;
    }

    str project = "{\n"
                  "   \"type\": \"exe\","
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
                            "   \"flags\": \"-I./include\",\n"
                            "   \"link_flags\": \"-I./lib/" +
                  name + "/include" +
                  (internal_inc.empty() ? "/" : internal_inc + "/") +
                  " -L " + "./lib/" + name + "bin" +
                  " -l" + name + "\",\n" +
                  "   \"bin\": \"./bin\",\n"
                  "   \"src\": \"./src\",\n"
                  "   \"projdir\": \"./.proj\",\n"
                  "   \"libdir\": \"lib\",\n"
                  "   \"incdir\": \"" +
                  incdir + "\",\n"
                           "\n"
                           "   \"library\": \"" +
                  name + "\",\n"
                         "\n"
                         "   \"libs\": [\n"
                         "      \n"
                         "   ]\n"
                         "}\n";

    str default_src = "#include \"" +
                      (internal_inc.empty() ? str("") : internal_inc + "/") +
                      name +
                      ".h\"\n"
                      "\n"
                      "void test() {\n"
                      "    printf(\"Hello, World!\\n\");\n"
                      "}\n";

    // Initialize the project directory and files
    exec("mkdir -p " + dest.string());
    std::cout << dest.string() << std::endl;
    exec("rm -rf " + dest.string() + "/*");
    exec("mkdir -p " + dest.string() + "/src");
    exec("mkdir -p " + dest.string() + "/bin");
    exec("mkdir -p " + dest.string() + "/include");
    exec("mkdir -p " + dest.string() + "/.proj");

    if (incdir != "false")
    {
        exec("mkdir -p " + (dest / fs::path(incdir.cpp_str())).string());
    }

    tryexec("git init " + dest.string());

    // Write the handle.json
    handle = std::ofstream(dest / ".proj/handle.json", std::ios::trunc);
    handle << project << std::endl;

    handle = std::ofstream(dest / str("src/" + name + cext).c_str(), std::ios::trunc);
    handle << default_src << std::endl;

    str make = read_file_newline((main / "defaults/lib.mk").string());
    handle = std::ofstream(dest / "makefile", std::ios::trunc);
    handle << make << std::endl;

    str default_header = read_file_newline((main / "defaults" / str("default" + hext)).string());
    handle = std::ofstream(dest / "include" / (internal_inc.empty() ? "." : internal_inc) / str(name + hext), std::ios::trunc);
    handle << default_header;

    // Make sure directories exist
    assert(fs::exists(dest / "src"));
    assert(fs::exists(dest / "bin"));
    assert(fs::exists(dest / "include"));
    assert(fs::exists(dest / ".proj"));

    std::cout << "Project initialized successfully in " << name << "!" << std::endl;
}