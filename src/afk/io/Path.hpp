#pragma once

#include <string>

namespace Afk {
  class Path {
  public:
    static auto get_absolute_path(const std::string &path) -> std::string;
    static auto get_directory(const std::string &path) -> std::string;
    static auto get_filename(const std::string &path) -> std::string;
    static auto get_extension(const std::string &path) -> std::string;
  };
}
