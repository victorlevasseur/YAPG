#include "Rendering/Animation/Frame.hpp"

#include "Meta/Metadata.hpp"

namespace yapg
{

void Frame::registerClass()
{
    meta::MetadataStore::registerClass<Frame>()
        .declareAttribute("rect", &Frame::rect)
        .declareAttribute("relative_duration", &Frame::relativeDuration);
}

}
