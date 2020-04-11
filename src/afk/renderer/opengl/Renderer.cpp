#include "afk/renderer/opengl/Renderer.hpp"

#include <filesystem>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <frozen/unordered_map.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <stb/stb_image.h>
// Must be loaded after GLAD.
#include <GLFW/glfw3.h>

#include "afk/Afk.hpp"
#include "afk/debug/Assert.hpp"
#include "afk/io/Log.hpp"
#include "afk/io/Path.hpp"
#include "afk/renderer/Mesh.hpp"
#include "afk/renderer/Model.hpp"
#include "afk/renderer/Shader.hpp"
#include "afk/renderer/ShaderProgram.hpp"
#include "afk/renderer/Texture.hpp"
#include "afk/renderer/opengl/ModelHandle.hpp"
#include "afk/renderer/opengl/ShaderHandle.hpp"
#include "afk/renderer/opengl/ShaderProgramHandle.hpp"
#include "afk/renderer/opengl/TextureHandle.hpp"

using namespace std::string_literals;
using std::pair;
using std::shared_ptr;
using std::size_t;
using std::string;
using std::unordered_map;
using std::vector;
using std::filesystem::path;

using glm::ivec2;
using glm::mat4;
using glm::vec3;
using glm::vec4;

using Afk::Engine;
using Afk::Shader;
using Afk::ShaderProgram;
using Afk::Texture;
using Afk::OpenGl::ModelHandle;
using Afk::OpenGl::Renderer;
using Afk::OpenGl::ShaderHandle;
using Afk::OpenGl::ShaderProgramHandle;
using Afk::OpenGl::TextureHandle;

constexpr auto material_strings =
    frozen::make_unordered_map<Texture::Type, const char *>({
        {Texture::Type::Diffuse, "texture_diffuse"},
        {Texture::Type::Specular, "texture_specular"},
        {Texture::Type::Normal, "texture_normal"},
        {Texture::Type::Height, "texture_height"},
    });

constexpr auto gl_shader_types = frozen::make_unordered_map<Shader::Type, GLenum>({
    {Shader::Type::Vertex, GL_VERTEX_SHADER},
    {Shader::Type::Fragment, GL_FRAGMENT_SHADER},
});

auto Renderer::set_option(GLenum option, bool state) const -> void {
  if (state) {
    glEnable(option);
  } else {
    glDisable(option);
  }
}

Renderer::Renderer()
  : models(0, PathHash{}, PathEquals{}), textures(0, PathHash{}, PathEquals{}),
    shaders(0, PathHash{}, PathEquals{}),
    shader_programs(0, PathHash{}, PathEquals{}) {
  afk_assert(glfwInit(), "Failed to initialize GLFW");

  // FIXME: Give user an option to change graphics settings.
  glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_NATIVE_CONTEXT_API, GLFW_NATIVE_CONTEXT_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->opengl_major_version);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->opengl_minor_version);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, this->enable_vsync ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifndef __APPLE__
  glfwWindowHint(GLFW_SAMPLES, 4);
#endif

  auto *mode   = glfwGetVideoMode(glfwGetPrimaryMonitor());
  this->window = glfwCreateWindow(mode->width, mode->height, Engine::GAME_NAME,
                                  glfwGetPrimaryMonitor(), nullptr);

  afk_assert(this->window != nullptr, "Failed to create window");
  glfwMakeContextCurrent(this->window);
  afk_assert(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)),
             "Failed to initialize GLAD");

  Afk::status << "Renderer subsystem (OpenGL) initialized.\n";
}

Renderer::~Renderer() {
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

auto Renderer::get_window_size() const -> ivec2 {
  auto width  = 0;
  auto height = 0;
  glfwGetFramebufferSize(this->window, &width, &height);

  return ivec2{width, height};
}

auto Renderer::clear_screen(vec4 clear_color) const -> void {
  afk_assert_debug(clear_color.x >= 0.0f && clear_color.x <= 255.0f,
                   "Red channel out of range");
  afk_assert_debug(clear_color.y >= 0.0f && clear_color.y <= 255.0f,
                   "Green channel out of range");
  afk_assert_debug(clear_color.z >= 0.0f && clear_color.z <= 255.0f,
                   "Blue channel out of range");
  afk_assert_debug(clear_color.w >= 0.0f && clear_color.w <= 1.0f,
                   "Alpha channel out of range");

  glClearColor(clear_color.x / 255.0f, clear_color.y / 255.0f,
               clear_color.z / 255.0f, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  this->set_option(GL_DEPTH_TEST, true);
}

auto Renderer::set_viewport(int x, int y, int width, int height) const -> void {
  glViewport(x, y, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

auto Renderer::swap_buffers() -> void {
  glfwSwapBuffers(this->window);
}

auto Renderer::get_model(const path &file_path) -> const ModelHandle & {
  const auto is_loaded = this->models.count(file_path) == 1;

  if (!is_loaded) {
    this->models[file_path] = this->load_model(Model{file_path});
  }

  return this->models.at(file_path);
}

auto Renderer::get_texture(const path &file_path) -> const TextureHandle & {
  const auto is_loaded = this->textures.count(file_path) == 1;

  if (!is_loaded) {
    this->textures[file_path] = this->load_texture(Texture{file_path});
  }

  return this->textures.at(file_path);
}

auto Renderer::get_shader(const path &file_path) -> const ShaderHandle & {
  const auto is_loaded = this->shaders.count(file_path) == 1;

  if (!is_loaded) {
    this->shaders[file_path] = this->compile_shader(Shader{file_path});
  }

  return this->shaders.at(file_path);
}

auto Renderer::get_shader_program(const path &file_path) -> const ShaderProgramHandle & {
  const auto is_loaded = this->shader_programs.count(file_path) == 1;

  if (!is_loaded) {
    this->shader_programs[file_path] = this->link_shaders(ShaderProgram{file_path});
  }

  return this->shader_programs.at(file_path);
}

auto Renderer::set_texture_unit(size_t unit) const -> void {
  afk_assert_debug(unit > 0, "Invalid texure ID");
  glActiveTexture(unit);
}

auto Renderer::bind_texture(const TextureHandle &texture) const -> void {
  afk_assert_debug(texture.id > 0, "Invalid texture unit");
  glBindTexture(GL_TEXTURE_2D, texture.id);
}

auto Renderer::draw_model(const ModelHandle &model, const ShaderProgramHandle &shader,
                          Transform transform) const -> void {
  glPolygonMode(GL_FRONT_AND_BACK, this->wireframe_enabled ? GL_LINE : GL_FILL);

  for (const auto &mesh : model.meshes) {

    auto material_bound = vector<bool>(static_cast<size_t>(Texture::Type::Count));

    // Bind all of the textures to shader uniforms.
    for (auto i = size_t{0}; i < mesh.textures.size(); i++) {
      this->set_texture_unit(GL_TEXTURE0 + i);

      auto name = material_strings.at(mesh.textures[i].type);

      const auto index = static_cast<size_t>(mesh.textures[i].type);

      afk_assert_debug(!material_bound[index], "Material "s + name + " already bound"s);
      material_bound[index] = true;

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
    glDrawElements(GL_TRIANGLES, mesh.num_indices, MeshHandle::INDEX, nullptr);
    glBindVertexArray(0);

    this->set_texture_unit(GL_TEXTURE0);
  }
}

auto Renderer::use_shader(const ShaderProgramHandle &shader) const -> void {
  afk_assert_debug(shader.id > 0, "Invalid shader ID");
  glUseProgram(shader.id);
}

auto Renderer::load_mesh(const Mesh &mesh) -> MeshHandle {
  afk_assert(mesh.vertices.size() > 0, "Mesh missing vertices");
  afk_assert(mesh.indices.size() > 0, "Mesh missing indices");
  afk_assert(mesh.indices.size() < std::numeric_limits<Mesh::Index>::max(),
             "Too many indices");

  auto mesh_handle        = MeshHandle{};
  mesh_handle.num_indices = mesh.indices.size();
  mesh_handle.transform   = std::move(mesh.transform);

  // Create new buffers.
  glGenVertexArrays(1, &mesh_handle.vao);
  glGenBuffers(1, &mesh_handle.vbo);
  glGenBuffers(1, &mesh_handle.ibo);

  afk_assert(mesh_handle.vao > 0, "Mesh VAO creation failed");
  afk_assert(mesh_handle.vbo > 0, "Mesh VBO creation failed");
  afk_assert(mesh_handle.ibo > 0, "Mesh IBO creation failed");

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

  afk_assert(!is_loaded, "Model with path '"s + model.file_path.string() + "' already loaded"s);

  auto modelHandle = ModelHandle{};

  // Load meshes and textures.
  for (const auto &mesh : model.meshes) {
    auto mesh_handle = this->load_mesh(mesh);

    for (const auto &texture : mesh.textures) {
      const auto &texture_handle = this->get_texture(texture.file_path);
      auto &loaded_handle        = this->textures[texture.file_path];

      // FIXME: There's definitely a more elegant way to do this.
      if (loaded_handle.type != texture.type) {
        loaded_handle.type = texture.type;
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

  afk_assert(!is_loaded, "Texture with path '"s + texture.file_path.string() + "' already loaded"s);

  auto width    = 0;
  auto height   = 0;
  auto channels = 0;
  auto image    = shared_ptr<unsigned char>{
      stbi_load(Afk::get_absolute_path(texture.file_path).string().c_str(),
                &width, &height, &channels, STBI_rgb_alpha),
      stbi_image_free};

  afk_assert(image != nullptr,
             "Failed to load image: '"s + texture.file_path.string() + "'"s);

  auto texture_handle = TextureHandle{};
  texture_handle.type = texture.type;

  // Send the texture to the GPU.
  glGenTextures(1, &texture_handle.id);
  afk_assert(texture_handle.id > 0, "Texture creation failed");
  glBindTexture(GL_TEXTURE_2D, texture_handle.id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image.get());
  glGenerateMipmap(GL_TEXTURE_2D);

  // Set texture parameters.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  Afk::status << "Texture '" << texture.file_path.string()
              << "' loaded with ID " << texture_handle.id << ".\n";
  this->textures[texture.file_path] = std::move(texture_handle);

  return this->textures[texture.file_path];
}

auto Renderer::compile_shader(const Shader &shader) -> ShaderHandle {
  const auto is_loaded = this->shaders.count(shader.file_path) == 1;

  afk_assert(!is_loaded, "Shader with path '"s + shader.file_path.string() + "' already loaded"s);

  auto shader_handle = ShaderHandle{};

  const auto *shader_code_ptr = shader.code.c_str();
  shader_handle.id            = glCreateShader(gl_shader_types.at(shader.type));
  shader_handle.type          = shader.type;

  afk_assert(shader_handle.id > 0, "Shader creation failed");

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

    afk_assert(false, "Shader compilation failed: "s +
                          shader.file_path.string() + ": "s + error_msg.data());
  }

  Afk::status << "Shader '" << shader.file_path.string()
              << "' compiled with ID " << shader_handle.id << ".\n";
  this->shaders[shader.file_path] = std::move(shader_handle);

  return this->shaders[shader.file_path];
}

auto Renderer::link_shaders(const ShaderProgram &shader_program) -> ShaderProgramHandle {
  const auto is_loaded = this->shader_programs.count(shader_program.file_path) == 1;

  afk_assert(!is_loaded, "Shader program with path '"s +
                             shader_program.file_path.string() + "' already loaded"s);

  auto shader_program_handle = ShaderProgramHandle{};

  shader_program_handle.id = glCreateProgram();
  afk_assert(shader_program_handle.id > 0, "Shader program creation failed");

  for (const auto &shader_path : shader_program.shader_paths) {
    const auto &shader_handle = this->get_shader(shader_path);
    glAttachShader(shader_program_handle.id, shader_handle.id);
  }

  glLinkProgram(shader_program_handle.id);

  auto did_succeed = GLint{};
  glGetProgramiv(shader_program_handle.id, GL_LINK_STATUS, &did_succeed);

  if (!did_succeed) {
    auto error_length = GLint{0};
    auto error_msg    = vector<GLchar>{};

    glGetProgramiv(shader_program_handle.id, GL_INFO_LOG_LENGTH, &error_length);
    error_msg.resize(static_cast<size_t>(error_length));
    glGetProgramInfoLog(shader_program_handle.id, error_length, &error_length,
                        error_msg.data());

    afk_assert(false, "Shader "s + "'"s + shader_program.file_path.string() +
                          "' linking failed: "s + error_msg.data());
  }

  Afk::status << "Shader program '" << shader_program.file_path.string()
              << "' linked with ID " << shader_program_handle.id << ".\n";
  this->shader_programs[shader_program.file_path] = std::move(shader_program_handle);

  return this->shader_programs[shader_program.file_path];
}

auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, bool value) const -> void {
  afk_assert_debug(program.id > 0, "Invalid shader program ID");
  glUniform1i(glGetUniformLocation(program.id, name.c_str()),
              static_cast<GLboolean>(value));
}

auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, int value) const -> void {
  afk_assert_debug(program.id > 0, "Invalid shader program ID");
  glUniform1i(glGetUniformLocation(program.id, name.c_str()), static_cast<GLint>(value));
}

auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, float value) const -> void {
  afk_assert_debug(program.id > 0, "Invalid shader program ID");
  glUniform1f(glGetUniformLocation(program.id, name.c_str()), static_cast<GLfloat>(value));
}
auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, vec3 value) const -> void {
  afk_assert_debug(program.id > 0, "Invalid shader program ID");
  glUniform3fv(glGetUniformLocation(program.id, name.c_str()), 1, glm::value_ptr(value));
}
auto Renderer::set_uniform(const ShaderProgramHandle &program,
                           const string &name, mat4 value) const -> void {
  afk_assert_debug(program.id > 0, "Invalid shader program ID");
  glUniformMatrix4fv(glGetUniformLocation(program.id, name.c_str()), 1,
                     GL_FALSE, glm::value_ptr(value));
}
