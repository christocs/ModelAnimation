#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include "afk/Afk.hpp"
#include "afk/ui/Log.hpp"

using std::ostringstream;

namespace Afk {
  namespace Internal {
    class Status {};
    class Error {};

    template<typename T>
    auto operator<<(Status &log, T const &value) -> Status & {
      auto &afk = Engine::get();

      std::cout << value;
      auto ss = ostringstream{};
      ss << value;
      afk.ui.log.append("%s", ss.str().c_str());

      return log;
    }
  }

  extern Internal::Status status;
}
