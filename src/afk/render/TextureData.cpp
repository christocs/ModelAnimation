#include "afk/render/TextureData.hpp"

#include <stdexcept>
#include <string>

#include "afk/io/Path.hpp"

using namespace std::string_literals;
using std::runtime_error;
using std::string;

using Afk::Path;
using Afk::TextureData;

TextureData::TextureData(const string &_path) {
  const auto absPath = Path::getAbsolutePath(_path);

  if (!this->image.loadFromFile(absPath)) {
    throw runtime_error{"Unable to load image: "s + absPath};
  }

  this->path = _path;
}
