#include "afk/render/opengl/Renderer.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics/Image.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "afk/Afk.hpp"
#include "afk/io/Log.hpp"
#include "afk/io/Path.hpp"
#include "afk/render/MeshData.hpp"
#include "afk/render/ModelData.hpp"
#include "afk/render/ShaderData.hpp"
#include "afk/render/TextureData.hpp"
#include "afk/render/opengl/ModelHandle.hpp"
#include "afk/render/opengl/ShaderHandle.hpp"
#include "afk/render/opengl/ShaderProgramHandle.hpp"
#include "afk/render/opengl/TextureHandle.hpp"

using namespace std::string_literals;
using std::runtime_error;
using std::size_t;
using std::string;
using std::unordered_map;
using std::vector;

using glm::mat4;
using glm::vec3;
using glm::vec4;
using sf::Image;

using Afk::Log;
using Afk::Path;
using Afk::ShaderData;
using Afk::TextureData;
using Afk::OpenGl::ModelHandle;
using Afk::OpenGl::Renderer;
using Afk::OpenGl::ShaderHandle;
using Afk::OpenGl::ShaderProgramHandle;
using Afk::OpenGl::TextureHandle;

static auto getMaterialName(TextureData::Type type) -> string {
  static const auto types = unordered_map<TextureData::Type, string>{
      {TextureData::Type::Diffuse, "texture_diffuse"},   //
      {TextureData::Type::Specular, "texture_specular"}, //
      {TextureData::Type::Normal, "texture_normal"},     //
      {TextureData::Type::Height, "texture_height"},     //
  };

  return types.at(type);
}

static auto getShaderType(ShaderData::Type type) -> GLenum {
  static const auto types = unordered_map<ShaderData::Type, GLenum>{
      {ShaderData::Type::Vertex, GL_VERTEX_SHADER},
      {ShaderData::Type::Fragment, GL_FRAGMENT_SHADER},
  };

  return types.at(type);
}

Renderer::Renderer()
  : window(sf::Window{
        sf::VideoMode{1920, 1080}, Afk::Engine::GAME_NAME, sf::Style::Fullscreen,
        sf::ContextSettings{this->DEPTH_BITS, this->STENCIL_BITS, this->MSAA_LEVEL,
                            this->OPENGL_MAJOR_VERSION, this->OPENGL_MINOR_VERSION,
                            sf::ContextSettings::Core}}) {

  this->window.setMouseCursorVisible(false);
  this->window.setVerticalSyncEnabled(true);

  if (!gladLoadGL()) {
    throw runtime_error{"Failed to initialize GLAD"};
  }

  glEnable(GL_DEPTH_TEST);
  Log::status("OpenGL renderer initialized.");
}

auto Renderer::clearScreen(vec4 clearColor) const -> void {
  glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto Renderer::setViewport(int x, int y, unsigned width, unsigned height) const -> void {
  glViewport(x, y, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

auto Renderer::swapBuffers() -> void {
  this->window.display();
}

auto Renderer::getModel(const string &path) -> ModelHandle {
  const auto isLoaded = this->models.count(path) == 1;

  if (!isLoaded) {
    this->models[path] = this->loadModel(ModelData{path});
  }

  return this->models.at(path);
}

auto Renderer::getTexture(const string &path) -> TextureHandle {
  const auto isLoaded = this->textures.count(path) == 1;

  if (!isLoaded) {
    this->textures[path] = this->loadTexture(TextureData{path});
  }

  return this->textures.at(path);
}

auto Renderer::getShader(const string &path) -> ShaderHandle {
  const auto isLoaded = this->shaders.count(path) == 1;

  if (!isLoaded) {
    this->shaders[path] = this->compileShader(ShaderData{path});
  }

  return this->shaders.at(path);
}

auto Renderer::getShaderProgram(const string &name) -> ShaderProgramHandle {
  const auto isLoaded = this->shaderPrograms.count(name) == 1;

  if (!isLoaded) {
    throw runtime_error{"Shader program '"s + name + "' is not loaded"s};
  }

  return this->shaderPrograms.at(name);
}

auto Renderer::setTextureUnit(size_t unit) const -> void {
  assert(unit > 0);
  glActiveTexture(unit);
}

auto Renderer::bindTexture(const TextureHandle &texture) const -> void {
  assert(texture.id > 0);
  glBindTexture(GL_TEXTURE_2D, texture.id);
}

auto Renderer::drawModel(const ModelHandle &model, const ShaderProgramHandle &shader,
                         Transform transform) const -> void {
  assert(model.meshes.size() > 0);

  for (const auto &mesh : model.meshes) {
    assert(mesh.vao > 0);
    assert(mesh.numIndices > 0);

    auto materialCount = vector<unsigned>{};

    const auto numMaterialTypes = static_cast<size_t>(TextureData::Type::Count);
    materialCount.resize(numMaterialTypes);
    for (auto i = size_t{0}; i < numMaterialTypes; ++i) {
      materialCount[i] = 1u;
    }

    // Bind all of the textures to shader uniforms.
    for (auto i = size_t{0}; i < mesh.textures.size(); i++) {
      this->setTextureUnit(GL_TEXTURE0 + i);

      auto name   = getMaterialName(mesh.textures[i].type);
      auto number = std::to_string(materialCount[i]);
      materialCount[i] += 1;

      this->setUniform(shader, name + "_" + number, static_cast<int>(i));
      this->bindTexture(mesh.textures[i]);
    }

    auto modelMatrix = mat4{1.0f};

    // Apply parent tranformation.
    modelMatrix = glm::translate(modelMatrix, transform.translation);
    modelMatrix *= glm::mat4_cast(transform.rotation);
    modelMatrix = glm::scale(modelMatrix, transform.scale);

    // Apply local transformation.
    modelMatrix = glm::translate(modelMatrix, mesh.transform.translation);
    modelMatrix *= glm::mat4_cast(mesh.transform.rotation);
    modelMatrix = glm::scale(modelMatrix, mesh.transform.scale);

    this->setUniform(shader, "model", modelMatrix);

    // Draw the mesh.
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    this->setTextureUnit(GL_TEXTURE0);
  }
}

auto Renderer::useShader(const ShaderProgramHandle &shader) const -> void {
  assert(shader.id > 0);
  glUseProgram(shader.id);
}

auto Renderer::loadMesh(const MeshData &meshData) -> MeshHandle {
  assert(meshData.vertices.size() > 0);
  assert(meshData.indices.size() > 0);

  auto meshHandle       = MeshHandle{};
  meshHandle.numIndices = meshData.indices.size();
  meshHandle.transform  = std::move(meshData.transform);

  // Create new buffers.
  glGenVertexArrays(1, &meshHandle.vao);
  glGenBuffers(1, &meshHandle.vbo);
  glGenBuffers(1, &meshHandle.ibo);

  // Load data into the vertex buffer.
  glBindVertexArray(meshHandle.vao);
  glBindBuffer(GL_ARRAY_BUFFER, meshHandle.vbo);
  glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(VertexData),
               meshData.vertices.data(), GL_STATIC_DRAW);

  // Load index data into the index buffer.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshHandle.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(MeshData::Index),
               meshData.indices.data(), GL_STATIC_DRAW);

  // Set the vertex attribute pointers.
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), nullptr);

  // Vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        reinterpret_cast<void *>(offsetof(VertexData, normal)));

  // UVs
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        reinterpret_cast<void *>(offsetof(VertexData, uvs)));

  // Vertex tangent
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        reinterpret_cast<void *>(offsetof(VertexData, tangent)));

  // Vertex bitangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData),
                        reinterpret_cast<void *>(offsetof(VertexData, bitangent)));

  glBindVertexArray(0);

  return meshHandle;
}

auto Renderer::loadModel(const ModelData &modelData) -> ModelHandle {
  const auto isLoaded = this->models.count(modelData.path) == 1;

  if (isLoaded) {
    throw runtime_error{"Model with path '"s + modelData.path + "' already loaded"s};
  }

  auto modelHandle = ModelHandle{};

  // Load meshes and textures.
  for (const auto &mesh : modelData.meshes) {
    auto meshHandle = this->loadMesh(mesh);

    for (const auto &texture : mesh.textures) {
      auto textureHandle = this->getTexture(texture.path);

      if (textureHandle.type != texture.type) {
        this->textures[texture.path].type = texture.type;
        textureHandle.type                = texture.type;
      }

      meshHandle.textures.push_back(std::move(textureHandle));
    }

    modelHandle.meshes.push_back(std::move(meshHandle));
  }

  this->models[modelData.path] = std::move(modelHandle);

  return this->models[modelData.path];
}

auto Renderer::loadTexture(const TextureData &textureData) -> TextureHandle {
  const auto isLoaded = this->textures.count(textureData.path) == 1;

  if (isLoaded) {
    throw runtime_error{"Texture with path '"s + textureData.path + "' already loaded"s};
  }

  const auto [width, height] = textureData.image.getSize();

  auto textureHandle = TextureHandle{};
  textureHandle.type = textureData.type;

  // Send the texture to the GPU.
  glGenTextures(1, &textureHandle.id);
  glBindTexture(GL_TEXTURE_2D, textureHandle.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, textureData.image.getPixelsPtr());
  glGenerateMipmap(GL_TEXTURE_2D);

  // Set texture parameters.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Log::status("Texture '"s + textureData.path + "' loaded with ID "s +
              std::to_string(textureHandle.id) + "."s);
  this->textures[textureData.path] = std::move(textureHandle);

  return this->textures[textureData.path];
}

auto Renderer::compileShader(const ShaderData &shaderData) -> ShaderHandle {
  const auto isLoaded = this->shaders.count(shaderData.path) == 1;

  if (isLoaded) {
    throw runtime_error{"Shader with path '"s + shaderData.path + "' already loaded"s};
  }

  auto shaderHandle = ShaderHandle{};

  const auto *shaderCodePtr = shaderData.code.c_str();
  shaderHandle.id           = glCreateShader(getShaderType(shaderData.type));
  shaderHandle.type         = shaderData.type;

  glShaderSource(shaderHandle.id, 1, &shaderCodePtr, nullptr);
  glCompileShader(shaderHandle.id);

  auto didSucceed = GLint{};
  glGetShaderiv(shaderHandle.id, GL_COMPILE_STATUS, &didSucceed);

  if (!didSucceed) {
    auto errorLength = GLint{0};
    auto errorMsg    = vector<GLchar>{};

    glGetShaderiv(shaderHandle.id, GL_INFO_LOG_LENGTH, &errorLength);
    errorMsg.resize(static_cast<size_t>(errorLength));
    glGetShaderInfoLog(shaderHandle.id, errorLength, &errorLength, errorMsg.data());

    throw runtime_error{"Shader compilation failed: "s + shaderData.path +
                        ": "s + errorMsg.data()};
  }

  Log::status("Shader '"s + shaderData.path + "' compiled with ID "s +
              std::to_string(shaderHandle.id) + "."s);
  this->shaders[shaderData.path] = std::move(shaderHandle);

  return this->shaders[shaderData.path];
}

auto Renderer::linkShaders(const string &name, const ShaderHandles &shaderHandles)
    -> ShaderProgramHandle {
  const auto isLoaded = this->shaderPrograms.count(name) == 1;

  if (isLoaded) {
    throw runtime_error{"Shader program with name '"s + name + "' already loaded"s};
  }

  auto shaderProgram = ShaderProgramHandle{};

  shaderProgram.id = glCreateProgram();

  for (const auto &shader : shaderHandles) {
    glAttachShader(shaderProgram.id, shader.id);
  }

  glLinkProgram(shaderProgram.id);

  auto didSucceed = GLint{};
  glGetProgramiv(shaderProgram.id, GL_LINK_STATUS, &didSucceed);

  if (!didSucceed) {
    auto errorLength = GLint{0};
    auto errorMsg    = vector<GLchar>{};

    glGetProgramiv(shaderProgram.id, GL_INFO_LOG_LENGTH, &errorLength);
    errorMsg.resize(static_cast<size_t>(errorLength));
    glGetProgramInfoLog(shaderProgram.id, errorLength, &errorLength, errorMsg.data());

    throw runtime_error{"Shader "s + "'"s + name + "' linking failed: "s +
                        errorMsg.data()};
  }

  Log::status("Shader program '"s + name + "' linked with ID "s +
              std::to_string(shaderProgram.id) + "."s);
  this->shaderPrograms[name] = std::move(shaderProgram);

  return this->shaderPrograms[name];
}

auto Renderer::toggleWireframe() -> void {
  auto mode = this->wireframeEnabled ? GL_FILL : GL_LINE;
  glPolygonMode(GL_FRONT_AND_BACK, mode);
  this->wireframeEnabled = !this->wireframeEnabled;
}

auto Renderer::setUniform(const ShaderProgramHandle &shader, const string &name,
                          bool value) const -> void {
  assert(shader.id > 0);
  glUniform1i(glGetUniformLocation(shader.id, name.c_str()), static_cast<GLboolean>(value));
}

auto Renderer::setUniform(const ShaderProgramHandle &shader, const string &name,
                          int value) const -> void {
  assert(shader.id > 0);
  glUniform1i(glGetUniformLocation(shader.id, name.c_str()), static_cast<GLint>(value));
}

auto Renderer::setUniform(const ShaderProgramHandle &shader, const string &name,
                          float value) const -> void {
  assert(shader.id > 0);
  glUniform1f(glGetUniformLocation(shader.id, name.c_str()), static_cast<GLfloat>(value));
}
auto Renderer::setUniform(const ShaderProgramHandle &shader, const string &name,
                          vec3 value) const -> void {
  assert(shader.id > 0);
  glUniform3fv(glGetUniformLocation(shader.id, name.c_str()), 1, glm::value_ptr(value));
}
auto Renderer::setUniform(const ShaderProgramHandle &shader, const string &name,
                          mat4 value) const -> void {
  assert(shader.id > 0);
  glUniformMatrix4fv(glGetUniformLocation(shader.id, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}
