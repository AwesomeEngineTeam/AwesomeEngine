namespace base
{
	static int lua_import(lua_State* L)
	{
		const char* libName = lua_tostring(L, 1);

		if (strcmp(libName, "Input") == 0)
		    input::ImportLibrary(L);
		else if (strcmp(libName, "Time") == 0)
			time::ImportLibrary(L);
		else if (strcmp(libName, "Maths") == 0)
            math::ImportLibrary(L);
		else if (strcmp(libName, "Vector3") == 0)
            vec3::ImportLibrary(L);
        else if (strcmp(libName, "Vector2") == 0)
            vec2::ImportLibrary(L);
        else if (strcmp(libName, "Quaternion") == 0)
            quat::ImportLibrary(L);
        else if (strcmp(libName, "MathUtils") == 0)
            math_utils::ImportLibrary(L);
        else if (strcmp(libName, "Rotation") == 0)
            rot::ImportLibrary(L);
        else if (strcmp(libName, "UniformTransform") == 0)
            utr::ImportLibrary(L);
		else if (strcmp(libName, "Physics") == 0)
			physics::ImportLibrary(L);
        else
            LOG_WARNING("%s does not refer to an existing library.", libName);

		return 0;
	}

	static int lua_print(lua_State* L)
	{
		int nargs = lua_gettop(L);

		std::string res;
		for (int i = 1; i <= nargs; ++i)
		{
			if (lua_isstring(L, i) == true)
				res += lua_tostring(L, i);
			else if (luaL_callmeta(L, i, "__tostring") == true)
			{
				res += lua_tostring(L, -1);
				lua_pop(L, 1);
			}
			else if (lua_isboolean(L, i) == true)
				res += (lua_toboolean(L, i) == true ? "true" : "false");
            else
                res += "<nil/no __tostring found>";
		}

		LOG_LOG(res);

		return 0;
	}

	static int lua_cast(lua_State* L)
	{
		Component* comp = *reinterpret_cast<Component**>(luaL_checkudata(L, 1, TypeIDToString(ComponentID::Component)));

		if (!comp)
		{
			lua_settop(L, 1);
			return 1;
		}

		const char* type = lua_tostring(L, 2);
		lua_remove(L, 2);

		if (strcmp(type, TypeIDToString(comp->typeId)) == 0)
			luwra::setMetatable(L, type);
		else
		{
			lua_pushnil(L);

			if(lua_getglobal(L, "DisableCastExceptions") == static_cast<int>(LuaType::Nil))
			{
				lua_pop(L, 1);

				std::string errMsg = std::string("Cannot cast from ") + TypeIDToString(comp->typeId) + " to " + type;
				return luaL_argerror(L, 2, errMsg.c_str());
			}
			else lua_pop(L, 1);
		}

		return 1;
	}

	static void ImportLibrary(lua_State* state)
    {
		static constexpr luaL_Reg baseRegs[] = { {"print", lua_print}, {"import", lua_import}, {"Cast", lua_cast}, LUA_END_METATABLE };

        lua_getglobal(state, "_G");
        luaL_setfuncs(state, baseRegs, 0);

        lua_pop(state, 1);
    }
}