#pragma once

#include <cstdlib>
#include <iostream>
#include <string>

#if defined(__GNUC__) || defined(__clang__)
  #define AFK_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
  #define AFK_FUNCTION __FUNCSIG__
#else
  #define AFK_FUNCTION __func__
#endif

#define afk_assert(expression, message)                                        \
  Afk::assertion(expression, message, #expression, __FILE__, __LINE__, AFK_FUNCTION)

#define afk_unreachable() Afk::unreachable(__FILE__, __LINE__, AFK_FUNCTION)

#ifdef NDEBUG
  #define afk_assert_debug(expression, message) ((void)0)
#else
  #define afk_assert_debug(expression, message)                                \
    Afk::assertion(expression, message, #expression, __FILE__, __LINE__, AFK_FUNCTION)
#endif

#ifdef NDEBUG
  #define afk_unreachable_debug() ((void)0)
#else
  #define afk_unreachable_debug()                                              \
    Afk::unreachable(__FILE__, __LINE__, AFK_FUNCTION)
#endif

namespace Afk {
  auto assertion(bool condition, const std::string &msg,
                 const std::string &expression, const std::string &file_name,
                 std::size_t line_num, const std::string &function_name) -> void;
  [[noreturn]] auto unreachable(const std::string &file_name, std::size_t line_num,
                                const std::string &function_name) -> void;
}

inline auto Afk::assertion(bool condition, const std::string &msg,
                           const std::string &expression, const std::string &file_path,
                           size_t line_num, const std::string &function_name) -> void {
  if (!condition) {
    std::cerr << "\nAssertion '" + expression + "' failed: " + msg + "\n  in " +
                     function_name + "\n  at " + file_path + ":" +
                     std::to_string(line_num) + "\n  ";
    std::abort();
  }
}

inline auto Afk::unreachable(const std::string &file_path, size_t line_num,
                             const std::string &function_name) -> void {
  std::cerr << "\nUnreachable statement hit\n  in " + function_name + "\n  at " +
                   file_path + ":" + std::to_string(line_num) + "\n  ";
  std::abort();
}
