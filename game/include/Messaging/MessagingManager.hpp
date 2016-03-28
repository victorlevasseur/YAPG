#ifndef YAPG_GAME_MESSAGING_MESSAGINGMANAGER_H
#define YAPG_GAME_MESSAGING_MESSAGINGMANAGER_H

#include <map>
#include <set>
#include <typeindex>

#include "Messaging/Message.hpp"
#include "Tools/Singleton.hpp"
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
template<class MessageType>
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
template<class... MessageTypes>
class Receiver : public SingleReceiver<MessageTypes>...
{
public:
    Receiver() : SingleReceiver<MessageTypes>()... {};
    virtual ~Receiver() {};

private:

};

class MessagingManager : public tools::Singleton<MessagingManager>
{
    friend class tools::Singleton<MessagingManager>;

    template<class MessageType>
    friend class SingleReceiver;

public:
    virtual ~MessagingManager() {};

protected:
    MessagingManager() {};

private:
    template<class MessageType>
    void subscribe(SingleReceiver<MessageType>& receiver)
    {
        m_receivers[typeid(MessageType)].insert(&receiver);
        std::cout << "[Messaging/Info] A " << typeid(receiver).name() << " instance subscribed to " << typeid(MessageType).name() << " messages." << std::endl;
    }

    template<class MessageType>
    void unsubscribe(SingleReceiver<MessageType>& receiver)
    {
        m_receivers[typeid(MessageType)].erase(&receiver);
        std::cout << "[Messaging/Info] A " << typeid(receiver).name() << " instance unsubscribed to " << typeid(MessageType).name() << " messages." << std::endl;
    }

    template<class MessageType>
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

#include "Messaging/Receiver.inl"

#endif
