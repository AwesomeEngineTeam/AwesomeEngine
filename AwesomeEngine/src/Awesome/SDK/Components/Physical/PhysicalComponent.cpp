#include <Awesome/SDK/Components/Physical/PhysicalComponent.hpp>

#include <Awesome/Core/Reflexion/Reflect.hpp>

REFLECT_ENUM(aw::sdk::CollisionLayer)

#include <Awesome/SDK/Scene/Actor.hpp>
#include <Awesome/SDK/System/ScriptSystem.hpp>

namespace aw
{
	namespace sdk
	{
		void PhysicalComponent::OnTriggerCallback(Component* other)
		{
			auto event = [this, other]()
			{
				if (!this || !other)
					return;

				auto scripts = m_actor->GetComponents<aw::sdk::Script>();

				for (auto script : scripts)
				{
					if(script->IsActive())
						script->OnTrigger(this, other);
				}
			};

			ScriptSystem::AddEvent(event);
		}

		void PhysicalComponent::OnCollisionCallback(Component* other)
		{
			auto event = [this, other]()
			{
				if (!this || !other)
					return;

				auto scripts = m_actor->GetComponents<aw::sdk::Script>();

				for (auto script : scripts)
				{
					if (script->IsActive())
						script->OnCollision(this, other);
				}
			};

			ScriptSystem::AddEvent(event);
		}
	}
}