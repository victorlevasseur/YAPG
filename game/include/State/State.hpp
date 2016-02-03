#ifndef YAPG_GAME_STATE_STATE_H
#define YAPG_GAME_STATE_STATE_H

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>


namespace state
{

class State
{
public:
    using Ptr = std::unique_ptr<State>;
    using NonOwningPtr = State*;

    State();

    virtual void onStart() {};

    virtual void onStop() {};

    virtual void onPause() {};

    virtual void onUnpause() {};

    virtual void processEvent(sf::Event event) {};

    virtual void update(sf::Time dt) {};

    virtual void render(sf::RenderTarget &target) {};
};

}

#endif
