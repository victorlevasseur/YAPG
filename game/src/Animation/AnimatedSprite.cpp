#include "Animation/AnimatedSprite.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace animation
{

AnimatedSprite::AnimatedSprite(std::shared_ptr<sf::Texture>& texture, const std::map<std::string, Animation>& animations) :
    sf::Drawable(),
    sf::Transformable(),
    m_texture(texture),
    m_animations(animations),
    m_currentAnimation(),
    m_currentTime(0.f),
    m_vertices(sf::Quads, 4)
{
    m_vertices[0].position = sf::Vector2f(0.f, 0.f);
    m_vertices[1].position = sf::Vector2f(1.f, 0.f);
    m_vertices[2].position = sf::Vector2f(1.f, 1.f);
    m_vertices[3].position = sf::Vector2f(0.f, 1.f);
}

std::string AnimatedSprite::getCurrentAnimation() const
{
    return m_currentAnimation;
}

void AnimatedSprite::setCurrentAnimation(const std::string& name)
{
    m_currentAnimation = name;
    m_currentTime = 0.f;
}

void AnimatedSprite::update(float dt)
{
    m_currentTime += dt;
    while(m_currentTime >= m_animations.at(m_currentAnimation).getDuration())
        m_currentTime -= m_animations.at(m_currentAnimation).getDuration();

    //Update the picture according to the animation
    const sf::IntRect& frameRect =
        m_animations.at(m_currentAnimation).getFrameAt(m_currentTime).rect;
    m_vertices[0].texCoords = sf::Vector2f(frameRect.left, frameRect.top);
    m_vertices[1].texCoords = sf::Vector2f(frameRect.left + frameRect.width, frameRect.top);
    m_vertices[2].texCoords = sf::Vector2f(frameRect.left + frameRect.width, frameRect.top + frameRect.height);
    m_vertices[3].texCoords = sf::Vector2f(frameRect.left, frameRect.top + frameRect.height);
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = m_texture.get();
    states.transform *= getTransform();
    target.draw(m_vertices, states);
}

}
