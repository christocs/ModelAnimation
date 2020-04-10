#pragma once

#include <glm/glm.hpp>

#include "afk/event/EventManager.hpp"
#include "afk/renderer/Camera.hpp"
#include "afk/renderer/Renderer.hpp"
#include "afk/ui/Ui.hpp"

#include <reactphysics3d.h>
#include <entt/entt.hpp>

namespace Afk {
  class Engine {
  public:
    static constexpr const char *GAME_NAME = "ICT397";

    Renderer renderer          = {};
    Ui ui                      = {this->renderer.window};
    Camera camera              = {};
    EventManager event_manager = {};

    Engine();
    Engine(Engine &&)      = delete;
    Engine(const Engine &) = delete;
    auto operator=(const Engine &) -> Engine & = delete;
    auto operator=(Engine &&) -> Engine & = delete;

    static auto get() -> Engine &;

    auto render() -> void;
    auto update() -> void;

    auto get_time() -> float;
    auto get_delta_time() -> float;
    auto get_is_running() const -> bool;

  private:
    auto update_camera() -> void;
    auto move_keyboard(Event e) -> void;
    auto move_mouse(Event e) -> void;
    auto handle_mouse() -> void;

    bool is_running   = true;
    int frame_count   = {};
    float last_update = {};

    entt::registry registry;

    rp3d::DynamicsWorld* world;

    rp3d::RigidBody* cityBody;
    rp3d::ProxyShape* cityShape;

    rp3d::RigidBody* basketballBody;
    rp3d::ProxyShape* basketballShape;

    rp3d::SphereShape sphereShape = rp3d::SphereShape(0.8f);
    rp3d::BoxShape boxShape = rp3d::BoxShape(rp3d::Vector3(100000000.0f, 0.1f, 100000000.0f));
  };
}
