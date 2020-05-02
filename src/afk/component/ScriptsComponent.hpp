#include <filesystem>
#include <map>
#include <memory>

#include "afk/component/BaseComponent.hpp"
#include "afk/component/LuaScript.hpp"

using std::filesystem::path;

namespace Afk {
  /**
   * Script Component v2
   * Gets around ENTT's limitation of one component of a type per entity.
   * (With other ECS like systems, each unique script would get it's own type)
   * (However, we can't really do this when it's all in Lua)
   * (So all the scripts are bundled into one ScriptsComponent)
   */
  class ScriptsComponent : public BaseComponent {
  public:
    /**
     * Create an owned ScriptComponent
     * @param e owner
     */
    ScriptsComponent(GameObject e);
    /**
     * Check to see if files loaded by this are ready for a live reload,
     * and reload them if they are.
     * @param l lua state
     */
    auto check_live_reload(lua_State *l) -> void;
    /**
     * add a script to the component
     * @param script_path script path
     * @param l lua state
     * @param evt event manager
     * @return self, for chained method calls
     */
    auto add_script(const path &script_path, lua_State *l, EventManager *evt)
        -> ScriptsComponent &;
    /**
     * remove a script from this component
     * @param script_path path of script to remove
     */
    auto remove_script(const path &script_path) -> void;

  private:
    std::map<path, std::shared_ptr<Afk::LuaScript>> loaded_files;
    std::map<path, std::filesystem::file_time_type> last_write;
  };
}
