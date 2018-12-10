#pragma once

#ifndef AWESOME_LUA_ENUMS_GUARD
#define AWESOME_LUA_ENUMS_GUARD

#include <Awesome/Core/Types.hpp>

#include "lua.hpp"

namespace aw
{
	enum class LuaType : uint8
	{
		Nil = LUA_TNIL,
		Boolean = LUA_TBOOLEAN,
		Lightuserdata = LUA_TLIGHTUSERDATA,
		Number = LUA_TNUMBER,
		String = LUA_TSTRING,
		Table = LUA_TTABLE,
		Function = LUA_TFUNCTION,
		Userdata = LUA_TUSERDATA,
		Thread = LUA_TTHREAD,
	};
}

#endif //GUARD
