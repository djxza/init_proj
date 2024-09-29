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

   std::cout << "RADI DO OVDE\n";

   bool need_command = true;

   for (const auto &e : args)
   {
      if (!e.starts_with(str("-")))
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

   std::cout << "RADI DO OVDE\n";

   if (command == "init")
   {
      if (command_args.empty())
      {
         print_usage();
         return EXIT_FAILURE;
      }
      else if (command_args[0] == "exe")
      {
         if (command_args.size() < 2)
         {
            std::cerr << "Error: Missing argument for 'init exe'.\n";
            return EXIT_FAILURE;
         }
         init::exe(fs::path(command_args[1].cpp_str()));
      }
      else if (command_args[0] == "lib")
      {
         if (command_args.size() < 2)
         {
            std::cerr << "Error: Missing argument for 'init lib'.\n";
            return EXIT_FAILURE;
         }
         init::lib(fs::path(command_args[1].cpp_str()));
      }
      else
      {
         std::cout << "Usage: " << argv[0] << " init exe name or " << argv[0] << " init lib name\n";
      }
   }
   else if (command == "get")
   {

      if (command_args.size() < 1)
      {
         std::cerr << "Error: Missing argument for 'get'.\n";
         return EXIT_FAILURE;
      }

      package::get(command_args[0]);
   }

   std::cout << "RADI DO OVDE\n";

   return 0;
}
