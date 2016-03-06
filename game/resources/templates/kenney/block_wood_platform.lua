entity_template = {
    name = "kenney_block_wood_platform",
    friendlyname = "[Kenney] Wood platform",

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
        ["Platform"] = {
            platform_type = "Jumpthru",
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
                        y = 16
                    },
                    {
                        x = 0,
                        y = 16
                    },
                }
            }
        },
        ["Render"] = {
            texture = "spritesheet_complete.png",
            current_animation = "default",
            animations = {
                ["default"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 2340, top = 0, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
