entity_template = {
    name = "kenney_player1",
    friendlyname = "Friendly name of template",

    parameters = {
        x = {
            name = "X position",
            component = "Position",
            type = "number",
            attribute = "x",
        },
        y = {
            name = "Y position",
            component = "Position",
            type = "number",
            attribute = "y",
        },
    },

    components = {
        ["Position"] = {
            x = 0,
            y = 0,
            z = 100,
            width = 64,
            height = 128
        },
        ["Platformer"] = {
            onIdle = function(entity)
                entity:set_string_attribute("Render", "current_animation", "idle")
            end,
            onStartWalking = function(entity)
                entity:set_string_attribute("Render", "current_animation", "walking")
            end,
            onStartJumping = function(entity)
                entity:set_string_attribute("Render", "current_animation", "jump")
            end,
            onStartFalling = function(entity)
                entity:set_string_attribute("Render", "current_animation", "jump")
            end,
            onTurnRight = function(entity)
                entity:set_bool_attribute("Render", "flipped", false)
            end,
            onTurnLeft = function(entity)
                entity:set_bool_attribute("Render", "flipped", true)
            end,
        },
        ["Hitbox"] = {
            polygon = {
                points = {
                    {
                        x = 12,
                        y = 50
                    },
                    {
                        x = 52,
                        y = 50
                    },
                    {
                        x = 52,
                        y = 128
                    },
                    {
                        x = 12,
                        y = 128
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
                            rect = { left = 390, top = 1290, width = 128, height = 256},
                            relative_duration = 1,
                        },
                    },
                },
                ["walking"] = {
                    total_duration = 0.2,
                    frames = {
                        {
                            rect = { left = 390, top = 516, width = 128, height = 256},
                            relative_duration = 1,
                        },
                        {
                            rect = { left = 390, top = 258, width = 128, height = 256},
                            relative_duration = 1,
                        },
                    },
                },
                ["jump"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 390, top = 1548, width = 128, height = 256},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
