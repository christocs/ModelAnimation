#pragma once

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.hpp"

using Index = unsigned;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uvs;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    GLuint id;
    std::string type;
    std::string path;
};

class Mesh {
  public:
    std::vector<Vertex> vertices  = {};
    std::vector<Index> indices    = {};
    std::vector<Texture> textures = {};

    Mesh(std::vector<Vertex> &&_vertices, std::vector<Index> &&_indices,
         std::vector<Texture> &&_textures);

    Mesh(const std::vector<Vertex> &_vertices, const std::vector<Index> &_indices,
         const std::vector<Texture> &_textures);

    auto draw(const Shader &shader) const -> void;

  private:
    GLuint vao = {};
    GLuint vbo = {};
    GLuint ibo = {};

    auto setupMesh() -> void;
};
