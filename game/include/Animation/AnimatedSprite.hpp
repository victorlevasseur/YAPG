#ifndef YAPG_GAME_ANIMATION_ANIMATEDSPRITE_H
#define YAPG_GAME_ANIMATION_ANIMATEDSPRITE_H

#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Animation/Animation.hpp"

namespace animation
{

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
    AnimatedSprite(std::shared_ptr<sf::Texture> texture, const std::map<std::string, Animation>& animations);

    std::string getCurrentAnimation() const;
    void setCurrentAnimation(const std::string& name);

    void update(float dt);

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::shared_ptr<sf::Texture> m_texture;

    const std::map<std::string, Animation>& m_animations;
    std::string m_currentAnimation;
    float m_currentTime;

    sf::VertexArray m_vertices;
};

}

#endif
