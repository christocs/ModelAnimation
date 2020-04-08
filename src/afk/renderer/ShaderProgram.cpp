#include "afk/renderer/ShaderProgram.hpp"

#include <filesystem>
#include <fstream>
#include <string>

#include "afk/io/Path.hpp"
#include "afk/renderer/Shader.hpp"
#include "afk/utility/Assert.hpp"

using std::ifstream;
using std::string;
using std::filesystem::path;
using namespace std::string_literals;

using Afk::ShaderProgram;

ShaderProgram::ShaderProgram(const path &_file_path) {
  const auto abs_path = Afk::get_absolute_path(_file_path);
  auto file           = ifstream{abs_path};

  afk_assert(file.is_open(),
             "Unable to open shader program '"s + _file_path.string() + "'"s);

  auto line = string{};
  while (std::getline(file >> std::ws, line)) {
    this->shader_paths.push_back(path{line});
  }

  this->file_path = _file_path;
}
