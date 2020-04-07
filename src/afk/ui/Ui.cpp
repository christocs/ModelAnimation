#include "afk/ui/Ui.hpp"

#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <imgui/imgui.h>

#include "afk/Afk.hpp"
#include "afk/io/Log.hpp"
#include "afk/io/Path.hpp"
#include "afk/renderer/Renderer.hpp"
#include "afk/utility/Assert.hpp"

using Afk::Engine;
using Afk::Ui;

Ui::Ui(Renderer::Window _window)
  : ini_path(Afk::get_absolute_path("cfg/imgui.ini").string()), window(_window) {
  afk_assert(_window != nullptr, "Window is uninitialized");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  auto &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.IniFilename = this->ini_path.c_str();
  // ImGui::LoadIniSettingsFromDisk(this->ini_path.c_str());
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(this->window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
  io.Fonts->AddFontFromFileTTF(
      Afk::get_absolute_path("res/font/NotoSans-Regular.ttf").string().c_str(), 19.0f);
  auto &style = ImGui::GetStyle();
  style.ScaleAllSizes(1.5f);

  Afk::status << "UI subsystem initialized.\n";
}

Ui::~Ui() {
  // ImGui::SaveIniSettingsToDisk(this->ini_path.c_str());
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

auto Ui::draw() -> void {
  if (this->is_visible) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow(&this->is_visible);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
}
