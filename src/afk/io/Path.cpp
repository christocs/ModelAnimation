#include "afk/io/Path.hpp"

#include <string>

#include <cpplocate/cpplocate.h>

using Afk::Path;
using std::string;

auto Path::getAbsolutePath(const string &path) -> string {
  static auto rootDir = cpplocate::getModulePath();

  return rootDir + '/' + path;
}

auto Path::getDirectory(const string &path) -> string {
  return path.substr(0, path.find_last_of("/\\"));
}

auto Path::getFilename(const string &path) -> string {
  return path.substr(path.find_last_of("/\\") + 1);
}

auto Path::getExtension(const string &path) -> string {
  return path.substr(path.find_last_of(".") + 1);
}
