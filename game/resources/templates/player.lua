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

                entity:write_to_console("Idle ! in anim " .. entity:get_component("Render"):get_string("default_animation"))
            end,
            onStartWalking = function(entity)
                entity:write_to_console("Start walking !")
            end,
            onStartJumping = function(entity)
                entity:write_to_console("Jump !")
            end,
            onStartFalling = function(entity)
                entity:write_to_console("Start falling !")
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
            default_animation = "default",
            animations = {
                ["default"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 390, top = 1290, width = 128, height = 256},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
