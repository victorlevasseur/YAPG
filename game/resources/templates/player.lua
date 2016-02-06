player = {
    friendlyname = "Friendly name of template",

    components = {
        ["Position"] = {
            x = 0,
            y = 0,
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
        },
        ["Hitbox"] = {
            polygon = {
                points = {
                    {
                        x = 0,
                        y = 0
                    },
                    {
                        x = 64,
                        y = 0
                    },
                    {
                        x = 64,
                        y = 128
                    },
                    {
                        x = 0,
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
