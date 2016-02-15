#ifndef YAPG_GAME_STATE_STATE_H
#define YAPG_GAME_STATE_STATE_H

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace state{ class StateEngine; }

namespace state
{

/**
 * The first parameter of State sub-classes must be a ref to StateEngine.
 */
class State
{
public:
    using Ptr = std::unique_ptr<State>;
    using NonOwningPtr = State*;

    State(StateEngine& stateEngine);

    void onStart();

    virtual void onStop() {};

    virtual void onPause() {};

    virtual void onUnpause() {};

    virtual void processEvent(sf::Event event, sf::RenderTarget &target) {};

    void update(sf::Time dt);

    virtual void render(sf::RenderTarget &target) {};

protected:
    virtual void doStart() {};

    virtual void doUpdate(sf::Time dt) {};

    const StateEngine& getStateEngine() const;
    StateEngine& getStateEngine();

    sf::Time getTimeSinceStart() const;

private:
    StateEngine& m_stateEngine;
    sf::Time m_timeSinceStart;
};

}

#endif
