#include "afk/io/Path.hpp"

#include <filesystem>

#include <cpplocate/cpplocate.h>

using std::filesystem::path;

auto Afk::get_resource_path(path file_path) -> path {
  static auto root_dir = path{cpplocate::getModulePath()}.make_preferred();

  return root_dir / file_path.make_preferred();
}
