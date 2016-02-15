#ifndef YAPG_GAME_ANIMATION_ANIMATION_H
#define YAPG_GAME_ANIMATION_ANIMATION_H

#include "Animation/Frame.hpp"

namespace animation
{

class Animation
{
public:
    Animation();
    Animation(const std::vector<Frame>& frames);
    Animation(std::vector<Frame>&& frames);

    void normalizeFramesDurations();

    const Frame& getFrameAt(float t) const;
    Frame& getFrameAt(float t);

    float getDuration() const { return m_totalDuration; }
    void setDuration(float duration) { m_totalDuration = duration; normalizeFramesDurations(); }

    static void registerClass();

private:
    std::vector<Frame> m_frames;
    float m_totalDuration;
};

}

#endif
