#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>
#include <glm/fwd.hpp>
// Must be included after GLAD.
#include <GLFW/glfw3.h>

#include "afk/renderer/Shader.hpp"
#include "afk/renderer/opengl/MeshHandle.hpp"
#include "afk/renderer/opengl/ModelHandle.hpp"
#include "afk/renderer/opengl/ShaderHandle.hpp"
#include "afk/renderer/opengl/ShaderProgramHandle.hpp"
#include "afk/renderer/opengl/TextureHandle.hpp"

namespace Afk {
  struct Model;
  struct Mesh;
  struct Model;
  struct Texture;
  struct ShaderProgram;

  namespace OpenGl {
    class Renderer {
    public:
      using MeshHandle          = OpenGl::MeshHandle;
      using ModelHandle         = OpenGl::ModelHandle;
      using ShaderHandle        = OpenGl::ShaderHandle;
      using ShaderProgramHandle = OpenGl::ShaderProgramHandle;
      using TextureHandle       = OpenGl::TextureHandle;

      struct PathHash {
        auto operator()(const std::filesystem::path &p) const -> std::size_t {
          return std::filesystem::hash_value(p);
        }
      };

      struct PathEquals {
        auto operator()(const std::filesystem::path &lhs,
                        const std::filesystem::path &rhs) const -> bool {
          return lhs.lexically_normal() == rhs.lexically_normal();
        }
      };

      using Models =
          std::unordered_map<std::filesystem::path, ModelHandle, PathHash, PathEquals>;
      using Textures =
          std::unordered_map<std::filesystem::path, TextureHandle, PathHash, PathEquals>;
      using Shaders =
          std::unordered_map<std::filesystem::path, ShaderHandle, PathHash, PathEquals>;
      using ShaderPrograms =
          std::unordered_map<std::filesystem::path, ShaderProgramHandle, PathHash, PathEquals>;

      using Window = std::add_pointer<GLFWwindow>::type;

      Window window          = nullptr;
      bool wireframe_enabled = false;

      Renderer();
      ~Renderer();

      auto set_option(GLenum option, bool state) const -> void;
      auto check_errors() const -> void;
      auto get_window_size() const -> glm::ivec2;

      // Draw commands
      auto clear_screen(glm::vec4 clear_color = {1.0f, 1.0f, 1.0f, 1.0f}) const -> void;
      auto swap_buffers() -> void;
      auto set_viewport(int x, int y, int width, int height) const -> void;
      auto draw_model(const ModelHandle &model, const ShaderProgramHandle &shader,
                      Transform transform) const -> void;

      // State management
      auto use_shader(const ShaderProgramHandle &shader) const -> void;
      auto set_texture_unit(std::size_t unit) const -> void;
      auto bind_texture(const TextureHandle &texture) const -> void;

      // Resource management
      auto get_model(const std::filesystem::path &file_path) -> const ModelHandle &;
      auto get_texture(const std::filesystem::path &file_path) -> const TextureHandle &;
      auto get_shader(const std::filesystem::path &file_path) -> const ShaderHandle &;
      auto get_shader_program(const std::filesystem::path &file_path)
          -> const ShaderProgramHandle &;

      // Resource loading
      auto load_model(const Model &model) -> ModelHandle;
      auto load_texture(const Texture &texture) -> TextureHandle;
      auto load_mesh(const Mesh &meshData) -> MeshHandle;
      auto compile_shader(const Shader &shader) -> ShaderHandle;
      auto link_shaders(const ShaderProgram &shader_program) -> ShaderProgramHandle;

      // Uniform management
      auto set_uniform(const ShaderProgramHandle &program,
                       const std::string &name, bool value) const -> void;
      auto set_uniform(const ShaderProgramHandle &program,
                       const std::string &name, int value) const -> void;
      auto set_uniform(const ShaderProgramHandle &program,
                       const std::string &name, float value) const -> void;
      auto set_uniform(const ShaderProgramHandle &program,
                       const std::string &name, glm::vec3 value) const -> void;
      auto set_uniform(const ShaderProgramHandle &program,
                       const std::string &name, glm::mat4 value) const -> void;

    private:
      const int opengl_major_version = 4;
      const int opengl_minor_version = 1;
      const bool enable_vsync        = true;

      Models models                  = {};
      Textures textures              = {};
      Shaders shaders                = {};
      ShaderPrograms shader_programs = {};
    };
  }
}
