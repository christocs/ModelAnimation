#include "afk/utility/Assert.hpp"

#include <stdexcept>
#include <string>

using std::runtime_error;
using std::size_t;
using std::string;
using namespace std::string_literals;

using Afk::AssertionFailure;

AssertionFailure::AssertionFailure(const char *_msg) : msg(_msg) {}
AssertionFailure::AssertionFailure(const std::string &_msg) : msg(_msg) {}
AssertionFailure::~AssertionFailure() {}

auto AssertionFailure::what() const noexcept -> const char * {
  return this->msg.c_str();
}

auto Afk::assertion(bool condition, const string &msg, const string &file_path,
                    size_t line_num) -> void {
  if (!condition) {
    throw AssertionFailure{file_path + ":"s + std::to_string(line_num) + ": "s + msg};
  }
}

auto Afk::unreachable(const string &file_path, size_t line_num) -> void {
  throw AssertionFailure{file_path + ":"s + std::to_string(line_num) + ": "s +
                         "Hit unreachable statement"};
}
