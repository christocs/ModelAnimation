#include "ModelSource.hpp"

Afk::ModelSource::ModelSource(GameObject e, std::string path) {
  this->owning_entity = e;
  this->name          = path;
}
