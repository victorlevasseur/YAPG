entity_template = {
    name = "kenney_block_grass",
    friendlyname = "[Kenney] Grass block",

    parameters = {
        x = {
            name = "X position",
            component = "Position",
            type = "number",
            attribute = "x",
        },
        y = {
            name = "Y position",
            component = "Position",
            type = "number",
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
            current_animation = "default",
            animations = {
                ["default"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 1560, top = 390, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
