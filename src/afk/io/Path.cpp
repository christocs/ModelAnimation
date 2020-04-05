#include "afk/io/Path.hpp"

#include <filesystem>
#include <string>

#include <cpplocate/cpplocate.h>

using std::string;
using std::filesystem::path;

static auto normalise_path(string raw_path) -> path {
#if defined(WIN32)
  std::replace(raw_path.begin(), raw_path.end(), '\\', '/');
#endif

  return path{raw_path};
}

auto Afk::get_absolute_path(path file_path) -> path {
  static auto root_dir = path{normalise_path(cpplocate::getModulePath())};

  return root_dir / file_path.make_preferred();
}
