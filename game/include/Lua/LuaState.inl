#include "Lua/LuaAnyConversions.hpp"

namespace lua
{

template<class T>
void LuaState::declareAnyConvertibleType(const std::string& typeName)
{
    m_luaState.set_function(typeName + "_value", &anyconv::ConvertToAny<T>);
    m_luaState.set_function("as_" + typeName, &anyconv::ConvertFromAny<T>);
}

}
