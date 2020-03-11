#include "Model.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cpplocate/cpplocate.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.hpp"
#include "Shader.hpp"

using namespace std::string_literals;
using std::runtime_error;
using std::string;
using std::vector;

Model::Model(string const &path) {
    this->loadModel(path);
}

auto Model::draw(Shader shader) const -> void {
    for (auto i = size_t{0}; i < meshes.size(); ++i) {
        meshes[i].draw(shader);
    }
}

auto Model::loadModel(string const &path) -> void {
    const auto absPath = cpplocate::getModulePath() + "/" + path;

    auto importer = Assimp::Importer{};
    const auto *scene =
        importer.ReadFile(absPath, aiProcess_Triangulate | aiProcess_FlipUVs |
                                       aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw runtime_error{"ASSIMP import error: "s + importer.GetErrorString()};
    }

    // Get the model directory.
    this->dir = absPath.substr(0, absPath.find_last_of('/'));
    // Recursively process the ASSIMP nodes.
    this->processNode(scene->mRootNode, scene);
}

auto Model::processNode(const aiNode *node, const aiScene *scene) -> void {
    // Process all meshes at this node.
    for (auto i = size_t{0}; i < node->mNumMeshes; ++i) {
        const auto *mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene));
    }

    // Process all child nodes.
    for (auto i = size_t{0}; i < node->mNumChildren; ++i) {
        this->processNode(node->mChildren[i], scene);
    }
}

auto Model::processMesh(const aiMesh *mesh, const aiScene *scene) -> Mesh {
    auto meshVertices = vector<Vertex>{};
    auto meshIndices  = vector<unsigned>{};
    auto meshTextures = vector<Texture>{};

    // Retreieve of this meshes vertices.
    for (auto i = size_t{0}; i < mesh->mNumVertices; ++i) {
        auto vertex = Vertex{};

        vertex.position = glm::vec3{mesh->mVertices[i].x, mesh->mVertices[i].y,
                                    mesh->mVertices[i].z};

        vertex.normal = glm::vec3{mesh->mNormals[i].x, mesh->mNormals[i].y,
                                  mesh->mNormals[i].z};

        // Add texture coordinates if available.
        if (mesh->mTextureCoords[0]) {
            vertex.uvs = glm::vec2{mesh->mTextureCoords[0][i].x,
                                   mesh->mTextureCoords[0][i].y};
        } else {
            vertex.uvs = glm::vec2(0.0f, 0.0f);
        }

        vertex.tangent = glm::vec3{mesh->mTangents[i].x, mesh->mTangents[i].y,
                                   mesh->mTangents[i].z};

        vertex.bitangent =
            glm::vec3{mesh->mBitangents[i].x, mesh->mBitangents[i].y,
                      mesh->mBitangents[i].z};

        meshVertices.push_back(vertex);
    }

    // Retreieve indices from all of this meshes faces.
    for (auto i = size_t{0}; i < mesh->mNumFaces; ++i) {
        const auto face = mesh->mFaces[i];

        for (auto j = size_t{0}; j < face.mNumIndices; ++j) {
            meshIndices.push_back(face.mIndices[j]);
        }
    }

    const auto *material = scene->mMaterials[mesh->mMaterialIndex];

    // Process materials.
    const auto diffuseMaps =
        loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    const auto specularMaps =
        loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    const auto normalMaps =
        loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    const auto heightMaps =
        loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");

    meshTextures.insert(meshTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
    meshTextures.insert(meshTextures.end(), specularMaps.begin(),
                        specularMaps.end());
    meshTextures.insert(meshTextures.end(), normalMaps.begin(), normalMaps.end());
    meshTextures.insert(meshTextures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh{std::move(meshVertices), std::move(meshIndices),
                std::move(meshTextures)};
}

auto Model::loadMaterialTextures(const aiMaterial *mat, const aiTextureType type,
                                 const string &typeName) -> vector<Texture> {
    auto texturesLoaded = vector<Texture>{};

    // Load all textures associated with the model.
    for (auto i = 0u; i < mat->GetTextureCount(type); ++i) {
        auto alreadyLoaded = false;
        auto path          = aiString{};
        mat->GetTexture(type, i, &path);

        // Check if we've already loaded the texture.
        for (auto j = size_t{0}; j < this->textures.size(); ++j) {
            if (path == aiString{this->textures[j].path}) {
                texturesLoaded.push_back(this->textures[j]);
                alreadyLoaded = true;
                break;
            }
        }

        // Load the texture.
        if (!alreadyLoaded) {
            auto texture = Texture{};

            texture.id   = this->loadTexture(path.C_Str());
            texture.type = typeName;
            texture.path = path.C_Str();

            texturesLoaded.push_back(texture);
            this->textures.push_back(texture);
        }
    }

    return texturesLoaded;
}

auto Model::loadTexture(const string &path) -> GLuint {
    const auto absPath = this->dir + '/' + path;

    auto textureID = GLuint{};
    glGenTextures(1, &textureID);

    auto image = sf::Image{};

    if (!image.loadFromFile(absPath)) {
        throw runtime_error{"Unable to load image: "s + absPath};
    }

    const auto [width, height] = image.getSize();

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}
