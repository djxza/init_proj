#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <filesystem>
#include <cassert>

#include "utils.hpp"
#include "commands/init/exe.hpp"
#include "commands/init/lib.hpp"
#include "commands/package/get.hpp"

using namespace commands;

void print_usage()
{
   std::cout << "Usage: init_proj command [command_args...] [flags..]\n";
}

int main(int ac, const char *argv[])
{
   std::vector<str> args(argv + 1, argv + ac); // Skip the program name (argv[0])
   std::vector<str> command_args;
   std::string command;

   utils::set_arg0(argv[0]);

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
         init::exe(fs::path(command_args[1].cpp_str()));
      else if (command_args[0] == "lib")
         init::lib(fs::path(command_args[1].cpp_str()));
      else
         std::cout << "Usage: " << argv[0] << " init exe name or " << argv[0] << " init lib name\n";
   }
   else if (command == "get")
   {
      package::get(command_args[1]);
   }

   return 0;
}
