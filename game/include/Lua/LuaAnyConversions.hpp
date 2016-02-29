#ifndef YAPG_GAME_LUA_LUAANYCONVERSIONS_H
#define YAPG_GAME_LUA_LUAANYCONVERSIONS_H

#include <boost/any.hpp>

namespace lua
{

namespace anyconv
{

namespace priv
{

template<class T>
typename std::enable_if<std::is_copy_constructible<T>::value && std::is_nothrow_destructible<T>::value, T>::type
ConvertFromAnyImpl(const boost::any& value)
{
    if(value.type() == typeid(T))
        return boost::any_cast<T>(value);
    else
        return T(); //TODO: Add a warning
}

template<class T>
typename std::enable_if<!std::is_copy_constructible<T>::value || !std::is_nothrow_destructible<T>::value, T>::type
ConvertFromAnyImpl(const boost::any& value)
{
    static_assert(true, "Trying to register a boost::any conversion of a type that doesn't support it !");
    return T();
}

template<class T>
typename std::enable_if<std::is_copy_constructible<T>::value && std::is_nothrow_destructible<T>::value, boost::any>::type
ConvertToAnyImpl(const T& value)
{
    return boost::any(value);
}

template<class T>
typename std::enable_if<!std::is_copy_constructible<T>::value || !std::is_nothrow_destructible<T>::value, boost::any>::type
ConvertFromAnyImpl(const T& value)
{
    static_assert(true, "Trying to register a boost::any conversion of a type that doesn't support it !");
    return boost::any();
}

}

template<class T>
T ConvertFromAny(const boost::any& value)
{
    return priv::ConvertFromAnyImpl<T>(value);
};

template<class T>
boost::any ConvertToAny(const T& value)
{
    return priv::ConvertToAnyImpl<T>(value);
}

}

}

#endif
