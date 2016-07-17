entity_template = {
    name = "yapg_player1",
    friendlyname = "[YAPG] Player 1",

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
            width = 32,
            height = 64
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
                                x = 4*2,
                                y = 3*2
                            },
                            {
                                x = (4+7)*2,
                                y = 3*2
                            },
                            {
                                x = (4+7)*2,
                                y = 31*2
                            },
                            {
                                x = 4*2,
                                y = 31*2
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
                        x = 4*2,
                        y = 3*2
                    },
                    {
                        x = (4+7)*2,
                        y = 3*2
                    },
                    {
                        x = (4+7)*2,
                        y = 31*2
                    },
                    {
                        x = 4*2,
                        y = 31*2
                    },
                }
            }
        },
        ["render"] = {
            texture = "yapg/player1.png",
            current_animation = "idle",
            animations = {
                ["idle"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 0, top = 0, width = 32, height = 64},
                            relative_duration = 1,
                        },
                    },
                },
                ["walking"] = {
                    total_duration = 0.5,
                    frames = {
                        {
                            rect = { left = 32, top = 0, width = 32, height = 64},
                            relative_duration = 1,
                        },
                        {
                            rect = { left = 0, top = 0, width = 32, height = 64},
                            relative_duration = 1,
                        },
                        {
                            rect = { left = 64, top = 0, width = 32, height = 64},
                            relative_duration = 1,
                        },
                        {
                            rect = { left = 0, top = 0, width = 32, height = 64},
                            relative_duration = 1,
                        },
                    },
                },
                ["jump"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 64, top = 0, width = 32, height = 64},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
