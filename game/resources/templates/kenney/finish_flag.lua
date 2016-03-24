entity_template = {
    name = "kenney_finish_flag",
    friendlyname = "[Kenney] Finish flag",

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
            width = 64,
            height = 64
        },
        ["FinishLine"] = {

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
