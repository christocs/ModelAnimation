type = asset.object
components = {
    -- script = {
    --     "script/basketball.lua"
    -- },
    transform = {
        x = 0, y = 100, z = 0
    },
    phys = { --0.3f, 0.2f, 0.0f, 30.0f, true,
        bounciness = 0.3, 
        linear_damping = 0.2,
        angular_damping = 0.0,
        mass = 30.0,
        gravity = true,
        body_type = rigidbody.dynamic,
        shape = {
            type = shape.sphere,
            r = 1.4--sphere is type = shape.sphere, r = <radius>
        }
    },
    model = {
        path = "res/model/basketball/basketball.fbx"
    }
}
