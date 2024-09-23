#include "utils.hpp"

bool str::starts_with(const std::string &prefix) const
{
   if (this->size() < prefix.size())
      return false;
   return this->compare(0, prefix.size(), prefix) == 0;
}

std::string str::cpp_str() const
{
   return *this;
}

str &str::operator=(const str &s)
{
   if (this != &s)
   {
      std::string::operator=(s);
   }
   return *this;
}

void utils::exec(const str &cmd)
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

void utils::tryexec(const str &cmd)
{
   if (std::system(cmd.c_str()) != 0)
   {
      std::cout << "Failed to run " << cmd << "\n";
   }
}

str utils::get_exec(const str &cmd)
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

str utils::read_file(const str &filename)
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

str utils::read_file_newline(const str &filename)
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