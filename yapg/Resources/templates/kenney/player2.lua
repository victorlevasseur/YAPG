entity_template = {
    name = "kenney_player2",
    friendlyname = "[Kenney] Player 2",
    inherits = "kenney_player1",

    components = {
        ["render"] = {
            animations = {
                ["idle"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 650, top = 516, width = 128, height = 256},
                            relative_duration = 1,
                        },
                    },
                },
                ["walking"] = {
                    total_duration = 0.2,
                    frames = {
                        {
                            rect = { left = 520, top = 1548, width = 128, height = 256},
                            relative_duration = 1,
                        },
                        {
                            rect = { left = 520, top = 1290, width = 128, height = 256},
                            relative_duration = 1,
                        },
                    },
                },
                ["jump"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 650, top = 774, width = 128, height = 256},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
