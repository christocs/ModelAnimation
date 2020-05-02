#include "ModelSource.hpp"

Afk::ModelSource::ModelSource(GameObject e, const std::filesystem::path &name_,
                              const std::filesystem::path &shader_path) {
  this->owning_entity = e;
  this->name          = name_;
  this->shader_program_path = shader_path;
}
