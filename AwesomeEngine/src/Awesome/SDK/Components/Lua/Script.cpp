#include <Awesome/SDK/Components/Lua/Script.hpp>
#include <Awesome/SDK/Lua/Libraries.hpp>

namespace aw
{
	namespace sdk
	{
		Script::Script(Actor* actor, const std::string& filename) :
			Component(actor, static_cast<uint32>(ComponentID::Script), filename),
			m_script{ this, filename }
		{
		}

#if defined(WITH_EDITOR)
		void Script::SetBindingsLayout(ed::ScriptWidget* widget) const
		{
			m_script.SetBindingsLayout(widget);
		}

		void Script::SetBindingLayout(ed::ScriptWidget* widget, const std::string& fieldName) const
		{
			m_script.SetBindingLayout(widget, fieldName);
		}

		void Script::RemoveBinding(const std::string& fieldName)
		{
			m_script.RemoveBinding(fieldName);
		}
#endif

		void Script::Run()
		{
			m_script.Run();
		}

		void Script::Start()
		{
			m_script.Start();
		}

		void Script::Update(float frameTime)
		{
			m_script.Update(frameTime);
		}

		void Script::OnTrigger(Component* myComp, Component* other)
		{
			m_script.OnTrigger(myComp, other);
		}

		void Script::OnCollision(Component* myComp, Component* other)
		{
			m_script.OnCollision(myComp, other);
		}

		void Script::OnDestroy()
		{
			m_script.OnDestroy();
		}

		const std::string& Script::GetPath() const
		{
			return m_script.GetPath();
		}

		void Script::SetPath(const std::string& path)
		{
			m_script.SetPath(path);
		}

		int Script::Get(lua_State* L)
		{
			return m_script.Get(L);
		}

		int Script::Set(lua_State* L)
		{
			return m_script.Set(L);
		}

		int Script::Execute(lua_State* L, const char* funcName)
		{
			return m_script.Execute(L, funcName);
		}
	}
}