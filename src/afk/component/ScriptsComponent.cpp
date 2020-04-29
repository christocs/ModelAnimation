#include "ScriptsComponent.hpp"

#include "afk/Afk.hpp"

using Afk::ScriptsComponent;

ScriptsComponent::ScriptsComponent() : loaded_files(), last_write() {}
ScriptsComponent::~ScriptsComponent() {}

auto ScriptsComponent::add_script(const path &script_path, lua_State *lua) -> void {
  auto lua_script = LuaScript{&(Afk::Engine::get().event_manager)};
  lua_script.load(script_path, lua);
  this->loaded_files.emplace(script_path, std::move(lua_script));
  this->last_write.emplace(script_path, std::filesystem::last_write_time(script_path));
}
auto ScriptsComponent::remove_script(const path &script_path) -> void {
  this->loaded_files.erase(script_path);
}
auto ScriptsComponent::check_live_reload(lua_State *lua) -> void {
  for (auto &script : this->loaded_files) {
    const auto &script_path = script.first;
    if (std::filesystem::last_write_time(script_path) > this->last_write[script_path]) {
      script.second.unload();
      script.second.load(script_path, lua);
    }
  }
}
