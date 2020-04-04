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

    template<typename T>
    auto operator<<(Error &log, T const &value) -> Error & {
      std::cerr << value;
      return log;
    }
  }

  extern Internal::Status status;
  extern Internal::Error error;
}
