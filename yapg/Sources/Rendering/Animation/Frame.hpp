#ifndef YAPG_GAME_ANIMATION_FRAME_H
#define YAPG_GAME_ANIMATION_FRAME_H

#include <SFML/Graphics/Rect.hpp>

namespace animation
{

struct Frame
{
    sf::IntRect rect;
    float relativeDuration = 1.f;

    static void registerClass();
};

}

#endif
