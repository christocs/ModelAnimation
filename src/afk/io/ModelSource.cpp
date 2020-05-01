#include "ModelSource.hpp"

Afk::ModelSource::ModelSource(GameObject e, std::string name) {
  this->owning_entity = e;
  this->name          = name;
}
