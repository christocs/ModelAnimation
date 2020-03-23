#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Window/Window.hpp>
#include <glm/fwd.hpp>

#include "afk/render/ShaderData.hpp"
#include "afk/render/opengl/MeshHandle.hpp"
#include "afk/render/opengl/ModelHandle.hpp"
#include "afk/render/opengl/ShaderHandle.hpp"
#include "afk/render/opengl/ShaderProgramHandle.hpp"
#include "afk/render/opengl/TextureHandle.hpp"

namespace Afk {
  struct Model;
  struct MeshData;
  struct ModelData;
  struct TextureData;

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

      static constexpr unsigned OPENGL_MAJOR_VERSION = 4;
      static constexpr unsigned OPENGL_MINOR_VERSION = 1;
      static constexpr unsigned STENCIL_BITS         = 8;
      static constexpr unsigned MSAA_LEVEL           = 4;
#ifdef _WIN32
      static constexpr unsigned DEPTH_BITS = 24;
#else
      static constexpr unsigned DEPTH_BITS = 32;
#endif

      sf::Window window = {};

      Renderer();

      auto clearScreen(glm::vec4 clearColor = {1.0f, 1.0f, 1.0f, 1.0f}) const -> void;
      auto swapBuffers() -> void;
      auto setViewport(int x, int y, unsigned width, unsigned height) const -> void;
      auto drawModel(const ModelHandle &model, const ShaderProgramHandle &shader,
                     Transform transform) const -> void;
      auto useShader(const ShaderProgramHandle &shader) const -> void;
      auto setTextureUnit(std::size_t unit) const -> void;
      auto bindTexture(const TextureHandle &texture) const -> void;

      auto getModel(const std::string &path) -> ModelHandle;
      auto getTexture(const std::string &path) -> TextureHandle;
      auto getShader(const std::string &path) -> ShaderHandle;
      auto getShaderProgram(const std::string &name) -> ShaderProgramHandle;

      auto loadModel(const ModelData &modelData) -> ModelHandle;
      auto loadTexture(const TextureData &textureData) -> TextureHandle;
      auto compileShader(const ShaderData &shaderData) -> ShaderHandle;
      auto linkShaders(const std::string &name, const ShaderHandles &shaderHandles)
          -> ShaderProgramHandle;
      auto loadMesh(const MeshData &meshData) -> MeshHandle;

      auto toggleWireframe() -> void;

      auto setUniform(const ShaderProgramHandle &shader,
                      const std::string &name, bool value) const -> void;
      auto setUniform(const ShaderProgramHandle &shader,
                      const std::string &name, int value) const -> void;
      auto setUniform(const ShaderProgramHandle &shader,
                      const std::string &name, float value) const -> void;
      auto setUniform(const ShaderProgramHandle &shader,
                      const std::string &name, glm::vec3 value) const -> void;
      auto setUniform(const ShaderProgramHandle &shader,
                      const std::string &name, glm::mat4 value) const -> void;

    private:
      ModelMap models                 = {};
      TextureMap textures             = {};
      ShaderMap shaders               = {};
      ShaderProgramMap shaderPrograms = {};
      bool wireframeEnabled           = false;
    };
  }
}
