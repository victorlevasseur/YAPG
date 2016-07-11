namespace messaging
{

template<class MessageType>
SingleReceiver<MessageType>::SingleReceiver() : BaseReceiver()
{
    MessagingManager::getInstance().subscribe<MessageType>(*this);
}

template<class MessageType>
SingleReceiver<MessageType>::~SingleReceiver()
{
    MessagingManager::getInstance().unsubscribe<MessageType>(*this);
}

}
