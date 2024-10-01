#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

void quit() {
   exit(EXIT_FAILURE);
}

// Function to access nested JSON fields
json get_json_field(const json& j, const std::vector<std::string>& keys) {
   json temp = j;
   for (const auto& key : keys) {
      if (temp.contains(key)) {
         temp = temp[key];
      } else {
         std::cerr << "Key '" << key << "' not found.\n";
         quit();
      }
   }
   return temp;
}

int main(int ac, const char** argv) {

#ifdef _DEBUG
   std::cout << ac << std::endl;
#endif // _DEBUG

   // Ensure there are enough arguments
   if (ac < 3) {
      std::cerr << "Usage: " << argv[0] << " <file> <field1> [<field2> ...]\n";
      quit();
   }

   // Open the JSON file
   std::ifstream read(argv[1]);
   if (!read.good()) {
      std::cerr << " Failed to read " << argv[1] << ".\n";
      quit();
   }

   // Parse the JSON
   json j;
   read >> j;

   // Extract field keys from the arguments
   std::vector<std::string> keys;
   for (int i = 2; i < ac; ++i) {
      keys.emplace_back(argv[i]);
   }

   // Retrieve the nested field
   json result = get_json_field(j, keys);

   // Print the result
   if (result.is_string()) {
      std::cout << result.get<std::string>() << std::endl; // Print string without quotes
   } else {
      std::cout << result.dump(4) << std::endl; // Pretty print for non-string values
   }

   return EXIT_SUCCESS;
}
