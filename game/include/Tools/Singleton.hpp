#ifndef YAPG_GAME_TOOLS_SINGLETON_H
#define YAPG_GAME_TOOLS_SINGLETON_H

namespace tools
{

template<class Derived>
class Singleton
{
public:
    static Derived& getInstance()
    {
        return instance;
    }

protected:
    Singleton() {};

private:

    static Derived instance;
};

template<class Derived>
Derived Singleton<Derived>::instance = Derived();

}

#endif
