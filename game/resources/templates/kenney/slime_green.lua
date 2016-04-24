entity_template = {
    name = "kenney_slime_green",
    friendlyname = "[Kenney] Green Slime",

    parameters = {
        x = {
            name = "X position",
            component = "Position",
            attribute = "x",
        },
        y = {
            name = "Y position",
            component = "Position",
            attribute = "y",
        },
    },

    components = {
        ["Position"] = {
            x = 0,
            y = 0,
            z = 100,
            width = 64,
            height = 64
        },
        ["Platformer"] = {
            on_idle = function(entity)
                render(entity).current_animation = "idle&jump"
            end,
            on_start_walking = function(entity)
                render(entity).current_animation = "walking"
            end,
            on_start_jumping = function(entity)
                render(entity).current_animation = "idle&jump"
            end,
            on_start_falling = function(entity)
                render(entity).current_animation = "idle&jump"
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
        },
        ["CustomBehavior"] = {
            on_update = function(entity, dt)
                if(entity:get_custom_data():has_value("old_pos_x")) then
                    local old_pos_x = as_float(entity:get_custom_data():get_value("old_pos_x"))

                    if(old_pos_x == position(entity).x) then
                        local go_left = as_bool(entity:get_custom_data():get_value("go_left"))
                        entity:get_custom_data():set_value("go_left", bool_value(not go_left))
                    end
                end

                local go_left = as_bool(entity:get_custom_data():get_value("go_left"))
                platformer(entity).wants_to_go_left = go_left
                platformer(entity).wants_to_go_right = not go_left

                entity:get_custom_data():set_value("old_pos_x", float_value(position(entity).x))
            end,
        },
        ["CustomData"] = {
            go_left = bool_value(true)
        },
        ["Hitbox"] = {
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
        ["Render"] = {
            texture = "spritesheet_complete.png",
            current_animation = "idle&jump",
            animations = {
                ["idle&jump"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 3120, top = 1170, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
                ["walking"] = {
                    total_duration = 0.2,
                    frames = {
                        {
                            rect = { left = 3120, top = 1170, width = 128, height = 128},
                            relative_duration = 1,
                        },
                        {
                            rect = { left = 3120, top = 780, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
