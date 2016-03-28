#ifndef YAPG_GAME_MESSAGING_MESSAGE_H
#define YAPG_GAME_MESSAGING_MESSAGE_H

namespace messaging
{

struct Message
{
    Message() {};
    virtual ~Message() {};
};

struct TestMessage1 : public Message {};
struct TestMessage2 : public Message {};
struct TestMessage3 : public Message {};

}

#endif
