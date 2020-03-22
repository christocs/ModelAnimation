#pragma once

#include <string>

namespace Afk {
  class Path {
  public:
    static auto getAbsolutePath(const std::string &path) -> std::string;
    static auto getDirectory(const std::string &path) -> std::string;
    static auto getFilename(const std::string &path) -> std::string;
    static auto getExtension(const std::string &path) -> std::string;
  };
}
