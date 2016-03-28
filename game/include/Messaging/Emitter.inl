namespace messaging
{

template<typename MessageType, typename... Args>
void Emitter::emit(Args... args)
{
    MessagingManager::getInstance().send(MessageType(args...));
}

}
