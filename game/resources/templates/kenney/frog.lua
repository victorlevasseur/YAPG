entity_template = {
    name = "kenney_frog",
    friendlyname = "[Kenney] Frog",

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
                entity:set_attribute("Render", "current_animation", string_value("idle"))
            end,
            on_start_walking = function(entity)
                entity:set_attribute("Render", "current_animation", string_value("jump"))
                entity:set_attribute("Platformer", "wants_to_go_left", bool_value(false))
                entity:set_attribute("Platformer", "wants_to_go_right", bool_value(false))
            end,
            on_start_jumping = function(entity)
                entity:set_attribute("Render", "current_animation", string_value("jump"))
            end,
            on_start_falling = function(entity)
                entity:set_attribute("Render", "current_animation", string_value("jump"))
            end,
            on_turn_right = function(entity)
                entity:set_attribute("Render", "flipped", bool_value(true))
            end,
            on_turn_left = function(entity)
                entity:set_attribute("Render", "flipped", bool_value(false))
            end,
            acceleration = 10000000,
            deceleration = 10000000,
            max_walking_speed = 100,
            max_jumping_speed = 400,
        },
        ["CustomBehavior"] = {
            on_update = function(entity, dt)
                if(entity:get_custom_data():has_value("old_pos_x")) then
                    local old_pos_x = as_float(entity:get_custom_data():get_value("old_pos_x"))

                    if((as_bool(entity:get_attribute("Platformer", "wants_to_go_left")) or
                        as_bool(entity:get_attribute("Platformer", "wants_to_go_right"))) and
                        old_pos_x == as_float(entity:get_attribute("Position", "x"))
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
                    entity:set_attribute("Platformer", "wants_to_jump", bool_value(true))

                    -- The frog only moves when jumping
                    local go_left = as_bool(entity:get_custom_data():get_value("go_left"))
                    entity:set_attribute("Platformer", "wants_to_go_left", bool_value(go_left))
                    entity:set_attribute("Platformer", "wants_to_go_right", bool_value(not go_left))

                    entity:get_custom_data():set_value("jump_timer", float_value(0))
                else
                    entity:set_attribute("Platformer", "wants_to_jump", bool_value(false))
                end

                entity:get_custom_data():set_value("old_pos_x", entity:get_attribute("Position", "x"))
            end,
        },
        ["CustomData"] = {
            go_left = bool_value(true),
            jump_timer = float_value(0),
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
