#pragma once

#include <string>

namespace Afk {
  class Log {
  public:
    static constexpr bool SHOW_STATUS = true;
    static constexpr bool SHOW_ERROR  = true;

    static auto status(const std::string &msg) -> void;
    static auto error(const std::string &msg) -> void;
  };
}
