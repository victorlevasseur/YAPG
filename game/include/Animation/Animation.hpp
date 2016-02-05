#ifndef YAPG_GAME_ANIMATION_ANIMATION_H
#define YAPG_GAME_ANIMATION_ANIMATION_H

#include "Animation/Frame.hpp"

namespace animation
{

class Animation
{
public:
    Animation();

    void normalizeFramesDurations();

    const Frame& getFrameAt(float t) const;
    Frame& getFrameAt(float t);

    float getDuration() const { return m_totalDuration; }

    static void registerClass();

private:
    std::vector<Frame> m_frames;
    float m_totalDuration;
};

}

#endif
