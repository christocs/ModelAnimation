#include "afk/render/Mesh.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "afk/render/Shader.hpp"

using namespace std::string_literals;
using glm::mat4;
using glm::vec3;
using std::size_t;
using std::string;
using std::unordered_map;
using std::vector;

using Afk::Render::Mesh;
using Afk::Render::Transform;

// FIXME: Decouple from OpenGL.
// FIXME: Needs logic for properly handling copying verses moving.

Transform::Transform(mat4 transform) {
    auto _scale       = glm::vec3{};
    auto _rotation    = glm::quat{};
    auto _translation = glm::vec3{};
    auto _skew        = glm::vec3{};
    auto _perspective = glm::vec4{};

    glm::decompose(transform, _scale, _rotation, _translation, _skew, _perspective);

    this->translation = _translation;
    this->scale       = _scale;
    this->rotation    = _rotation;
}

Mesh::Mesh(Vertices &&_vertices, Indices &&_indices, Textures &&_textures,
           mat4 &&_transform)
    : vertices(std::move(_vertices)), indices(std::move(_indices)),
      textures(std::move(_textures)), transform(std::move(_transform)) {
    this->setupMesh();
}

auto Mesh::draw(const Shader &shader, Transform parentTransform) const -> void {
    auto map = unordered_map<string, unsigned>{
        {"texture_diffuse", 1u},  //
        {"texture_specular", 1u}, //
        {"texture_normal", 1u},   //
        {"texture_height", 1u},   //
    };

    // Bind all of the textures to shader uniforms.
    for (auto i = size_t{0}; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        auto name   = textures[i].type;
        auto number = std::to_string(map[name]);
        map[name] += 1;

        shader.setUniform(name + number, static_cast<unsigned>(i));
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Apply parent tranformation.
    auto model = mat4{1.0f};

    model = glm::translate(model, parentTransform.translation);
    model *= glm::mat4_cast(parentTransform.rotation);
    model = glm::scale(model, parentTransform.scale);

    // Apply local transformation.
    model = glm::translate(model, this->transform.translation);
    model *= glm::mat4_cast(this->transform.rotation);
    model = glm::scale(model, this->transform.scale);

    shader.setUniform("model", model);

    // Draw the mesh.
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

auto Mesh::setupMesh() -> void {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ibo);

    // Load data into the vertex buffer.
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                 this->vertices.data(), GL_STATIC_DRAW);

    // Load index data into the index buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(Index),
                 this->indices.data(), GL_STATIC_DRAW);

    // Set the vertex attribute pointers.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, normal)));

    // UVs
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, uvs)));

    // Vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, tangent)));
    // Vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, bitangent)));

    glBindVertexArray(0);
}
