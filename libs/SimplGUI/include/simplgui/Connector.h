#ifndef SIMPLGUI_CONNECTOR_H
#define SIMPLGUI_CONNECTOR_H

#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace simplgui{ class Widget; }

namespace simplgui
{

template<class WidgetClass, class... Args>
class Connector
{
public:
    Connector() : m_bindedFunctions(), currentId(0) {};

    unsigned int bind(std::function<void(std::shared_ptr<WidgetClass>, Args...)> &function)
    {
        unsigned int newId = currentId++;
        m_bindedFunctions[newId] = function;

        return newId;
    }

    unsigned int bind(std::function<void(std::shared_ptr<WidgetClass>, Args...)> &&function)
    {
        unsigned int newId = currentId++;
        m_bindedFunctions[newId] = function;

        return newId;
    }

    void unbind(unsigned int id)
    {
        m_bindedFunctions.erase(id);
    }

    void call(std::shared_ptr<Widget> widget, Args... args)
    {
        call(std::dynamic_pointer_cast<WidgetClass>(widget), args...);
    }

    void call(std::shared_ptr<WidgetClass> widget, Args... args)
    {
        for(auto it = m_bindedFunctions.begin(); it != m_bindedFunctions.end(); ++it)
        {
            (it->second)(widget, args...);
        }
    }

private:
    std::map<unsigned int, std::function<void(std::shared_ptr<WidgetClass>, Args...)>> m_bindedFunctions;
    unsigned int currentId;
};

}

#endif
