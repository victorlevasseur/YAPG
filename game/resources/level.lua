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
                x = 1*64,
                y = 6*64,
            }
        },
        {
            id = 2,
            template = "kenney_block_grass",
            values = {
                x = 2*64,
                y = 6*64,
            }
        },
        {
            id = 3,
            template = "kenney_block_grass",
            values = {
                x = 3*64,
                y = 6*64,
            }
        },
        {
            id = 4,
            template = "kenney_block_grass",
            values = {
                x = 4*64,
                y = 6*64,
            }
        },
        {
            id = 5,
            template = "kenney_block_grass_center",
            values = {
                x = 1*64,
                y = 7*64,
            }
        },
        {
            id = 6,
            template = "kenney_block_grass_center",
            values = {
                x = 2*64,
                y = 7*64,
            }
        },
        {
            id = 7,
            template = "kenney_block_grass_center",
            values = {
                x = 3*64,
                y = 7*64,
            }
        },
        {
            id = 8,
            template = "kenney_block_grass_center",
            values = {
                x = 4*64,
                y = 7*64,
            }
        },
        {
            id = 9,
            template = "kenney_block_wood_platform",
            values = {
                x = 2*64,
                y = 4.5*64,
            }
        },
        {
            id = 10,
            template = "kenney_block_wood_platform",
            values = {
                x = 3*64,
                y = 4.5*64,
            }
        },
        {
            id = 11,
            template = "kenney_block_grass_center",
            values = {
                x = 1*64,
                y = 11*64,
            }
        },
        {
            id = 12,
            template = "kenney_block_grass_center",
            values = {
                x = 2*64,
                y = 11*64,
            }
        },
        {
            id = 13,
            template = "kenney_block_grass_center",
            values = {
                x = 3*64,
                y = 11*64,
            }
        },
        {
            id = 14,
            template = "kenney_block_grass_center",
            values = {
                x = 4*64,
                y = 11*64,
            }
        },
        {
            id = 15,
            template = "kenney_block_grass_center",
            values = {
                x = 5*64,
                y = 11*64,
            }
        },
        {
            id = 16,
            template = "kenney_block_grass_center",
            values = {
                x = 6*64,
                y = 11*64,
            }
        },
        {
            id = 17,
            template = "kenney_block_grass_center",
            values = {
                x = 7*64,
                y = 11*64,
            }
        },
        {
            id = 18,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 11*64,
            }
        },
        {
            id = 19,
            template = "kenney_block_grass_center",
            values = {
                x = 9*64,
                y = 11*64,
            }
        },
        {
            id = 20,
            template = "kenney_block_grass_center",
            values = {
                x = 10*64,
                y = 11*64,
            }
        },
        {
            id = 21,
            template = "kenney_block_grass_center",
            values = {
                x = 11*64,
                y = 11*64,
            }
        },
        {
            id = 22,
            template = "kenney_block_grass_center",
            values = {
                x = 12*64,
                y = 11*64,
            }
        },
        {
            id = 23,
            template = "kenney_block_grass_center",
            values = {
                x = 1*64,
                y = 10*64,
            }
        },
        {
            id = 24,
            template = "kenney_block_grass_center",
            values = {
                x = 1*64,
                y = 9*64,
            }
        },
        {
            id = 25,
            template = "kenney_block_grass_center",
            values = {
                x = 1*64,
                y = 8*64,
            }
        },
        {
            id = 26,
            template = "kenney_block_grass_center",
            values = {
                x = 13*64,
                y = 11*64,
            }
        },
        {
            id = 27,
            template = "kenney_block_grass_center",
            values = {
                x = 14*64,
                y = 11*64,
            }
        },
        {
            id = 28,
            template = "kenney_block_grass_center",
            values = {
                x = 14*64,
                y = 10*64,
            }
        },
        {
            id = 29,
            template = "kenney_block_grass_center",
            values = {
                x = 14*64,
                y = 9*64,
            }
        },
        {
            id = 30,
            template = "kenney_block_grass_center",
            values = {
                x = 14*64,
                y = 8*64,
            }
        },
        {
            id = 31,
            template = "kenney_block_wood_platform",
            values = {
                x = 5*64,
                y = 6*64,
            }
        },
        {
            id = 31,
            template = "kenney_block_wood_platform",
            values = {
                x = 7*64,
                y = 6*64,
            }
        },
        {
            id = 31,
            template = "kenney_block_wood_platform",
            values = {
                x = 6*64,
                y = 7*64,
            }
        },
        {
            id = 32,
            template = "kenney_block_wood_platform",
            values = {
                x = 5*64,
                y = 10*64,
            }
        },
        {
            id = 35,
            template = "kenney_block_wood_platform",
            values = {
                x = 6*64,
                y = 9*64,
            }
        },
        {
            id = 36,
            template = "kenney_block_wood_platform",
            values = {
                x = 7*64,
                y = 9*64,
            }
        },
        {
            id = 37,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 8*64,
            }
        },
        {
            id = 38,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 7*64,
            }
        },
        {
            id = 39,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 6*64,
            }
        },
        {
            id = 40,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 5*64,
            }
        },
        {
            id = 41,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 4*64,
            }
        },
        {
            id = 42,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 3*64,
            }
        },
        {
            id = 43,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 2*64,
            }
        },
        {
            id = 44,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 1*64,
            }
        },
        {
            id = 45,
            template = "kenney_block_grass_center",
            values = {
                x = 8*64,
                y = 0*64,
            }
        },
        {
            id = 46,
            template = "kenney_block_grass_center",
            values = {
                x = 14*64,
                y = 7*64,
            }
        },
        {
            id = 47,
            template = "kenney_block_grass",
            values = {
                x = 14*64,
                y = 6*64,
            }
        },
        {
            id = 48,
            template = "kenney_block_grass",
            values = {
                x = 10*64,
                y = 10*64,
            }
        },
        {
            id = 49,
            template = "kenney_block_grass_slope_right",
            values = {
                x = 11*64,
                y = 9*64,
            }
        },
        {
            id = 50,
            template = "kenney_block_grass_slope_right",
            values = {
                x = 12*64,
                y = 8*64,
            }
        },
        {
            id = 51,
            template = "kenney_block_grass_slope_right",
            values = {
                x = 13*64,
                y = 7*64,
            }
        },
        {
            id = 52,
            template = "kenney_block_grass_slope_right_center",
            values = {
                x = 11*64,
                y = 10*64,
            }
        },
        {
            id = 53,
            template = "kenney_block_grass_slope_right_center",
            values = {
                x = 12*64,
                y = 9*64,
            }
        },
        {
            id = 53,
            template = "kenney_block_grass_slope_right_center",
            values = {
                x = 13*64,
                y = 8*64,
            }
        },
        {
            id = 54,
            template = "kenney_block_grass_center",
            values = {
                x = 13*64,
                y = 9*64,
            }
        },
        {
            id = 55,
            template = "kenney_block_grass_center",
            values = {
                x = 13*64,
                y = 10*64,
            }
        },
        {
            id = 56,
            template = "kenney_block_grass_center",
            values = {
                x = 12*64,
                y = 10*64,
            }
        },
        {
            id = 57,
            template = "kenney_block_grass_slope_right",
            values = {
                x = 9*64,
                y = 10*64,
            }
        },
        {
            id = 58,
            template = "kenney_block_wood_platform",
            values = {
                x = 9*64,
                y = 4.5*64,
            }
        },
        {
            id = 59,
            template = "kenney_block_wood_platform",
            values = {
                x = 10*64,
                y = 4.5*64,
            }
        },
        {
            id = 60,
            template = "kenney_block_wood_platform",
            values = {
                x = 11*64,
                y = 4.5*64,
            }
        },
        {
            id = 61,
            template = "kenney_block_wood_platform",
            values = {
                x = 12*64,
                y = 4.5*64,
            }
        },
    }
}
