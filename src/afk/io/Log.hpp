#pragma once

#include <sstream>

#include "afk/Afk.hpp"
#include "afk/ui/Log.hpp"

namespace Afk {
  namespace Io {
    struct Log {};

    template<typename T>
    auto operator<<(Log &log, T const &value) -> Log & {
      auto &afk = Engine::get();

      auto ss = std::ostringstream{};
      ss << value;
      afk.ui.log.append("%s", ss.str().c_str());

      return log;
    }

    extern Log log;
  }
}
