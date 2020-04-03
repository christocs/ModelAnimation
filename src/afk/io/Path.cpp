#include "afk/io/Path.hpp"

#include <string>

#include <cpplocate/cpplocate.h>

using Afk::Path;
using std::string;

auto Path::get_absolute_path(const string &path) -> string {
  static auto root_dir = cpplocate::getModulePath();

  return root_dir + '/' + path;
}

auto Path::get_directory(const string &path) -> string {
  return path.substr(0, path.find_last_of("/\\"));
}

auto Path::get_filename(const string &path) -> string {
  return path.substr(path.find_last_of("/\\") + 1);
}

auto Path::get_extension(const string &path) -> string {
  return path.substr(path.find_last_of(".") + 1);
}
