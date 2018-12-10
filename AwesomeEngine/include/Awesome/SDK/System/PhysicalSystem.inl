namespace aw
{
	namespace sdk
	{
		//inline PhysicalSystem::PhysicalSystem(const float _fixedInterval) : fixedInterval{ _fixedInterval }
		//{
		//	m_scene.Create();
		//}

		inline PhysicalSystem::~PhysicalSystem()
		{
			s_scene.Destroy();
		}

		inline void PhysicalSystem::Init()
		{
			s_scene.Create();
		}

		inline void PhysicalSystem::UnInit()
		{
			s_scene.Destroy();
		}

		//inline ph::Scene& PhysicalSystem::GetPhysicalScene()
		//{
		//	return m_scene;
		//}

		inline const ph::Scene& PhysicalSystem::GetPhysicalScene()
		{
			return s_scene;
		}

		inline bool PhysicalSystem::Register(Component* component)
		{
			if (component == nullptr)
				return false;

			if (InheritsOf(component->typeId, TypeInfo<Collider>::id) || component->typeId == TypeInfo<Rigidbody>::id)
			{
				//component->m_isUsed = true;

				//s_scene.AddActor(*(reinterpret_cast<PhysicalComponent*>(component)->m_body));
				//s_phComps.push_back(reinterpret_cast<PhysicalComponent*>(component));

				component->m_isUsed = true;

				s_instMutex.lock();
				s_compsToInstantiate.push_back(reinterpret_cast<PhysicalComponent*>(component));
				s_instMutex.unlock();

				return true;
			}
			
			return false;
		}

		//inline void PhysicalSystem::Unregister(Component* component)
		//{
		//	if (component == nullptr)
		//		return;

		//	if (InheritsOf(component->typeId, TypeInfo<Collider>::id) || component->typeId == TypeInfo<Rigidbody>::id)
		//	{
		//		//s_scene.RemoveActor(*reinterpret_cast<PhysicalComponent*>(component)->GetPhysicBody());

		//		//for (auto it = s_phComps.begin(); it != s_phComps.end(); it++)
		//		//{
		//		//	if (*it == component)
		//		//	{
		//		//		s_phComps.erase(it);
		//		//		return;
		//		//	}
		//		//}

		//		s_gcMutex.lock();
		//		s_compsToDestroy.push_back(reinterpret_cast<PhysicalComponent*>(component));
		//		s_gcMutex.unlock();
		//	}
		//}
	}
}