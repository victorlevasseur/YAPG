entity_template = {
    name = "kenney_block_grass_center",
    friendlyname = "[Kenney] Center grass block",

    inherits = "kenney_block_grass",

    components = {
        ["render"] = {
            current_animation = "default",
            animations = {
                ["default"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 1690, top = 390, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
