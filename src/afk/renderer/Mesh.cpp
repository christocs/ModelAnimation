#include "afk/renderer/Mesh.hpp"

#include "afk/debug/Assert.hpp"

// add bone id and weight to the first zero-weight found
auto Afk::Vertex::add_bone(unsigned int id, float weight) -> void
{
  afk_assert(this->no_bones < Afk::Vertex::MAX_BONES, "Max bones already assigned to vertex");
  this->bone_ids[this->no_bones] = id;
  this->bone_weights[this->no_bones] = weight;
  this->no_bones++;
}
