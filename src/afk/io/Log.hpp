#pragma once

#include <iostream>
#include <string>

namespace Afk {
  namespace Internal {
    class Status {};
    class Error {};

    template<typename T>
    auto operator<<(Status &log, T const &value) -> Status & {
      std::cout << value;
      return log;
    }
  }

  extern Internal::Status status;
}
