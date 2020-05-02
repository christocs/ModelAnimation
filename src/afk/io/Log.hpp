#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>

#include "afk/Afk.hpp"
#include "afk/io/Path.hpp"
#include "afk/ui/Log.hpp"

namespace Afk {
  namespace Io {
    /**
     * logger
     */
    struct Log {
      std::filesystem::path log_path = {};
      std::ofstream log_file         = {};

      Log();
    };

    /**
     * log value to stream
     * @param value thing to log
     */
    template<typename T>
    auto operator<<(Log &log, T const &value) -> Log & {
      using std::ostringstream;

      auto &afk = Engine::get();
      auto ss   = ostringstream{};

      ss << value;
      afk.ui.log.append("%s", ss.str().c_str());
      log.log_file << value;

      return log;
    }
    /**
     * logger
     */
    inline auto log = Log{};
  }
}
