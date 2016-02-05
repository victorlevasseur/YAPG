block = {
    friendlyname = "Friendly name of template",

    components = {
        ["Position"] = {
            x = 0,
            y = 0,
            width = 100,
            height = 100
        },
        ["Platform"] = {

        },
        ["Hitbox"] = {
            polygon = {
                points = {
                    {
                        x = 0,
                        y = 0
                    },
                    {
                        x = 100,
                        y = 0
                    },
                    {
                        x = 100,
                        y = 100
                    },
                    {
                        x = 0,
                        y = 100
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
                            rect = { left = 1690, top = 780, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
