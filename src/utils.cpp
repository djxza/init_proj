#include "utils.hpp"

bool str::starts_with(const str &prefix) const
{
   if (this->size() < prefix.size())
      return false;
   return this->compare(0, prefix.size(), prefix) == 0;
}

bool str::ends_with(const str &suffix) const
{
    return size() >= suffix.size() && std::equal(suffix.rbegin(), suffix.rend(), rbegin());
}

// Find the first occurrence of a substring (similar to std::string::find)
usize str::find_first(const str first) const
{
    return this->find(first);  // std::string::find returns the position of the first occurrence or std::string::npos
}

usize str::find_last(const str last) const
{
     return this->rfind(last);  // std::string::rfind returns the position of the last occurrence or std::string::npos
}

std::string str::cpp_str() const
{
   return *this;
}

str str::after(const usize i) const
{
   str ret;

   for(usize x = i; x < this->size(); ++x) {
      ret += (*this)[x];
   }

   return ret;
}

str &str::operator=(const str &s)
{
   if (this != &s)
   {
      std::string::operator=(s);
   }
   return *this;
}

str arg0;

void utils::set_arg0(const str &x)
{
   arg0 = x;
}

const str utils::get_arg0()
{
   return arg0;
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

str utils::trim(const str &x)
{

   auto start = x.find_first_not_of(" \n\r\t");
   auto end = x.find_last_not_of(" \n\r\t");

   return (start == std::string::npos || end == std::string::npos)
              ? ""
              : x.substr(start, end - start + 1);
}

str utils::get_exec(const str &cmd)
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