#include "utils.hpp"

void str::operator-(const usize i)
{
   (*this).resize(this->size() - i);
}

void str::operator+(const usize i)
{
   (*this).resize(this->size() + i);
}

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
   return this->find(first); // std::string::find returns the position of the first occurrence or std::string::npos
}

usize str::find_last(const str last) const
{
   return this->rfind(last); // std::string::rfind returns the position of the last occurrence or std::string::npos
}

std::string str::cpp_str() const
{
   return *this;
}

str str::after(const usize i) const
{
   if (i + 1 >= this->size())
   {
      return str(); // If 'i' is out of bounds, return an empty string.
   }

   return substr(i + 1); // Use the built-in std::string::substr to simplify this.
}

str str::remove_char(char c) const
{
   str result;

   // Iterate through the string and only append characters that are not 'c'
   for (char ch : *this)
   {
      if (ch != c)
      {
         result += ch;
      }
   }

   return result;
}

str str::no_newline() const
{
   return (*this);
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

void utils::add_to_array(const str& filename, const str &key, const json &element)
{
    // Open the file and read the JSON content
    std::ifstream input_file(filename);
    json j;

    if (input_file.is_open()) {
        input_file >> j;
        input_file.close();
    } else {
        std::cerr << "Could not open the file for reading: " << filename << std::endl;
        return;
    }

    // Check if the key exists and if it's an array
    if (j.contains(key) && j[key].is_array()) {
        j[key].push_back(element);
    } else {
        // If the key doesn't exist or isn't an array, create a new array with the element
        j[key] = json::array();
        j[key].push_back(element);
    }

    // Open the file and write the updated JSON content
    std::ofstream output_file(filename);
    if (output_file.is_open()) {
        output_file << j.dump(4);  // Write pretty-printed JSON with 4-space indentation
        output_file.close();
    } else {
        std::cerr << "Could not open the file for writing: " << filename << std::endl;
    }
}

void utils::exec(const str &cmd)
{
#ifdef _Debug
   std::cout << "Command "
             << cmd
             << " executed with exit code "
             <<
#endif // _Debug

      (void) std::system(cmd.c_str())
 
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

utils::fn_attempt utils::int_exec(const str &cmd)
{
   return static_cast<utils::fn_attempt>
      (!std::system(cmd.c_str()));
}

void utils::msg_tryexec(const str &cmd, const str &msg)
{
   if (std::system(cmd.c_str()) != 0)
   {
      std::cout << msg;
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