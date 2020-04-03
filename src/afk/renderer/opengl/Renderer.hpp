#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
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

  namespace OpenGl {
    class Renderer {
    public:
      using MeshHandle          = OpenGl::MeshHandle;
      using ModelHandle         = OpenGl::ModelHandle;
      using ShaderHandle        = OpenGl::ShaderHandle;
      using ShaderProgramHandle = OpenGl::ShaderProgramHandle;
      using TextureHandle       = OpenGl::TextureHandle;

      using ShaderMap        = std::unordered_map<std::string, ShaderHandle>;
      using ShaderProgramMap = std::unordered_map<std::string, ShaderProgramHandle>;
      using ModelMap         = std::unordered_map<std::string, ModelHandle>;
      using TextureMap       = std::unordered_map<std::string, TextureHandle>;

      using ShaderHandles = std::vector<ShaderHandle>;
      using Window        = std::shared_ptr<GLFWwindow>;

      Window window = nullptr;

      Renderer();

      auto toggle_wireframe() -> void;
      auto get_window_size() -> std::pair<unsigned, unsigned>;

      // Draw commands
      auto clear_screen(glm::vec4 clear_color = {1.0f, 1.0f, 1.0f, 1.0f}) const -> void;
      auto swap_buffers() -> void;
      auto set_viewport(int x, int y, unsigned width, unsigned height) const -> void;
      auto draw_model(const ModelHandle &model, const ShaderProgramHandle &shader,
                      Transform transform) const -> void;

      // State management
      auto use_shader(const ShaderProgramHandle &shader) const -> void;
      auto set_texture_unit(std::size_t unit) const -> void;
      auto bind_texture(const TextureHandle &texture) const -> void;

      // Resource management
      auto get_model(const std::string &path) -> ModelHandle;
      auto get_texture(const std::string &path) -> TextureHandle;
      auto get_shader(const std::string &path) -> ShaderHandle;
      auto get_shader_program(const std::string &name) -> ShaderProgramHandle;

      // Resource loading
      auto load_model(const Model &model) -> ModelHandle;
      auto load_texture(const Texture &texture) -> TextureHandle;
      auto load_mesh(const Mesh &meshData) -> MeshHandle;
      auto compile_shader(const Shader &shader) -> ShaderHandle;
      auto link_shaders(const std::string &name, const ShaderHandles &shader_handles)
          -> ShaderProgramHandle;

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

      ModelMap models                  = {};
      TextureMap textures              = {};
      ShaderMap shaders                = {};
      ShaderProgramMap shader_programs = {};

      bool enable_vsync      = true;
      bool wireframe_enabled = false;
    };
  }
}
