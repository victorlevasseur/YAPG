entity_template = {
    name = "kenney_player1",
    friendlyname = "[Kenney] Player 1",

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
            height = 128
        },
        ["platformer"] = {
            on_idle = function(entity)
                render(entity).current_animation = "idle"
            end,
            on_start_walking = function(entity)
                render(entity).current_animation = "walking"
            end,
            on_start_jumping = function(entity)
                render(entity).current_animation = "jump"
            end,
            on_start_falling = function(entity)
                render(entity).current_animation = "jump"
            end,
            on_turn_right = function(entity)
                render(entity).flipped = false
            end,
            on_turn_left = function(entity)
                render(entity).flipped = true
            end,
        },
        ["player"] = {

        },
        ["health"] = {
            health = 1,
            max_health = 1
        },
        ["collidable"] = {
            polygons = {
                ["body"] = {
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
                        },
                    },
                },
            },
        },
        ["platformer_hitbox"] = {
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
        ["render"] = {
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
