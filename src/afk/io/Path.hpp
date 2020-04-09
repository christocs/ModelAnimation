#pragma once

#include <filesystem>

namespace Afk {
  auto get_absolute_path(const std::filesystem::path &file_path) -> std::filesystem::path;
}
