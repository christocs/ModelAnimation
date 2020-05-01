#include "ModelSource.hpp"

Afk::ModelSource::ModelSource(GameObject e, std::string path, std::filesystem::path shader_path) {
  this->owning_entity = e;
  this->name          = path;
  this->shader_program_path = shader_path;
}
