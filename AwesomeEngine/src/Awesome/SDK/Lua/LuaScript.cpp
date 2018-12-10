#include <Awesome/SDK/Lua/LuaScript.hpp>
#include <Awesome/SDK/Lua/Libraries.hpp>

namespace aw
{
	namespace sdk
	{

#if defined(WITH_EDITOR)
		void LuaScript::SetBindingsLayout(ed::ScriptWidget* widget) const
		{
			for (const auto& pair : m_bindingsVisits)
				pair.second(widget);
		}

		void LuaScript::SetBindingLayout(ed::ScriptWidget* widget, const std::string& key) const
		{
			auto it = m_bindingsVisits.find(key);

			if (it != m_bindingsVisits.end())
				it->second(widget);
		}
#endif

		void LuaScript::Run()
		{
			m_updateFunc = {};
			m_state = std::make_unique<luwra::StateWrapper>();

			lua_gc(*m_state, LUA_GCSETPAUSE, 100); //default = 200
			lua_gc(*m_state, LUA_GCSETSTEPMUL, 1500); //default = 200

			m_state->loadStandardLibrary();
			lua::base::ImportLibrary(*m_state);

			//TODO Make a lib that merges both ?
			lua::scene::ImportLibrary(*m_state);
			lua::comp::ImportLibrary(*m_state);

			if(m_owner)
				(*m_state)["this"] = m_owner;

			for (const auto& pair : m_bindings)
				(*m_state)[pair.first.c_str()] = pair.second;

			if (m_state->runFile(m_filePath.c_str()))
			{
				LOG_ERROR("%s", m_state->read<const char*>(-1));
				LOG_ERROR("Error while running script %s", m_filePath.c_str());
			}

			if (m_state->has("Update"))
				m_updateFunc = m_state->get<decltype(m_updateFunc)>("Update");
			else
				m_updateFunc = {};
		}

		void LuaScript::Start()
		{
			if (m_state.get() == nullptr)
				return;

			try
			{
				if (m_state->has("Start"))
				{
					//Passing through std::function to catch error messages
					m_state->get<std::function<void()>>("Start")();
				}
			}
			catch (const char* e)
			{
				LOG_ERROR(e);
				throw; //Because it is caught in a higher level
			}
			catch (...)
			{
				LOG_ERROR("Unhandled exception in %s", m_filePath.c_str());
			}
		}

		void LuaScript::Update(float frameTime)
		{
			if (m_updateFunc)
			{
				try
				{
					m_updateFunc(frameTime);
				}
				catch (const char* e)
				{
					LOG_ERROR(e)
				}
				catch (...)
				{
					LOG_ERROR("Unhandled exception in %s", m_filePath.c_str());
				}
			}
		}

		void LuaScript::OnCollision(Component* myComp, Component* other)
		{
			if (m_state.get() == nullptr)
				return;

			try
			{
				if (m_state->has("OnCollision"))
				{
					//Passing through std::function to catch error messages
					std::function<void(Component*, Component*)> func;
					func = m_state->get<decltype(func)>("OnCollision");
					
					func(myComp, other);
				}
			}
			catch (const char* e)
			{
				LOG_ERROR(e);
			}
			catch (...)
			{
				LOG_ERROR("Unhandled exception in %s", m_filePath.c_str());
			}
		}

		void LuaScript::OnTrigger(Component* myComp, Component* other)
		{
			if (m_state.get() == nullptr)
				return;

			try
			{
				if (m_state->has("OnTrigger"))
				{
					//Passing through std::function to catch error messages
					std::function<void(Component*, Component*)> func;
					func = m_state->get<decltype(func)>("OnTrigger");
					
					func(myComp, other);
				}
			}
			catch (const char* e)
			{
				LOG_ERROR(e);
			}
			catch (...)
			{
				LOG_ERROR("Unhandled exception in %s", m_filePath.c_str());
			}
		}

		void LuaScript::OnDestroy()
		{
			if (m_state.get() == nullptr)
				return;

			try
			{
				if (m_state->has("OnDestroy"))
				{
					//Passing through std::function to catch error messages
					std::function<void()> func;
					func = m_state->get<decltype(func)>("OnDestroy");

					func();
				}
			}
			catch (const char* e)
			{
				LOG_ERROR(e);
			}
			catch (...)
			{
				LOG_ERROR("Unhandled exception in %s", m_filePath.c_str());
			}
		}

		const std::string& LuaScript::GetPath() const
		{
			return m_filePath;
		}

		void LuaScript::SetPath(const std::string& path)
		{
			m_filePath = path;

			m_lastModifiedTime = 0;
		}

		void LuaScript::RemoveBinding(const std::string& key)
		{
			m_bindings.erase(key);
			m_bindingsVisits.erase(key);
		}

		int LuaScript::Get(lua_State* L)
		{
			lua_getglobal(*m_state, lua_tostring(L, -1));
			LuaHelper::CopyTo(*m_state, L, -1);

			lua_pop(*m_state, 1);

			return 1;
		}

		int LuaScript::Set(lua_State* L)
		{
			const char* field = lua_tostring(L, -2);
			if (m_state->has(field))
			{
				LuaHelper::CopyTo(L, *m_state, -1);
				lua_setglobal(*m_state, field);
			}
			else
				LOG_WARNING("No field named %s in script %s", field, m_filePath.c_str());

			return 0;
		}

		int LuaScript::Execute(lua_State* L, const std::string& funcName)
		{
			if (!m_state->has(funcName.c_str()))
			{
				LOG_WARNING("No function named %s in script %s", funcName.c_str(), m_filePath.c_str());
				return 0;
			}

			int ninitials = lua_gettop(*m_state);
			lua_getglobal(*m_state, funcName.c_str());

			int nargs = lua_gettop(L);
			for (int i = 1; i <= nargs; ++i)
				LuaHelper::CopyTo(L, *m_state, i);

			lua_pcall(*m_state, nargs, LUA_MULTRET, 0);

			int top = lua_gettop(*m_state);
			for (int i = (ninitials ? ninitials : 1); i <= top; ++i)
				LuaHelper::CopyTo(*m_state, L, i);

			int nresults = top - ninitials;
			if(nresults)
				lua_pop(*m_state, nresults);

			return nresults;
		}
	}
}