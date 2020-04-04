#pragma once

#include <filesystem>

namespace Afk {
  auto get_resource_path(std::filesystem::path file_path) -> std::filesystem::path;
}
