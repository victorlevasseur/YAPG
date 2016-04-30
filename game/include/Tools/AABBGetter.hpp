#ifndef YAPG_GAME_TOOLS_AABBGETTER_H
#define YAPG_GAME_TOOLS_AABBGETTER_H

#include <SFML/Graphics/Rect.hpp>

namespace tools
{

template<class T>
class AABBGetter
{
public:
    static sf::FloatRect getAABB(const T& value)
    {
        return value.getGlobalBounds();
    }
};

template<>
class AABBGetter<sf::Vector2f>
{
public:
    static sf::FloatRect getAABB(const sf::Vector2f& value)
    {
        return sf::FloatRect(value, sf::Vector2f(1.f, 1.f));
    }
};

}

#endif
