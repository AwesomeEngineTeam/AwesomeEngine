namespace aw
{
	namespace sdk
	{
		inline Scene::Scene(const std::string& n) : m_name{ n }
		{
		}

		inline Scene::~Scene()
		{
			m_root.reset();
		}

		inline std::list<Actor*>::iterator Scene::GetFirstActor()
		{
			return m_root->GetFirstChild();
		}

		inline std::list<Actor*>::iterator Scene::GetLastActor()
		{
			return m_root->GetLastChild();
		}

		template<typename T>
		inline T* Scene::CreateActor(Actor* parent)
		{
			T* newActor = new T();

#if WITH_EDITOR
			if (onCreateActor)
				onCreateActor(newActor, parent);
#endif

			if (!parent)
				m_root->AddChild(newActor);
			else
				parent->AddChild(newActor);

			return newActor;
		}

		template<typename T, typename... Args>
		inline T* Scene::CreateActor(Actor* parent, Args&&... args)
		{
			T* newActor = new T(std::forward<Args>(args)...);

#if WITH_EDITOR
			if (onCreateActor)
				onCreateActor(newActor, parent);
#endif

			if (!parent)
				m_root->AddChild(newActor);
			else
				parent->AddChild(newActor);

			return newActor;
		}

		inline void Scene::RemoveActor(Actor* actor)
		{
			actor->MarkForGarbageCollection();
		}

		inline void Scene::RemoveActorImmediate(Actor* actor)
		{
			if (!actor)
				return;

#if WITH_EDITOR
			if (onDestroyActor)
				onDestroyActor(actor);
#endif

			m_root->RemoveChild(actor);
		}

		inline Actor* Scene::FindActorById(uint32 id)
		{
			return reinterpret_cast<Actor*>(m_root->FindChildById(id));
		}

		inline Actor* Scene::FindActorWithName(const std::string& name)
		{
			auto actor = m_root->FindChildWithName(name);
			return reinterpret_cast<Actor*>(actor);
		}

		inline std::vector<Actor*> Scene::FindActorsWithName(const std::string& name)
		{
			auto actors = m_root->FindChildrenWithName(name);
			return reinterpret_cast<std::vector<Actor*>&>(actors);
		}

		inline bool Scene::RegisterComponent(Component* comp)
		{
			if (!comp)
				return false;

			if (comp->typeId == TypeInfo<MeshRenderer>::id ||
				comp->typeId == TypeInfo<Skybox>::id ||
				comp->typeId == TypeInfo<Camera>::id ||
				comp->typeId == TypeInfo<DirectionalLight>::id ||
				comp->typeId == TypeInfo<PointLight>::id ||
				comp->typeId == TypeInfo<Text2D>::id ||
				comp->typeId == TypeInfo<Billboard>::id)
			{
				comp->m_isUsed = RenderSystem::Register(comp);
				return comp->m_isUsed;
			}
			else if (comp->typeId == TypeInfo<Component>::id)
			{
				comp->m_isUsed = EmptySystem::Register(comp);
				return comp->m_isUsed;
			}
			else if (comp->typeId == TypeInfo<Rigidbody>::id || InheritsOf(comp->typeId, TypeInfo<Collider>::id))
			{
				comp->m_isUsed = PhysicalSystem::Register(comp);
				return comp->m_isUsed;
			}
			else if (comp->typeId == TypeInfo<Script>::id)
			{
				comp->m_isUsed = ScriptSystem::Register(comp);
				return comp->m_isUsed;
			}
			else if (comp->typeId == TypeInfo<SoundListener>::id || comp->typeId == TypeInfo<SoundEmitter>::id)
			{
				comp->m_isUsed = SoundSystem::Register(comp);
				return comp->m_isUsed;
			}

			LOG_WARNING("Component with unknown type passed to RegisterComponent (addr: %p)", comp);

			return false;
		}

		//inline void Scene::UnregisterComponent(Component* comp)
		//{
		//	if (!comp)
		//		return;

		//	if (comp->typeId == TypeInfo<MeshRenderer>::id ||
		//		comp->typeId == TypeInfo<Skybox>::id ||
		//		comp->typeId == TypeInfo<Camera>::id ||
		//		comp->typeId == TypeInfo<DirectionalLight>::id ||
		//		comp->typeId == TypeInfo<PointLight>::id ||
		//		comp->typeId == TypeInfo<Text2D>::id ||
		//		comp->typeId == TypeInfo<Billboard>::id)
		//	{
		//		RenderSystem::Unregister(comp);
		//	}
		//	else if (comp->typeId == TypeInfo<Rigidbody>::id || InheritsOf(comp->typeId, TypeInfo<Collider>::id))
		//		PhysicalSystem::Unregister(comp);
		//	else if (comp->typeId == TypeInfo<Component>::id)
		//		EmptySystem::Unregister(comp);
		//	else if (comp->typeId == TypeInfo<Script>::id)
		//		ScriptSystem::Unregister(comp);
		//	else if (comp->typeId == TypeInfo<SoundListener>::id || comp->typeId == TypeInfo<SoundEmitter>::id)
		//		SoundSystem::Unregister(comp);
		//	else
		//	{
		//		LOG_WARNING("Component with unknown type passed to UnregisterComponent (addr: %p)", comp);
		//	}
		//}
	}
}