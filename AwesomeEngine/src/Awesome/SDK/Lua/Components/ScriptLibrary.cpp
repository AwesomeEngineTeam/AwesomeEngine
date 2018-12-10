#include <Awesome/SDK/Lua/Libraries.hpp>

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace script
			{
				static int lua_getfromscript(lua_State* L)
				{
					Script* script = *reinterpret_cast<Script**>(lua_touserdata(L, 1));
					return script->Get(L);
				}

				static int lua_setscriptfield(lua_State* L)
				{
					Script* script = *reinterpret_cast<Script**>(lua_touserdata(L, 1));
					return script->Set(L);
				}

				static int lua_invokeMethod(lua_State* L)
				{
					Script* script = *reinterpret_cast<Script**>(lua_touserdata(L, 1));
					lua_remove(L, 1);

					const char* funcName = lua_tostring(L, 1);
					lua_remove(L, 1);

					//LuaHelper::DumpStack(L);

					return script->Execute(L, funcName);
				}

				const luwra::MemberMap Fields
				{
					COMPONENT_FIELDS(Script),
					{ "Get", &lua_getfromscript },
					{ "Set", &lua_setscriptfield },
					{ "Invoke", &lua_invokeMethod }
				};

				const luwra::MemberMap MetaMethods
				{

				};
			}
		}
	}
}