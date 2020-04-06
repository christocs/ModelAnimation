#include "afk/ui/Ui.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "afk/Afk.hpp"
#include "afk/renderer/Renderer.hpp"
#include "afk/utility/Assert.hpp"

using Afk::Engine;
using Afk::Ui;

Ui::Ui(Renderer::Window _window) : window(_window) {
  afk_assert(_window != nullptr, "Window is uninitialized");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  auto &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(this->window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
}

Ui::~Ui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

auto Ui::draw() -> void {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (this->is_visible) {
    ImGui::ShowDemoWindow(&this->is_visible);
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
