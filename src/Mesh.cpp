#include "Mesh.hpp"

#include <iostream>
#include <vector>

#include <glad/glad.h>

using namespace std::string_literals;
using std::size_t;
using std::vector;

Mesh::Mesh(vector<Vertex> &&_vertices, vector<Index> &&_indices,
           vector<Texture> &&_textures)
    : vertices(std::move(_vertices)), indices(std::move(_indices)),
      textures(std::move(_textures)) {
    this->setupMesh();
}

Mesh::Mesh(const vector<Vertex> &_vertices, const vector<Index> &_indices,
           const vector<Texture> &_textures)
    : vertices(_vertices), indices(_indices), textures(_textures) {
    this->setupMesh();
}

auto Mesh::draw(const Shader &shader) const -> void {
    auto diffuseNum  = 1u;
    auto specularNum = 1u;
    auto normalNum   = 1u;
    auto heightNum   = 1u;

    for (auto i = size_t{0}; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        auto number = ""s;
        auto name   = textures[i].type;

        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNum);
            ++diffuseNum;
        } else if (name == "texture_specular") {
            number = std::to_string(specularNum);
            ++specularNum;
        } else if (name == "texture_normal") {
            number = std::to_string(normalNum);
            ++normalNum;
        } else if (name == "texture_height") {
            number = std::to_string(heightNum);
            ++heightNum;
        }

        shader.setUniform(name + number, static_cast<unsigned>(i));
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

auto Mesh::setupMesh() -> void {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ibo);

    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
                 this->vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(Index),
                 this->indices.data(), GL_STATIC_DRAW);

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
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, bitangent)));

    glBindVertexArray(0);
}
