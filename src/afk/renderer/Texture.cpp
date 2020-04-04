#include "afk/renderer/Texture.hpp"

#include <filesystem>

#include "afk/io/Path.hpp"

using std::filesystem::path;

using Afk::Texture;

Texture::Texture(path _file_path) {
  this->file_path = Afk::get_resource_path(_file_path);
}
