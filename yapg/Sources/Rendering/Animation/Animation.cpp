#include "Rendering/Animation/Animation.hpp"

#include "Meta/Metadata.hpp"

namespace yapg
{

Animation::Animation() :
    m_frames(),
    m_totalDuration(0.f)
{

}

Animation::Animation(const std::vector<Frame>& frames) :
    m_frames(frames),
    m_totalDuration(0.f)
{

}

Animation::Animation(std::vector<Frame>&& frames) :
    m_frames(std::move(frames)),
    m_totalDuration(0.f)
{

}

void Animation::normalizeFramesDurations()
{
    float totalRelativeDuration = 0.f;
    for(auto it = m_frames.cbegin(); it != m_frames.cend(); ++it)
    {
        totalRelativeDuration += it->relativeDuration;
    }

    //Divide by the total relative duration to get a total relative duration = 1
    for(auto it = m_frames.begin(); it != m_frames.end(); ++it)
    {
        it->relativeDuration /= totalRelativeDuration;
    }
}

const Frame& Animation::getFrameAt(float t) const
{
    float endOfFrame = 0.f;
    auto it = m_frames.cbegin();
    while(endOfFrame <= t && it != m_frames.cend())
    {
        endOfFrame += it->relativeDuration * m_totalDuration;
        ++it;
    }

    return *(--it);
}

Frame& Animation::getFrameAt(float t)
{
    float endOfFrame = 0.f;
    auto it = m_frames.begin();
    while(endOfFrame <= t && it != m_frames.cend())
    {
        endOfFrame += it->relativeDuration * m_totalDuration;
        ++it;
    }

    return *(--it);
}

void Animation::registerClass()
{
    meta::MetadataStore::registerClass<Animation>()
        .declareAttribute("total_duration", &Animation::m_totalDuration)
        .declareAttribute("frames", &Animation::m_frames)
        .setExtraLoadFunction([](Animation* anim, const sol::object& luaObject)
        {
            anim->normalizeFramesDurations();
        });
}

}
