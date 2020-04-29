#include "ScriptsComponent.hpp"

#include <string>

#include "afk/Afk.hpp"
#include "afk/io/Log.hpp"
#include "afk/io/Path.hpp"

using Afk::ScriptsComponent;

ScriptsComponent::ScriptsComponent() : loaded_files(), last_write() {}

auto ScriptsComponent::add_script(const path &script_path, lua_State *lua,
                                  EventManager *evt_mgr) -> void {
  const auto abs_path = Afk::get_absolute_path(script_path);
  Afk::status << script_path.string() << "\n";
  Afk::status << abs_path.string() << "\n";
  auto lua_script = LuaScript{evt_mgr};
  lua_script.load(abs_path, lua);
  this->loaded_files.emplace(abs_path, std::move(lua_script));
  this->last_write.emplace(abs_path, std::filesystem::last_write_time(abs_path));
}
auto ScriptsComponent::remove_script(const path &script_path) -> void {
  const auto abs_path = Afk::get_absolute_path(script_path);
  this->loaded_files.erase(abs_path);
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
