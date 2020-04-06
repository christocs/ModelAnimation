#pragma once

#include <string>

#define afk_assert(condition, message)                                         \
  Afk::assertion(condition, message, __FILE__, __LINE__)

#define afk_unreachable() Afk::unreachable(__FILE__, __LINE__)

namespace Afk {
  class AssertionFailure : public std::exception {
  public:
    explicit AssertionFailure(const char *_msg);
    explicit AssertionFailure(const std::string &_msg);
    virtual ~AssertionFailure();
    virtual auto what() const noexcept -> const char *;

  protected:
    std::string msg;
  };

  auto assertion(bool condition, const std::string &msg,
                 const std::string &file_name, std::size_t line_num) -> void;
  [[noreturn]] auto unreachable(const std::string &file_name, std::size_t line_num) -> void;
}
