player = {
    friendlyname = "Friendly name of template",

    components = {
        ["Position"] = {
            x = 0,
            y = 0,
            width = 100,
            height = 100
        },
        ["Platformer"] = {
            onIdle = function(entity)
                entity:write_to_console("Idle !")
            end,
            onStartWalking = function(entity)
                entity:write_to_console("Start walking !")
            end,
            onStartJumping = function(entity)
                entity:write_to_console("Jump !")
            end,
            onStartFalling = function(entity)
                entity:write_to_console("Start falling !")
            end,
        },
        ["Hitbox"] = {
            polygon = {
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
        },
    }
}
