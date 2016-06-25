entity_template = {
    name = "kenney_block_wood_platform",
    friendlyname = "[Kenney] Wood platform",

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
        ["platform"] = {
            platform_type = "Jumpthru",
        },
        ["platformer_hitbox"] = {
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
        ["render"] = {
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
