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
            id = 1,
            template = "kenney_block_grass",
            values = {
                x = 80,
                y = 350,
            }
        },
        {
            id = 2,
            template = "kenney_block_grass",
            values = {
                x = 144,
                y = 350,
            }
        },
        {
            id = 3,
            template = "kenney_block_grass",
            values = {
                x = 208,
                y = 350,
            }
        },
        {
            id = 4,
            template = "kenney_block_grass",
            values = {
                x = 272,
                y = 350,
            }
        },
        {
            id = 5,
            template = "kenney_block_grass_center",
            values = {
                x = 80,
                y = 414,
            }
        },
        {
            id = 6,
            template = "kenney_block_grass_center",
            values = {
                x = 144,
                y = 414,
            }
        },
        {
            id = 7,
            template = "kenney_block_grass_center",
            values = {
                x = 208,
                y = 414,
            }
        },
        {
            id = 8,
            template = "kenney_block_grass_center",
            values = {
                x = 272,
                y = 414,
            }
        },
        {
            id = 9,
            template = "kenney_block_wood_platform",
            values = {
                x = 144,
                y = 230,
            }
        },
        {
            id = 10,
            template = "kenney_block_wood_platform",
            values = {
                x = 208,
                y = 230,
            }
        },
    }
}
