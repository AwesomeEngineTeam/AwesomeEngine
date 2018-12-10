#include <Awesome/SDK/System/EmptySystem.hpp>
#include <Awesome/SDK/Components/Component.hpp>

namespace aw
{
	namespace sdk
	{
		std::vector<Component*> EmptySystem::s_components;
		bool EmptySystem::s_isActive = true;

		void EmptySystem::GarbageCollect()
		{
			for (auto it = s_components.begin(); it != s_components.end(); ++it)
			{
				if ((*it)->GarbageCollectionRequired())
				{
					(*it)->m_isUsed = false;
					s_components.erase(it);

					break; //Lazy
				}
			}
		}

		bool EmptySystem::Register(Component* comp)
		{
			if (!comp)
				return false;

			comp->m_isUsed = true;
			s_components.push_back(comp);

			return true;
		}
	}
}