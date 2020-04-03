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

#include "afk/renderer/GlfwDeleter.hpp"
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

      auto getWindowSize() -> std::pair<unsigned, unsigned>;

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

      auto loadModel(const Model &modelData) -> ModelHandle;
      auto loadTexture(const Texture &textureData) -> TextureHandle;
      auto compileShader(const Shader &shaderData) -> ShaderHandle;
      auto linkShaders(const std::string &name, const ShaderHandles &shaderHandles)
          -> ShaderProgramHandle;
      auto loadMesh(const Mesh &meshData) -> MeshHandle;

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

      bool enableVsync       = true;
      int openglMajorVersion = 4;
      int openglMinorVersion = 1;
      unsigned windowWidth   = 1024;
      unsigned windowHeight  = 768;
    };
  }
}
