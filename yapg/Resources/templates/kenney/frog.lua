entity_template = {
    name = "kenney_frog",
    friendlyname = "[Kenney] Frog",

    parameters = {
        x = {
            name = "X position",
            component = "position",
            attribute = "x",
        },
        y = {
            name = "Y position",
            component = "position",
            attribute = "y",
        },
    },

    components = {
        ["position"] = {
            x = 0,
            y = 0,
            z = 100,
            width = 64,
            height = 64
        },
        ["platformer"] = {
            on_idle = function(entity)
                render(entity).current_animation = "idle"
            end,
            on_start_walking = function(entity)
                render(entity).current_animation = "jump"
                platformer(entity).wants_to_go_left = false
                platformer(entity).wants_to_go_right = false
            end,
            on_start_jumping = function(entity)
                render(entity).current_animation = "jump"
            end,
            on_start_falling = function(entity)
                render(entity).current_animation = "jump"
            end,
            on_turn_right = function(entity)
                render(entity).flipped = true
            end,
            on_turn_left = function(entity)
                render(entity).flipped = false
            end,
            acceleration = 10000000,
            deceleration = 10000000,
            max_walking_speed = 100,
            max_jumping_speed = 400,
        },
        ["custom_behavior"] = {
            on_update = function(entity, dt)
                if(entity:get_custom_data():has_value("old_pos_x")) then
                    local old_pos_x = as_float(entity:get_custom_data():get_value("old_pos_x"))

                    if((platformer(entity).wants_to_go_left or
                        platformer(entity).wants_to_go_right) and
                        old_pos_x == position(entity).x
                        ) then
                        local go_left = as_bool(entity:get_custom_data():get_value("go_left"))
                        entity:get_custom_data():set_value("go_left", bool_value(not go_left))
                    end
                end

                -- jump timer
                entity:get_custom_data():set_value(
                    "jump_timer",
                    float_value(as_float(entity:get_custom_data():get_value("jump_timer")) + dt)
                )

                if(as_float(entity:get_custom_data():get_value("jump_timer")) > 2) then
                    platformer(entity).wants_to_jump = true

                    -- The frog only moves when jumping
                    local go_left = as_bool(entity:get_custom_data():get_value("go_left"))
                    platformer(entity).wants_to_go_left = go_left
                    platformer(entity).wants_to_go_right = not go_left

                    entity:get_custom_data():set_value("jump_timer", float_value(0))
                else
                    platformer(entity).wants_to_jump = false
                end

                entity:get_custom_data():set_value("old_pos_x", float_value(position(entity).x))
            end,
        },
        ["custom_data"] = {
            go_left = bool_value(true),
            jump_timer = float_value(0),
        },
        ["health"] = {
            health = 1,
            max_health = 1,
        },
        ["platformer_hitbox"] = {
            polygon = {
                points = {
                    {
                        x = 0,
                        y = 32
                    },
                    {
                        x = 64,
                        y = 32
                    },
                    {
                        x = 64,
                        y = 64
                    },
                    {
                        x = 0,
                        y = 64
                    },
                }
            }
        },
        ["collidable"] = {
            polygons = {
                ["body"] = {
                    polygon = {
                        points = {
                            {
                                x = 0,
                                y = 38
                            },
                            {
                                x = 64,
                                y = 38
                            },
                            {
                                x = 64,
                                y = 64
                            },
                            {
                                x = 0,
                                y = 64
                            },
                        },
                    },
                    on_collision_begin = function(entity, other)
                        if(player(other) ~= nil and health(other) ~= nil) then
                            health(other):loose_pv(1)
                        end
                    end,
                },
                ["head"] = {
                    polygon = {
                        points = {
                            {
                                x = 0,
                                y = 32
                            },
                            {
                                x = 64,
                                y = 32
                            },
                            {
                                x = 64,
                                y = 38
                            },
                            {
                                x = 0,
                                y = 38
                            },
                        },
                    },
                    on_collision_begin = function(entity, other)
                        if(player(other) ~= nil and health(other) ~= nil) then
                            health(entity):kill()
                        end
                    end,
                }
            },
        },
        ["render"] = {
            texture = "spritesheet_complete.png",
            current_animation = "idle",
            animations = {
                ["idle"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 3380, top = 130, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
                ["jump"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 3250, top = 1820, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
