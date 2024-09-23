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