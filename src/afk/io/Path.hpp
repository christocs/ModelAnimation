#pragma once

#include <filesystem>

namespace Afk {
  /**
   * gets an absolute path (of something normally relative to the executable)
   * since our current working directory may not always be where the exe is
   */
  auto get_absolute_path(const std::filesystem::path &file_path) -> std::filesystem::path;
}
