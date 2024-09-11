#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>

// Function to check if a file exists
bool file_exists(const std::string& file_path) {
    struct stat buffer;
    return (stat(file_path.c_str(), &buffer) == 0);
}

// Function to transform a file's contents into a C++ string with escape sequences
std::string transform_to_cpp_string(const std::string& input) {
    std::ifstream file(input);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << input << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << "\""; // Start the C++ string

    std::string line;
    while (std::getline(file, line)) {
        for (char c : line) {
            if (c == '"') {
                ss << "\\\"";  // Escape double quotes
            } else if (c == '\\') {
                ss << "\\\\";  // Escape backslashes
            } else {
                ss << c;
            }
        }
        ss << "\\n\"\n\"";  // Add newline character and continue the string
    }

    std::string result = ss.str();
    if (!result.empty()) {
        result.pop_back(); // Remove the last extra '"'
        result.pop_back(); // Remove the last extra '\n'
        result += "\"";    // End the C++ string
    }

    return result;
}

// Function to reverse a C++ string with escape sequences into a plain text file
// Function to reverse a C++ string with escape sequences into a plain text file
std::string reverse_cpp_string(const std::string& input) {
    std::stringstream ss;
    bool in_escape = false;

    // Skip the leading and trailing quotation marks if present
    size_t start = (input.front() == '\"') ? 1 : 0;
    size_t end = (input.back() == '\"') ? input.size() - 1 : input.size();

    for (size_t i = start; i < end; ++i) {
        char c = input[i];
        if (in_escape) {
            if (c == 'n') {
                ss << '\n';
                // Skip any whitespace after newline
                while (i + 1 < end && (input[i + 1] == ' ' || input[i + 1] == '\t' || input[i + 1] == '\"')) {
                    i++;
                }
            } else if (c == '\"') {
                ss << '\"';
            } else if (c == '\\') {
                ss << '\\';
            }
            in_escape = false;
        } else if (c == '\\') {
            in_escape = true;
        } else if (c != '\"') {
            ss << c;
        }
    }

    return ss.str();
}


int main(int argc, char* argv[]) {
    bool reverse_flag = false;
    bool string_input = false;
    std::string input_data;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-r") {
            reverse_flag = true;
        } else if (arg == "-str") {
            string_input = true;
            if (i + 1 < argc) {
                input_data = argv[++i];
            } else {
                std::cerr << "-str flag requires an argument" << std::endl;
                return 1;
            }
        } else {
            input_data = arg;
        }
    }

    if (reverse_flag) {
        if (file_exists(input_data)) {
            std::ifstream file(input_data);
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string file_content = buffer.str();
            std::cout << reverse_cpp_string(file_content) << std::endl;
        } else {
            std::cout << reverse_cpp_string(input_data) << std::endl;
        }
    } else if (string_input) {
        if (file_exists(input_data)) {
            std::cerr << "-str flag requires a literal string, not a file." << std::endl;
            return 1;
        } else {
            std::cout << transform_to_cpp_string(input_data) << std::endl;
        }
    } else {
        if (file_exists(input_data)) {
            std::cout << transform_to_cpp_string(input_data) << std::endl;
        } else {
            std::cerr << "File does not exist: " << input_data << std::endl;
            return 1;
        }
    }

    return 0;
}
