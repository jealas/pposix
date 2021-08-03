#include <regex>

#include "pt.hpp"

int main(const int argc, const char *argv[]) {
  std::vector<std::regex> patterns{};

  if (argc == 1) {
    patterns.emplace_back(".+");
  }

  for (auto i{1}; i < argc; ++i) {
    const auto regex_c_str{argv[i]};

    try {
      patterns.emplace_back(regex_c_str);
    } catch (const std::regex_error &error) {
      std::cerr << "Invalid test name regex: " << '"' << regex_c_str << '"' << '\n'
                << "std::regex_error" << '-' << error.what() << std::endl;
      std::exit(EXIT_FAILURE);
    }
  }

  pt::main(patterns);
}