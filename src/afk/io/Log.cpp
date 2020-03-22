#include "afk/io/Log.hpp"

#include <iostream>
#include <string>

using Afk::Log;

auto Log::status(const std::string &msg) -> void {
  if (Log::SHOW_STATUS) {
    std::cout << msg << '\n';
  }
}

auto Log::error(const std::string &msg) -> void {
  if (Log::SHOW_ERROR) {
    std::cerr << msg << '\n';
  }
}
