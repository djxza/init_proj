#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <filesystem>
#include <cassert>

// #define _Debug

namespace fs = std::filesystem;

class str : public std::string
{
public:
   str() : std::string() {}
   str(const std::string &s) : std::string(s) {}
   str(const char *s) : std::string(s) {}

   bool starts_with(const std::string &prefix) const
   {
      if (this->size() < prefix.size())
         return false;
      return this->compare(0, prefix.size(), prefix) == 0;
   }

   std::string cppstr() const
   {
      return *this;
   }

   str &operator=(const str &s)
   {
      if (this != &s)
      {
         std::string::operator=(s);
      }
      return *this;
   }
};

enum class project_types
{
   EXE,
   LIB
};

class init_t
{
private:
   project_types type;

public:
   init_t(const str &tp_input)
   {
      str tp = tp_input; // Make a non-const copy for modification

      // Convert the string to lowercase
      std::transform(tp.begin(), tp.end(), tp.begin(),
                     [](unsigned char c)
                     { return std::tolower(c); });

      if (tp == "exe" || tp == "bin")
      {
         this->type = project_types::EXE;
      }
      else
      {
         this->type = project_types::LIB;
      }
   }
};

void print_usage()
{
   std::cout << "Usage: init_proj command [command_args...] [flags..]\n";
}

void exec(const str &cmd)
{
#ifdef _Debug
   std::cout << "Command "
             << cmd
             << " executed with exit code "
             <<
#endif // _Debug

       std::system(cmd.c_str())

#ifdef _Debug
             << "\n"
#endif // _Debug
       ;
}

str get_exec(const str &cmd)
{
   std::vector<char> buffer;
   std::string result;
   // Open a pipe to run the command and get the output
   std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
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

str read_file(const str &filename)
{
   str out;
   std::ifstream file(filename); // Open the file

   // Check if the file was successfully opened
   if (!file.is_open())
   {
      std::cerr << "Error: Could not open the file " << filename << std::endl;
      exit(EXIT_FAILURE);
   }

   str line;
   // Read the file line by line
   while (std::getline(file, line))
   {
      out += line;
   }

   file.close(); // Close the file

   return out;
}

str read_file_newline(const str &filename)
{
   str out;
   std::ifstream file(filename); // Open the file

   // Check if the file was successfully opened
   if (!file.is_open())
   {
      std::cerr << "Error: Could not open the file " << filename << std::endl;
      exit(EXIT_FAILURE);
   }

   str line;
   // Read the file line by line
   while (std::getline(file, line))
   {
      out += line + "\n";
   }

   file.close(); // Close the file

   return out;
}

void tryexec(const str &cmd)
{
   if (std::system(cmd.c_str()) != 0)
   {
      std::cout << "Failed to run " << cmd << "\n";
   }
}

void init_exec(const str &project_name)
{
   const fs::path dest(project_name.cppstr());
   const fs::path main = fs::path("/home/djozoleta/dev/init_proj/");

   str name = project_name;
   str author = "anonymous";
   str repo = "none";
   str license = "MIT";
   str desc;
   str incdir = "false";
   str executable = project_name; // Default executable name
   str lang = "c";                // Default to C
   str langver = "11";            // Default to C11

   // Ask for the language
   std::cout << "Enter project language (C or C++): ";
   std::getline(std::cin, lang);
   std::transform(lang.begin(), lang.end(), lang.begin(), ::tolower);

   // Validate the language input
   if (lang != "c" && lang != "c++")
   {
      std::cerr << "Error: Unsupported language. Only C and C++ are allowed." << std::endl;
      exit(EXIT_FAILURE);
   }

   // Set language version defaults
   if (lang == "c++")
      langver = "17";

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

   std::cout << "Executable name [" << executable << "]: ";
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
   exec("mkdir -p " + project_name);
   exec("rm -rf " + project_name + "/*");
   exec("mkdir -p " + project_name + "/src " + project_name + "/bin " + project_name + "/.proj");

   if (incdir != "false")
   {
      exec("mkdir -p " + (dest / fs::path(incdir.cppstr())).string());
   }

   tryexec("git init " + project_name);

   // Write the handle.json
   std::ofstream handle(dest / ".proj/handle.json", std::ios::trunc);
   handle << project << std::endl;
   str make = read_file_newline((main / "defaults" / "makefile").string());
   handle = std::ofstream(dest / "makefile", std::ios::trunc);
   handle << make << std::endl;

   // Make sure directories exist
   assert(fs::exists(dest / "src"));
   assert(fs::exists(dest / "bin"));
   assert(fs::exists(dest / ".proj"));

   std::cout << "Project initialized successfully in " << project_name << "!" << std::endl;
}

void init_lib(const str &project_name)
{
}

int main(int ac, const char *argv[])
{
   std::vector<str> args(argv + 1, argv + ac); // Skip the program name (argv[0])
   std::vector<str> command_args;
   std::string command;

   bool need_command = true;

   for (const auto &e : args)
   {
      if (!e.starts_with("-"))
      {
         if (need_command)
         {
            need_command = false;
            command = e;
         }
         else
         {
            command_args.push_back(e);
         }
      }
   }

   if (command == "init")
   {
      if (command_args.empty())
      {
         print_usage();
         return EXIT_FAILURE;
      }
      else if (command_args[0] == "exe")
         init_exec(command_args[1]);
      else if (command_args[0] == "lib")
         init_lib(command_args[1]);
      else
         std::cout << "Usage: " << argv[0] << " init exe name or " << argv[0] << " init lib name\n";
   }

   return 0;
}
