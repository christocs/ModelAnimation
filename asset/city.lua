--  const auto city_entity     = registry.create();
--   auto city_transform        = Transform{city_entity};
--   city_transform.scale       = vec3{0.25f};
--   city_transform.translation = vec3{0.0f, -1.0f, 0.0f};
--   registry.assign<Afk::Transform>(city_entity, city_transform);
--   registry.assign<Afk::ModelSource>(city_entity, city_entity, "res/model/city/city.fbx");
--   registry.assign<Afk::PhysicsBody>(city_entity, city_entity, &this->physics_body_system,
--                                     city_transform, 0.0f, 0.0f, 0.0f, 0.0f,
--                                     false, Afk::RigidBodyType::STATIC,
--                                     Afk::Box{100000000.0f, 0.1f, 100000000.0f});

type = asset.object
components = {
    transform = {
        x = 0, y = -1, z = 0,
        x_scale = 0.25, y_scale = 0.25, z_scale = 0.25
    },
    phys = { --0.3f, 0.2f, 0.0f, 30.0f, true,
        bounciness = 0.0, 
        linear_damping = 0.0,
        angular_damping = 0.0,
        mass = 0.0,
        gravity = false,
        body_type = rigidbody.static,
        shape = {
            type = shape.box,
            x = 100000000,
            y = 0.1,
            z = 100000000
        }
    },
    model = {
        path = "res/model/city/city.fbx"
    }
}
