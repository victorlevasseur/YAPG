#include "Lua/EntityHandle.hpp"
#include "Lua/LuaAnyConversions.hpp"

namespace yapg
{

template<class T>
void LuaState::declareAnyConvertibleType(const std::string& typeName)
{
    m_luaState.set_function(typeName + "_value", &anyconv::ConvertToAny<T>);
    m_luaState.set_function("as_" + typeName, &anyconv::ConvertFromAny<T>);
}

template<class T>
void LuaState::declareComponentGetter(const std::string& componentLuaName)
{
    m_luaState.set_function(componentLuaName, &EntityHandle::doGetComponentPtrForLua<T>);
}

}
