function update(evt)
    local upd8 = evt:to_update()
    local speed = 10.0
    local dt = upd8.delta
    local vel = speed * dt
    local cur_cam = camera.current()
    local cur_cam_pos = cur_cam:get_pos()
    if keyboard.is_pressed(key.W) then
        cur_cam:set_pos(cur_cam_pos:vec_add(cur_cam:front():scalar_mul(vel)))
    end
    if keyboard.is_pressed(key.S) then
        cur_cam:set_pos(cur_cam_pos:vec_sub(cur_cam:front():scalar_mul(vel)))
    end
    if keyboard.is_pressed(key.A) then
        cur_cam:set_pos(cur_cam_pos:vec_sub(cur_cam:right():scalar_mul(vel)))
    end
    if keyboard.is_pressed(key.D) then
        cur_cam:set_pos(cur_cam_pos:vec_add(cur_cam:right():scalar_mul(vel)))
    end
end

this:register_event(event.update, update)
