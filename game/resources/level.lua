level = {
    spawn_position = { -- The position of the spawn.
        x = 120,
        y = 80,
    },
    max_player = 1, --The maximum number of players of the level
    players = { --The player entities of the level (the first one will be the player 1, ...), need at least as many as the max_player count.
        "kenney_player1",
    },
    objects = { --The entities template instances with their parameter fulfilled
        {
            id = 2,
            template = "kenney_block",
            values = {
                x = 80,
                y = 350,
            }
        },
        {
            id = 3,
            template = "kenney_block",
            values = {
                x = 144,
                y = 350,
            }
        },
        {
            id = 4,
            template = "kenney_slope45",
            values = {
                x = 208,
                y = 286,
            }
        },
        {
            id = 5,
            template = "kenney_block",
            values = {
                x = 272,
                y = 286,
            }
        },
        {
            id = 6,
            template = "kenney_block",
            values = {
                x = 336,
                y = 222,
            }
        },
    }
}
