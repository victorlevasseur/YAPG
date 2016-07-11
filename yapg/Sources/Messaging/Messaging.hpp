#ifndef YAPG_GAME_MESSAGING_MESSAGINGMANAGER_H
#define YAPG_GAME_MESSAGING_MESSAGINGMANAGER_H

#include <map>
#include <set>
#include <typeindex>

#include "Tools/Singleton.hpp"

namespace yapg
{

namespace messaging
{

/**
 * Base class of receivers.
 * Just used to store all the receivers in a MessagingManager.
 */
class BaseReceiver
{
public:
    BaseReceiver() {};
    virtual ~BaseReceiver() {};
};

/**
 * A receiver able to receive a single type of Message.
 * Used internally by the Receiver class.
 */
template<typename MessageType>
class SingleReceiver : public BaseReceiver
{
public:
    SingleReceiver();

    virtual ~SingleReceiver();

    virtual void receive(const MessageType& message) = 0;
};

/**
 * Class able to receive multiple types of Message through methods with
 * a signature like void receive(const T& message) (T is the type of the Message).
 *
 * This is the class that should be used by the other classes of the game.
 */
template<typename... MessageTypes>
class Receiver : public SingleReceiver<MessageTypes>...
{
public:
    Receiver() : SingleReceiver<MessageTypes>()... {};
    virtual ~Receiver() {};

private:

};

class Emitter
{
public:
    template<typename MessageType, typename... Args>
    static void emit(Args... args);
};

class MessagingManager : public Singleton<MessagingManager>
{
    friend class Singleton<MessagingManager>;

    template<typename MessageType>
    friend class SingleReceiver;

    friend class Emitter;

public:
    virtual ~MessagingManager() {};

protected:
    MessagingManager() {};

private:
    template<typename MessageType>
    void subscribe(SingleReceiver<MessageType>& receiver)
    {
        m_receivers[typeid(MessageType)].insert(&receiver);
    }

    template<typename MessageType>
    void unsubscribe(SingleReceiver<MessageType>& receiver)
    {
        m_receivers[typeid(MessageType)].erase(&receiver);
    }

    template<typename MessageType>
    void send(const MessageType& message)
    {
        for(auto it = m_receivers[typeid(MessageType)].begin(); it != m_receivers[typeid(MessageType)].end(); ++it)
        {
            dynamic_cast<SingleReceiver<MessageType>*>((*it))->receive(message);
        }
    }

    std::map<std::type_index, std::set<BaseReceiver*>> m_receivers;
};

}

}

#include "Messaging/Emitter.inl"
#include "Messaging/Receiver.inl"

#endif
