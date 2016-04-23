entity_template = {
    name = "kenney_player1",
    friendlyname = "[Kenney] Player 1",

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
            height = 128
        },
        ["Platformer"] = {
            on_idle = function(entity)
                entity.render.current_animation = "idle"
            end,
            on_start_walking = function(entity)
                entity.render.current_animation = "walking"
            end,
            on_start_jumping = function(entity)
                entity.render.current_animation = "jump"
            end,
            on_start_falling = function(entity)
                entity.render.current_animation = "jump"
            end,
            on_turn_right = function(entity)
                entity.render.flipped = false
            end,
            on_turn_left = function(entity)
                entity.render.flipped = true
            end,
        },
        ["Player"] = {

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
        ["Collider"] = {

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
