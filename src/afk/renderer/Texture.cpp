#include "afk/renderer/Texture.hpp"

#include <stdexcept>
#include <string>

#include "afk/io/Path.hpp"

using namespace std::string_literals;
using std::runtime_error;
using std::string;

using Afk::Path;
using Afk::Texture;

Texture::Texture(const string &_path) {
  const auto abs_path = Path::get_absolute_path(_path);

  this->path = _path;
}
