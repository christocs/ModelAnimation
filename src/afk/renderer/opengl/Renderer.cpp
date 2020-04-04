#include "afk/renderer/opengl/Renderer.hpp"

#include <cassert>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <stb/stb_image.h>
// Must be loaded after GLAD.
#include <GLFW/glfw3.h>

#include "afk/Afk.hpp"
#include "afk/io/Log.hpp"
#include "afk/io/Path.hpp"
#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/Model.hpp"
#include "afk/renderer/Shader.hpp"
#include "afk/renderer/Texture.hpp"
#include "afk/renderer/opengl/ModelHandle.hpp"
#include "afk/renderer/opengl/ShaderHandle.hpp"
#include "afk/renderer/opengl/ShaderProgramHandle.hpp"
#include "afk/renderer/opengl/TextureHandle.hpp"

using namespace std::string_literals;
using std::pair;
using std::runtime_error;
using std::shared_ptr;
using std::size_t;
using std::string;
using std::unordered_map;
using std::vector;
using std::filesystem::path;

using glm::mat4;
using glm::vec3;
using glm::vec4;

using Afk::Engine;
using Afk::Shader;
using Afk::Texture;
using Afk::OpenGl::ModelHandle;
using Afk::OpenGl::Renderer;
using Afk::OpenGl::ShaderHandle;
using Afk::OpenGl::ShaderProgramHandle;
using Afk::OpenGl::TextureHandle;

static auto get_material_as_string(Texture::Type type) -> string {
  static const auto name = unordered_map<Texture::Type, string>{
      {Texture::Type::Diffuse, "texture_diffuse"},   //
      {Texture::Type::Specular, "texture_specular"}, //
      {Texture::Type::Normal, "texture_normal"},     //
      {Texture::Type::Height, "texture_height"},     //
  };

  return name.at(type);
}

static auto get_gl_shader_type(Shader::Type type) -> GLenum {
  static const auto types = unordered_map<Shader::Type, GLenum>{
      {Shader::Type::Vertex, GL_VERTEX_SHADER},
      {Shader::Type::Fragment, GL_FRAGMENT_SHADER},
  };

  return types.at(type);
}

Renderer::Renderer() {
  if (!glfwInit()) {
    throw runtime_error{"Failed to initialize GLFW"s};
  }

  // FIXME: Give user an option to change graphics settings.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->opengl_major_version);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->opengl_minor_version);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, this->enable_vsync ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifndef __APPLE__
  glfwWindowHint(GLFW_SAMPLES, 4);
#endif

  auto *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  this->window = Window{glfwCreateWindow(mode->width, mode->height, Engine::GAME_NAME,
                                         glfwGetPrimaryMonitor(), nullptr),
                        glfwDestroyWindow};

  if (!this->window.get()) {
    throw runtime_error{"Failed to create window"};
  }

  glfwMakeContextCurrent(this->window.get());

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    throw runtime_error{"Failed to initialize GLAD"s};
  }

  glEnable(GL_DEPTH_TEST);
  Afk::status << "OpenGL renderer initialized.\n";
}

auto Renderer::get_window_size() -> pair<unsigned, unsigned> {
  auto width  = 0;
  auto height = 0;
  glfwGetFramebufferSize(this->window.get(), &width, &height);

  return std::make_pair(static_cast<unsigned>(width), static_cast<unsigned>(height));
}

auto Renderer::clear_screen(vec4 clear_color) const -> void {
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto Renderer::set_viewport(int x, int y, unsigned width, unsigned height) const -> void {
  glViewport(x, y, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

auto Renderer::swap_buffers() -> void {
  glfwSwapBuffers(this->window.get());
}

auto Renderer::get_model(path file_path) -> ModelHandle {
  const auto is_loaded = this->models.count(file_path) == 1;

  if (!is_loaded) {
    this->models[file_path.string()] = this->load_model(Model{file_path});
  }

  return this->models.at(file_path.string());
}

auto Renderer::get_texture(path file_path) -> TextureHandle {
  const auto is_loaded = this->textures.count(file_path) == 1;

  if (!is_loaded) {
    this->textures[file_path.string()] = this->load_texture(Texture{file_path});
  }

  return this->textures.at(file_path.string());
}

auto Renderer::get_shader(path file_path) -> ShaderHandle {
  const auto is_loaded = this->shaders.count(file_path) == 1;

  if (!is_loaded) {
    this->shaders[file_path.string()] = this->compile_shader(Shader{file_path});
  }

  return this->shaders.at(file_path.string());
}

auto Renderer::get_shader_program(const string &name) -> ShaderProgramHandle {
  const auto is_loaded = this->shader_programs.count(name) == 1;

  if (!is_loaded) {
    throw runtime_error{"Shader program '"s + name + "' is not loaded"s};
  }

  return this->shader_programs.at(name);
}

auto Renderer::set_texture_unit(size_t unit) const -> void {
  assert(unit > 0);
  glActiveTexture(unit);
}

auto Renderer::bind_texture(const TextureHandle &texture) const -> void {
  assert(texture.id > 0);
  glBindTexture(GL_TEXTURE_2D, texture.id);
}

auto Renderer::draw_model(const ModelHandle &model, const ShaderProgramHandle &shader,
                          Transform transform) const -> void {
  assert(model.meshes.size() > 0);

  for (const auto &mesh : model.meshes) {
    assert(mesh.vao > 0);
    assert(mesh.num_indices > 0);

    auto material_bound = vector<bool>{};

    const auto num_material_types = static_cast<size_t>(Texture::Type::Count);
    material_bound.resize(num_material_types);
    for (auto i = size_t{0}; i < num_material_types; ++i) {
      material_bound[i] = false;
    }

    // Bind all of the textures to shader uniforms.
    for (auto i = size_t{0}; i < mesh.textures.size(); i++) {
      this->set_texture_unit(GL_TEXTURE0 + i);

      auto name = get_material_as_string(mesh.textures[i].type);

      const auto index = static_cast<size_t>(mesh.textures[i].type);

      if ((material_bound[index])) {
        throw runtime_error{"Material "s + name + " already bound"s};
      } else {
        material_bound[index] = true;
      }

      this->set_uniform(shader, "u_textures."s + name, static_cast<int>(i));
      this->bind_texture(mesh.textures[i]);
    }

    auto model_matrix = mat4{1.0f};

    // Apply parent tranformation.
    model_matrix = glm::translate(model_matrix, transform.translation);
    model_matrix *= glm::mat4_cast(transform.rotation);
    model_matrix = glm::scale(model_matrix, transform.scale);

    // Apply local transformation.
    model_matrix = glm::translate(model_matrix, mesh.transform.translation);
    model_matrix *= glm::mat4_cast(mesh.transform.rotation);
    model_matrix = glm::scale(model_matrix, mesh.transform.scale);

    this->set_uniform(shader, "u_matrices.model", model_matrix);

    // Draw the mesh.
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.num_indices, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    this->set_texture_unit(GL_TEXTURE0);
  }
}

auto Renderer::use_shader(const ShaderProgramHandle &shader) const -> void {
  assert(shader.id > 0);
  glUseProgram(shader.id);
}

auto Renderer::load_mesh(const Mesh &mesh) -> MeshHandle {
  assert(mesh.vertices.size() > 0);
  assert(mesh.indices.size() > 0);

  auto mesh_handle        = MeshHandle{};
  mesh_handle.num_indices = mesh.indices.size();
  mesh_handle.transform   = std::move(mesh.transform);

  // Create new buffers.
  glGenVertexArrays(1, &mesh_handle.vao);
  glGenBuffers(1, &mesh_handle.vbo);
  glGenBuffers(1, &mesh_handle.ibo);

  // Load data into the vertex buffer.
  glBindVertexArray(mesh_handle.vao);
  glBindBuffer(GL_ARRAY_BUFFER, mesh_handle.vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex),
               mesh.vertices.data(), GL_STATIC_DRAW);

  // Load index data into the index buffer.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_handle.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(Mesh::Index),
               mesh.indices.data(), GL_STATIC_DRAW);

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

  return mesh_handle;
}

auto Renderer::load_model(const Model &model) -> ModelHandle {
  const auto is_loaded = this->models.count(model.file_path) == 1;

  if (is_loaded) {
    throw runtime_error{"Model with path '"s + model.file_path.string() + "' already loaded"s};
  }

  auto modelHandle = ModelHandle{};

  // Load meshes and textures.
  for (const auto &mesh : model.meshes) {
    auto mesh_handle = this->load_mesh(mesh);

    for (const auto &texture : mesh.textures) {
      auto texture_handle = this->get_texture(texture.file_path);

      if (texture_handle.type != texture.type) {
        this->textures[texture.file_path].type = texture.type;
        texture_handle.type                    = texture.type;
      }

      mesh_handle.textures.push_back(std::move(texture_handle));
    }

    modelHandle.meshes.push_back(std::move(mesh_handle));
  }

  this->models[model.file_path] = std::move(modelHandle);

  return this->models[model.file_path];
}

auto Renderer::load_texture(const Texture &texture) -> TextureHandle {
  const auto is_loaded = this->textures.count(texture.file_path) == 1;

  if (is_loaded) {
    throw runtime_error{"Texture with path '"s + texture.file_path.string() + "' already loaded"s};
  }

  auto width    = 0;
  auto height   = 0;
  auto channels = 0;
  auto image    = shared_ptr<unsigned char>{
      stbi_load(Afk::get_resource_path(texture.file_path).c_str(), &width,
                &height, &channels, STBI_rgb_alpha),
      stbi_image_free};

  if (image == nullptr) {
    throw runtime_error{"Failed to load image: '"s + texture.file_path.string() + "'"s};
  }

  auto texture_handle = TextureHandle{};
  texture_handle.type = texture.type;

  // Send the texture to the GPU.
  glGenTextures(1, &texture_handle.id);
  glBindTexture(GL_TEXTURE_2D, texture_handle.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image.get());
  glGenerateMipmap(GL_TEXTURE_2D);

  // Set texture parameters.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Afk::status << "Texture '"s << texture.file_path << "' loaded with ID "s
              << texture_handle.id << ".\n"s;
  this->textures[texture.file_path] = std::move(texture_handle);

  return this->textures[texture.file_path];
}

auto Renderer::compile_shader(const Shader &shader) -> ShaderHandle {
  const auto is_loaded = this->shaders.count(shader.file_path) == 1;

  if (is_loaded) {
    throw runtime_error{"Shader with path '"s + shader.file_path.string() + "' already loaded"s};
  }

  auto shader_handle = ShaderHandle{};

  const auto *shader_code_ptr = shader.code.c_str();
  shader_handle.id            = glCreateShader(get_gl_shader_type(shader.type));
  shader_handle.type          = shader.type;

  glShaderSource(shader_handle.id, 1, &shader_code_ptr, nullptr);
  glCompileShader(shader_handle.id);

  auto did_succeed = GLint{};
  glGetShaderiv(shader_handle.id, GL_COMPILE_STATUS, &did_succeed);

  if (!did_succeed) {
    auto error_length = GLint{0};
    auto error_msg    = vector<GLchar>{};

    glGetShaderiv(shader_handle.id, GL_INFO_LOG_LENGTH, &error_length);
    error_msg.resize(static_cast<size_t>(error_length));
    glGetShaderInfoLog(shader_handle.id, error_length, &error_length, error_msg.data());

    throw runtime_error{"Shader compilation failed: "s +
                        shader.file_path.string() + ": "s + error_msg.data()};
  }

  Afk::status << "Shader '"s << shader.file_path << "' compiled with ID "s
              << shader_handle.id << ".\n"s;
  this->shaders[shader.file_path.string()] = std::move(shader_handle);

  return this->shaders[shader.file_path.string()];
}

auto Renderer::link_shaders(const string &name, const ShaderHandles &shader_handles)
    -> ShaderProgramHandle {
  const auto is_loaded = this->shader_programs.count(name) == 1;

  if (is_loaded) {
    throw runtime_error{"Shader program with name '"s + name + "' already loaded"s};
  }

  auto shader_program = ShaderProgramHandle{};

  shader_program.id = glCreateProgram();

  for (const auto &shader : shader_handles) {
    glAttachShader(shader_program.id, shader.id);
  }

  glLinkProgram(shader_program.id);

  auto did_succeed = GLint{};
  glGetProgramiv(shader_program.id, GL_LINK_STATUS, &did_succeed);

  if (!did_succeed) {
    auto error_length = GLint{0};
    auto error_msg    = vector<GLchar>{};

    glGetProgramiv(shader_program.id, GL_INFO_LOG_LENGTH, &error_length);
    error_msg.resize(static_cast<size_t>(error_length));
    glGetProgramInfoLog(shader_program.id, error_length, &error_length,
                        error_msg.data());

    throw runtime_error{"Shader "s + "'"s + name + "' linking failed: "s +
                        error_msg.data()};
  }

  Afk::status << "Shader program '"s << name << "' linked with ID "s
              << shader_program.id << ".\n"s;
  this->shader_programs[name] = std::move(shader_program);

  return this->shader_programs[name];
}

auto Renderer::toggle_wireframe() -> void {
  auto mode = this->wireframe_enabled ? GL_FILL : GL_LINE;
  glPolygonMode(GL_FRONT_AND_BACK, mode);
  this->wireframe_enabled = !this->wireframe_enabled;
}

auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, bool value) const -> void {
  assert(program.id > 0);
  glUniform1i(glGetUniformLocation(program.id, name.c_str()),
              static_cast<GLboolean>(value));
}

auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, int value) const -> void {
  assert(program.id > 0);
  glUniform1i(glGetUniformLocation(program.id, name.c_str()), static_cast<GLint>(value));
}

auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, float value) const -> void {
  assert(program.id > 0);
  glUniform1f(glGetUniformLocation(program.id, name.c_str()), static_cast<GLfloat>(value));
}
auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, vec3 value) const -> void {
  assert(program.id > 0);
  glUniform3fv(glGetUniformLocation(program.id, name.c_str()), 1, glm::value_ptr(value));
}
auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, mat4 value) const -> void {
  assert(program.id > 0);
  glUniformMatrix4fv(glGetUniformLocation(program.id, name.c_str()), 1,
                     GL_FALSE, glm::value_ptr(value));
}
