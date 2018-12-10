namespace aw
{
	namespace LuaHelper
	{
		template<typename ValueType>
		inline ValueType Get(lua_State* L, const char* fieldName)
		{
			lua_getfield(L, -1, fieldName);

			ValueType res{};
			if constexpr(std::is_same_v<ValueType, bool>)
				res = lua_toboolean(L, -1);
			else if constexpr(std::is_floating_point_v<ValueType>)
				res = luaL_checknumber(L, -1);
			else if constexpr(std::is_integral_v<ValueType>)
				res = luaL_checkinteger(L, -1);
			else if constexpr(std::is_same_v<const char*, ValueType>)
				res = luaL_checkstring(L, -1);
			else if constexpr(std::is_same_v<std::string, ValueType>)
				res = luaL_checkstring(L, -1);

			lua_pop(L, 1);

			return res;
		}

        template<typename ValueType>
        inline bool CheckType(lua_State* L, int idx)
        {
            if constexpr(std::is_same_v<ValueType, bool>)
                return lua_isboolean(L, idx);
            else if constexpr(std::is_floating_point_v<ValueType>)
                return static_cast<bool>(lua_isnumber(L, idx));
            else if constexpr(std::is_integral_v<ValueType>)
                return static_cast<bool>(lua_isinteger(L, idx));
            else if constexpr(std::is_same_v<const char*, ValueType>)
                return static_cast<bool>(lua_isstring(L, idx));
            else if constexpr(std::is_same_v<std::string, ValueType>)
                return static_cast<bool>(lua_isstring(L, idx));
            else
                return static_cast<bool>(lua_isuserdata(L, idx));
        }

        inline bool IsUserData(lua_State* L, int idx)
        {
            return static_cast<bool>(lua_isuserdata(L, idx));
        }

        inline bool AreOfSameType(lua_State* L, int obj1, int obj2)
        {
            lua_getmetatable(L, obj1);
            lua_getmetatable(L, obj2);

            bool sameType = static_cast<bool>(lua_compare(L, -1, -2, LUA_OPEQ));
            lua_pop(L, 2);

            return sameType;
        }

		template<typename ValueType>
		inline void Set(lua_State* L, const char* fieldName, ValueType value)
		{
		    /*if constexpr(std::is_fundamental_v<ValueType> == false)
            {
                lua_createtable(L, int(meta::ClassDescriptor<ValueType>::nbFields), 0);
                auto lambda = [L](auto&& field)
                {
                    LuaHelper::Set(L, field.name, field.value);
                };

                meta::ClassDescriptor<ValueType>::Visit(lambda, &value);
                lua_setfield(L, -2 - nbFields, fieldName);
            }
			else */if constexpr(std::is_same_v<ValueType, bool>)
				lua_pushboolean(L, value);
			else if constexpr(std::is_floating_point_v<ValueType>)
				lua_pushnumber(L, value);
			else if constexpr(std::is_integral_v<ValueType>)
				lua_pushinteger(L, value);
			else if constexpr(std::is_same_v<const char*, ValueType>)
				lua_pushstring(L, value);
			else if constexpr(std::is_same_v<std::string, ValueType>)
				lua_pushstring(L, value.c_str());
			else if constexpr(std::is_same_v<luaCFunction, ValueType>)
				lua_pushcfunction(L, value);
			else
				lua_pushnil(L);

			lua_setfield(L, -2, fieldName);
		}

		template<typename ValueType, size_t arraySize>
		inline void CreateTable(lua_State* L, const char* tableName, const std::array<std::pair<const char*, ValueType>, arraySize>& table)
		{
			lua_createtable(L, arraySize, 0);

			if constexpr (std::is_same_v<bool, ValueType>)
			{
				for (size_t i = 0; i < arraySize; ++i)
				{
					lua_pushboolean(L, table[i].second);
					lua_setfield(L, -2, table[i].first);
				}
			}
			else if constexpr (std::is_floating_point_v<ValueType>)
			{
				for (size_t i = 0; i < arraySize; ++i)
				{
					lua_pushnumber(L, table[i].second);
					lua_setfield(L, -2, table[i].first);
				}
			}
			else if constexpr (std::is_integral_v<ValueType>)
			{
				for (size_t i = 0; i < arraySize; ++i)
				{
					lua_pushinteger(L, table[i].second);
					lua_setfield(L, -2, table[i].first);
				}
			}
			else if constexpr (std::is_same_v<const char*, ValueType>)
			{
				for (size_t i = 0; i < arraySize; ++i)
				{
					lua_pushstring(L, table[i].second);
					lua_setfield(L, -2, table[i].first);
				}
			}
			else if constexpr (std::is_same_v<std::string, ValueType>)
			{
				for (size_t i = 0; i < arraySize; ++i)
				{
					lua_pushstring(L, table[i].second.data());
					lua_setfield(L, -2, table[i].first);
				}
			}
			else if constexpr (std::is_same_v<luaCFunction, ValueType>)
			{
				for (size_t i = 0; i < arraySize; ++i)
				{
					lua_pushcfunction(L, table[i].second);
					lua_setfield(L, -2, table[i].first);
				}
			}

			if (tableName != nullptr)
				lua_setglobal(L, tableName);
		}

		inline void CreateLibrary(lua_State* L, const char* libName, const luaL_Reg* lib, const luaL_Reg* metatable)
		{
			luaL_newlib(L, lib);
			lua_setglobal(L, libName);

			if (metatable != nullptr)
			{
				lua_getglobal(L, libName);

				luaL_newlib(L, metatable);
				lua_setmetatable(L, -2);
			}
		}

		//inline void PrintTable(lua_State *L, int index)
		//{
		//	lua_pushnil(L);

		//	while (lua_next(L, index) != 0)
		//	{
		//		std::string key = lua_isstring(L, -2) ? lua_tostring(L, -2) : std::to_string(lua_tonumber(L, -2));

		//		if (lua_isstring(L, -1))
		//			LOG_LOG("\t%s = %s\n", key.c_str(), lua_tostring(L, -1))
		//		else if (lua_isnumber(L, -1))
		//			LOG_LOG("\t%s = %d\n", key.c_str(), lua_tonumber(L, -1))
		//		else if (lua_istable(L, -1))
		//			PrintTable(L, lua_gettop(L));

		//		lua_pop(L, 1);
		//	}
		//}

		inline void DumpStack(lua_State* L)
		{
			int i = lua_gettop(L);
			LOG_LOG("--------------- Stack Dump ------------------------");
			while (i)
			{
				int t = lua_type(L, i);
				switch (static_cast<LuaType>(t))
				{
				case LuaType::String:
					LOG_LOG("%d: %s", i, lua_tostring(L, i));
					break;
				case LuaType::Boolean:
					LOG_LOG("%d: %s", i, lua_toboolean(L, i) ? "true" : "false");
					break;
				case LuaType::Number:
					LOG_LOG("%d: %f", i, lua_tonumber(L, i));
					break;
				//case LuaType::Table:
				//	LOG_LOG("%d: table:");
				//	PrintTable(L, i);
				//	break;
				default:
					LOG_LOG("%d: %s", i, lua_typename(L, t));
					break;
				}
				i--;
			}
			LOG_LOG("--------------- End Stack Dump ---------------");
		}

		inline void CopyTo(lua_State* lhs, lua_State* rhs, int objIndex)
		{
			if (objIndex < 0)
				objIndex = lua_gettop(lhs) + objIndex + 1;

			switch (lua_type(lhs, objIndex))
			{
			case LUA_TNIL:
				lua_pushnil(rhs);
				return;

			case LUA_TNUMBER:
				lua_pushnumber(rhs, lua_tonumber(lhs, objIndex));
				return;

			case LUA_TBOOLEAN:
				lua_pushboolean(rhs, lua_toboolean(lhs, objIndex));
				return;

			case LUA_TSTRING:
				lua_pushstring(rhs, lua_tostring(lhs, objIndex));
				return;

			case LUA_TTABLE:
				lua_newtable(rhs);
				lua_pushnil(lhs);

				while (lua_next(lhs, objIndex) != 0)
				{
					int top = lua_gettop(lhs);
					CopyTo(lhs, rhs, -2);

					CopyTo(lhs, rhs, -1);
					lua_pop(lhs, 1);

					lua_settable(rhs, -3);
				}
				return;

			case LUA_TUSERDATA:
			case LUA_TLIGHTUSERDATA:
			case LUA_TFUNCTION:
			case LUA_TTHREAD:
				LOG_WARNING("Passing userdata, function or thread from state to state not implemented. Pushing nil.");
				lua_pushnil(rhs);
				return;
			}
		}
	}
}