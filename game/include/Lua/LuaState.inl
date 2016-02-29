#include "Lua/LuaAnyConversions.hpp"

namespace lua
{

template<class T>
void LuaState::declareAnyConvertibleType(const std::string& typeName)
{
    m_luaState.set_function(typeName + "_to_any", &anyconv::ConvertToAny<T>);
    m_luaState.set_function(typeName + "_from_any", &anyconv::ConvertFromAny<T>);
}

}
