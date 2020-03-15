#include "ModelLoader.hpp"

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

auto ModelLoader::load(string const &path) -> Model {
    const auto modelPath = this->getModelPath(path);
    auto importer        = Assimp::Importer{};

    // Load the model via Assimp.
    const auto *scene = importer.ReadFile(modelPath, this->ASSIMP_PROCESSING_FLAGS);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw runtime_error{"ASSIMP import error: "s + importer.GetErrorString()};
    }

    // Get the model directory.
    this->model.path = modelPath;
    this->model.dir  = this->getModelDir(modelPath);
    // Recursively process the Assimp nodes.
    this->processNode(scene, scene->mRootNode,
                      this->assimpToGlmMat4(scene->mRootNode->mTransformation));

    return std::move(this->model);
}

auto ModelLoader::processNode(const aiScene *scene, const aiNode *node,
                              glm::mat4 transform) -> void {
    // Process all meshes at this node.
    for (auto i = size_t{0}; i < node->mNumMeshes; ++i) {
        const auto *mesh = scene->mMeshes[node->mMeshes[i]];
        this->model.meshes.push_back(this->processMesh(
            scene, mesh, transform * this->assimpToGlmMat4(node->mTransformation)));
    }

    // Process all child nodes.
    for (auto i = size_t{0}; i < node->mNumChildren; ++i) {
        this->processNode(scene, node->mChildren[i],
                          transform * this->assimpToGlmMat4(node->mTransformation));
    }
}

auto ModelLoader::processMesh(const aiScene *scene, const aiMesh *mesh,
                              glm::mat4 transform) -> Mesh {
    auto meshVertices = vector<Vertex>{};
    auto meshIndices  = vector<unsigned>{};
    auto meshTextures = vector<Texture>{};

    // Retrieve this meshes vertices.
    for (auto i = size_t{0}; i < mesh->mNumVertices; ++i) {
        auto vertex = Vertex{};

        vertex.position = glm::vec3{mesh->mVertices[i].x, mesh->mVertices[i].y,
                                    mesh->mVertices[i].z};

        vertex.normal = glm::vec3{mesh->mNormals[i].x, mesh->mNormals[i].y,
                                  mesh->mNormals[i].z};

        // Retrieve texture coordinates, tangents, and bitangents, if available.
        if (mesh->HasTextureCoords(0)) {
            vertex.uvs = glm::vec2{mesh->mTextureCoords[0][i].x,
                                   mesh->mTextureCoords[0][i].y};

            vertex.tangent = glm::vec3{mesh->mTangents[i].x, mesh->mTangents[i].y,
                                       mesh->mTangents[i].z};

            vertex.bitangent =
                glm::vec3{mesh->mBitangents[i].x, mesh->mBitangents[i].y,
                          mesh->mBitangents[i].z};
        }

        meshVertices.push_back(vertex);
    }

    // Retrieve indices from the meshes faces.
    for (auto i = size_t{0}; i < mesh->mNumFaces; ++i) {
        const auto face = mesh->mFaces[i];

        for (auto j = size_t{0}; j < face.mNumIndices; ++j) {
            meshIndices.push_back(face.mIndices[j]);
        }
    }

    // Load material textures.
    const auto *material = scene->mMaterials[mesh->mMaterialIndex];
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
                std::move(meshTextures), std::move(transform)};
}

auto ModelLoader::loadMaterialTextures(const aiMaterial *mat,
                                       const aiTextureType type,
                                       const string &typeName) -> vector<Texture> {
    auto materialTextures = vector<Texture>{};

    // Load all textures associated with the model.
    for (auto i = 0u; i < mat->GetTextureCount(type); ++i) {
        auto assimpPath = aiString{};
        mat->GetTexture(type, i, &assimpPath);
        const auto path      = string{assimpPath.C_Str()};
        const auto &textures = this->model.textures;

        // Check if we've already loaded the texture.
        const auto item = std::find_if(
            textures.begin(), textures.end(),
            [&path](const auto &texture) { return texture.path == path; });

        // Load the texture, or use the previously loaded one, if applicable.
        if (item != textures.end()) {
            materialTextures.push_back(*item);
        } else {
            auto texture = Texture{};

            texture.id   = this->loadTexture(path);
            texture.type = typeName;
            texture.path = path;

            materialTextures.push_back(texture);
            this->model.textures.push_back(texture);
        }
    }

    return materialTextures;
}

auto ModelLoader::loadTexture(const string &path) -> GLuint {
    const auto texturePath = this->getTexturePath(path);

    // Load image.
    auto image = sf::Image{};

    if (!image.loadFromFile(texturePath)) {
        throw runtime_error{"Unable to load image: "s + texturePath};
    }

    const auto [width, height] = image.getSize();

    // Send the image to the GPU.
    auto textureId = GLuint{};
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

    // Set texture parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureId;
}

auto ModelLoader::assimpToGlmMat4(aiMatrix4x4t<float> m) const -> glm::mat4 {
    return glm::mat4{m.a1, m.b1, m.c1, m.d1,  //
                     m.a2, m.b2, m.c2, m.d2,  //
                     m.a3, m.b3, m.c3, m.d3,  //
                     m.a4, m.b4, m.c4, m.d4}; //
}

auto ModelLoader::getModelPath(const string &path) const -> string {
    return cpplocate::getModulePath() + "/" + path;
}

auto ModelLoader::getModelDir(const string &path) const -> string {
    return path.substr(0, path.find_last_of("/\\"));
}

auto ModelLoader::getTexturePath(const string &path) const -> string {
    return this->model.dir + "/textures/" +
           path.substr(path.find_last_of("/\\") + 1);
}
