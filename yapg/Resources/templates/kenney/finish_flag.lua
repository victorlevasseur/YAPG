entity_template = {
    name = "kenney_finish_flag",
    friendlyname = "[Kenney] Finish flag",

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
            width = 64,
            height = 64
        },
        ["finish_line"] = {

        },
        ["collidable"] = {
            polygons = {
                ["all"] = {
                    polygon = {
                        points = {
                            {
                                x = 0,
                                y = 0
                            },
                            {
                                x = 24,
                                y = 0
                            },
                            {
                                x = 24,
                                y = 64
                            },
                            {
                                x = 0,
                                y = 64
                            },
                        },
                    },
                    on_collision_begin = function(entity, other)
                        if(player(other) ~= nil) then
                            player(other):set_finished_level()
                        end
                    end,
                },
            },
        },
        ["render"] = {
            texture = "spritesheet_complete.png",
            current_animation = "normal",
            animations = {
                ["normal"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 2600, top = 1040, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
                ["finished"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 2600, top = 1300, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
