#pragma once

#ifndef AWESOME_SDK_SCRIPT_GUARD
#define AWESOME_SDK_SCRIPT_GUARD

#include <Awesome/SDK/Components/Component.hpp>
#include <Awesome/SDK/Lua/LuaScript.hpp>

namespace aw
{
	namespace sdk
	{
		class Script : public Component
		{
			LuaScript m_script;

		public:
			Script(Actor* actor, const std::string& filename = "");

			#if defined(WITH_EDITOR)
				void AWESOME_SDK_API SetBindingsLayout(ed::ScriptWidget* widget) const;
				void AWESOME_SDK_API SetBindingLayout(ed::ScriptWidget* widget, const std::string& fieldName) const;
				void AWESOME_SDK_API RemoveBinding(const std::string& fieldName);
			#endif

			void Run();
			void Start();

			void Update(float deltaTime);

			void OnTrigger(Component* myComp, Component* other);
			void OnCollision(Component* myComp, Component* other);

			void OnDestroy();

			const std::string& GetPath() const;
			void SetPath(const std::string& path);

			template<typename T>
			inline void AddOrSetBinding(const std::string& key, T&& value);

			int Get(lua_State* L);
			int Set(lua_State* L);
			int Execute(lua_State* L, const char* funcName);

			REFLECT_BEGIN(Script)
				REFLECT_MEMBER_INHERITANCE(Component)
				REFLECT_MEMBER(m_script)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::Script>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::Script);
	};
}

#include <Awesome/SDK/Components/Lua/Script.inl>

#endif //GUARD