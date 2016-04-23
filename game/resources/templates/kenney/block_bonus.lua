entity_template = {
    name = "kenney_block_bonus",
    friendlyname = "[Kenney] Bonus block",

    inherits = "kenney_block_grass",

    parameters = {
        initial_state = {
            name = "Initial state (normal or hit)",
            component = "Render",
            attribute = "current_animation",
        },
    },

    components = {
        ["Collidable"] = {
            on_collision_begin = function(entity, other)
                -- Test if the block has not been hit and if the collision is made with a player
                if entity.render.current_animation == "normal" and other.player ~= nil then
                    -- Get some useful positions (the Y bottom coord of the block and the Y top coord of the player's HITBOX)
                    local block_bottom_pos = entity.position.y + entity.position.height
                    local player_top_hitbox_pos = other.position.y + other.hitbox.polygon:get_local_bounding_box().top

                    -- Test if the player hit the block from under
                    if player_top_hitbox_pos >= block_bottom_pos then
                        entity.render.current_animation = "hit"
                    end
                end
            end,
        },
        ["Render"] = {
            current_animation = "normal",
            animations = {
                ["normal"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 2340, top = 780, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
                ["hit"] = {
                    total_duration = 1,
                    frames = {
                        {
                            rect = { left = 2340, top = 520, width = 128, height = 128},
                            relative_duration = 1,
                        },
                    },
                },
            },
        }
    }
}
