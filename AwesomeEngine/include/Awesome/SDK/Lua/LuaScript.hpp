#pragma once

#ifndef AWESOME_LUA_SCRIPT_GUARD
#define AWESOME_LUA_SCRIPT_GUARD

#include <Awesome/SDK/Config.hpp>

#include <Awesome/Core/Logger/Logger.hpp>
#include <Awesome/Core/Reflexion/Reflect.hpp>
#include <Awesome/Lua/Pushable.hpp>

#include "luwra.hpp"
#include <Awesome/SDK/Components/Component.hpp>
#include <Awesome/SDK/Components/Physical/PhysicalComponent.hpp>

#if defined(WITH_EDITOR)
    #include <sys/types.h>
    #include <sys/stat.h>

	#include <Editor/Widgets/ScriptWidget.hpp>
#endif

#ifdef interface
#undef interface
#endif

namespace aw
{
	namespace sdk
	{
		class LuaScript
		{
			std::map<std::string, luwra::Pushable> m_bindings;

			std::unique_ptr<luwra::StateWrapper> m_state;
			std::function<void(float)> m_updateFunc;

			#if defined(WITH_EDITOR)
				std::map<std::string, std::function<void(ed::ScriptWidget*)>> m_bindingsVisits;
				time_t m_lastModifiedTime;
			#endif

			std::string m_filePath;
			Component* m_owner = nullptr;

			inline void InitInfo(Component* owner, const std::string& path);

		public:
			LuaScript() = delete;
			inline explicit LuaScript(const std::string& filename);
			inline LuaScript(Component* owner, const std::string& filename);

			~LuaScript() = default;

			#if defined(WITH_EDITOR)
				void AWESOME_SDK_API SetBindingsLayout(ed::ScriptWidget* widget) const;
				void AWESOME_SDK_API SetBindingLayout(ed::ScriptWidget* widget, const std::string& key) const;
			#endif

			void Run();
			void Start();
			AWESOME_SDK_API void Update(float frameTime);

			AWESOME_SDK_API void OnCollision(Component* myComp, Component* other);
			AWESOME_SDK_API void OnTrigger(Component* myComp, Component* other);

			AWESOME_SDK_API void OnDestroy();

			AWESOME_SDK_API const std::string& GetPath() const;
			AWESOME_SDK_API void SetPath(const std::string& path);

			template<typename T>
			inline void AddOrSetBinding(const std::string& key, T&& value);
			void RemoveBinding(const std::string& key);

			int Get(lua_State* L);
			int Set(lua_State* L);
			int Execute(lua_State* L, const std::string& funcName);

			inline explicit operator const luwra::StateWrapper&() const;
			inline explicit operator luwra::StateWrapper&();

			REFLECT_BEGIN(LuaScript)
				REFLECT_MEMBER(m_filePath)
				//REFLECT_MEMBER(m_bindings)
			REFLECT_END()
		};
	}
}

#include <Awesome/SDK/Lua/LuaScript.inl>

#endif // GUARD