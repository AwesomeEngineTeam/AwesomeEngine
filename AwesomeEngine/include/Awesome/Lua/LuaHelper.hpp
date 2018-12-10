#pragma once

#ifndef AWESOME_LUA_HELPER_GUARD
#define AWESOME_LUA_HELPER_GUARD

#include "lua.hpp"

#include <Awesome/Core/Debug.hpp>
#include <Awesome/Core/Meta.hpp>

#include <Awesome/Lua/Enums.hpp>

#include <type_traits>
#include <utility>
#include <iostream>
#include <array>

#define LUA_REG(method) { #method, &method }
#define LUA_NULL_REG { NULL, NULL }
#define LUA_HIDE_METATABLE_REG { "__metatable", NULL }

#define LUA_PUSH_OP_RESULT(state, retType, lhsType, rhsType, op) luwra::Value<retType>::push(state, luwra::Value<lhsType>::read(state, 1) op luwra::Value<rhsType>::read(state, 2))

#define GENERATE_METAMETHOD_BODY_FROM_OPERATOR(state, retType, lhsType, rhsType, op) \
    if(LuaHelper::CheckType<lhsType>(state, 1) == true)\
        LUA_PUSH_OP_RESULT(state, retType, lhsType, rhsType, op); \
    else \
        LUA_PUSH_OP_RESULT(state, retType, rhsType, lhsType, op);

/// Use to generate a lua function that checks both lhsType op rhsType and rhsType op lhsType
/// \param name The metamethod name to replace.
/// \param retType Operator return type
/// \param lhsType Left operand type
/// \param rhsType Right operand type
/// \param op Operator to use
#define GENERATE_METAMETHOD_FROM_OPERATOR(name, retType, lhsType, rhsType, op) \
    static int name(lua_State* L) \
    { \
        GENERATE_METAMETHOD_BODY_FROM_OPERATOR(L, retType, lhsType, rhsType, op) \
        return 1; \
    }

namespace aw
{
	namespace LuaHelper
	{
		using luaCFunction = int(*)(lua_State* L);

		template<typename ValueType>
        inline bool CheckType(lua_State* L, int idx);

        inline bool IsUserData(lua_State* L, int idx);

        /// Returns true if both metatables are the same (by pointer !!)
        inline bool AreOfSameType(lua_State* L, int obj1, int obj2);

		template<typename ValueType>
		inline ValueType Get(lua_State* L, const char* fieldName);

		template<typename ValueType>
		inline void Set(lua_State* L, const char* fieldName, ValueType value);

		template<typename ValueType, size_t arraySize>
		inline void CreateTable(lua_State* L, const char* tableName, const std::array<std::pair<const char*, ValueType>, arraySize>& table);

		inline void CreateLibrary(lua_State* L, const char* libName, const luaL_Reg* lib, const luaL_Reg* metatable = nullptr);

		//inline void PrintTable(lua_State* L, int index);

		inline void DumpStack(lua_State* L);

		inline void CopyTo(lua_State* lhs, lua_State* rhs, int objIndex);
	}
}

#include <Awesome/Lua/LuaHelper.inl>

#endif //GUARD