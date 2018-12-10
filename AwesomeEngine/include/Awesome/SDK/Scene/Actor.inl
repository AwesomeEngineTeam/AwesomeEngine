namespace aw
{
	namespace sdk
	{
		inline Actor::Actor(const std::string& name) :
			SceneNodeTemplate(name, true)
		{
		}

		inline Actor::~Actor()
		{
			if (m_root)
			{
				//UnregisterRecur(m_root);
				delete m_root;
			}
		}

		inline Component* Actor::GetRootComponent()
		{
			return m_root;
		}

		inline const Component* Actor::GetRootComponent() const
		{
			return m_root;
		}

		template <typename T>
		inline T* Actor::GetComponent()
		{
			if (!m_root)
				return nullptr;

			return reinterpret_cast<T*>(GetComponent<T>(m_root));
		}

		template <typename T>
		inline std::vector<T*> Actor::GetComponents()
		{
			if (!m_root)
				return std::vector<T*>();

			std::vector<T*> buff;
			GetComponent<T>(m_root, buff);

			return buff;
		}

		template <typename T>
		inline Component* Actor::GetComponent(Component* root)
		{
			int64 result = root->typeId - TypeInfo<T>::id;

			if (result == 0 || (result > 0 && result < 0x00100000))
				return root;

			for (auto it = root->GetFirstChild(); it != root->GetLastChild(); it++)
			{
				if (Component* comp = GetComponent<T>(reinterpret_cast<Component*>(*it)))
					return comp;
			}

			return nullptr;
		}

		template <typename T>
		inline void Actor::GetComponent(Component* root, std::vector<T*>& buff)
		{
			int64 result = root->typeId - TypeInfo<T>::id;

			if (result == 0 || (result > 0 && result < 0x00100000))
				buff.push_back(reinterpret_cast<T*>(root));

			for (auto it = root->GetFirstChild(); it != root->GetLastChild(); it++)
				GetComponent<T>(reinterpret_cast<Component*>(*it), buff);
		}

		inline Component* Actor::FindComponentById(uint32 id) const
		{
			if (!m_root)
				return nullptr;

			if (m_root->id == id)
				return m_root;

			return static_cast<Component*>(m_root->FindChildById(id));
		}

		inline Component* Actor::FindComponentWithName(const std::string& str) const
		{
			if (!m_root)
				return nullptr;

			auto comp = m_root->FindChildWithName(str);
			return static_cast<Component*>(comp);
		}

		inline std::vector<Component*> Actor::FindComponentsWithName(const std::string& str) const
		{
			if (!m_root)
				return std::vector<Component*>();

			auto comps = m_root->FindChildrenWithName(str);
			return reinterpret_cast<std::vector<Component*>&>(comps);
		}

		template <typename T>
		inline T* Actor::AddComponent(Component* parent)
		{
			T* newComp = new T(this);

#if WITH_EDITOR
			newComp = RegisterComponentInActor(newComp, parent);

			if(onAddComponent)
				onAddComponent(this);

			return newComp;
#else
			return RegisterComponentInActor(newComp, parent);
#endif
		}

		template <typename T, typename... Args>
		inline T* Actor::AddComponent(Component* parent, Args&&... args)
		{
			T* newComp = new T(this, std::forward<Args>(args)...);

#if WITH_EDITOR
			newComp = RegisterComponentInActor(newComp, parent);

			if (onAddComponent)
				onAddComponent(this);

			return newComp;
#else
			return RegisterComponentInActor(newComp, parent);
#endif
		}

		inline void Actor::RemoveComponent(Component* comp)
		{
			if (!comp)
				return;

			//UnregisterRecur(comp);

			comp->MarkForGarbageCollection();
			comp->SetActive(false);

			//m_gcMutex.lock();
			//m_componentsToDestroy.push_back(comp);
			//m_gcMutex.unlock();

			//if (comp == m_root)
			//{

			//	delete m_root;
			//	m_root = nullptr;
			//}
			//else
			//	m_root->RemoveChild(comp);

//#if WITH_EDITOR
//			if (onDestroyComponent)
//				onDestroyComponent(this);
//#endif
		}

		inline void Actor::MarkForGarbageCollection()
		{
			for (auto child : m_children)
				child->MarkForGarbageCollection();

			RemoveComponent(m_root);
			m_pendingGC = true;
		}

		template <typename T>
		T* Actor::RegisterComponentInActor(T* newComp, Component* parent)
		{
			if (!RegisterComponentInScene(newComp))
			{
				delete newComp;
				return nullptr;
			}

			if (!m_root)
				m_root = newComp;
			else if (!parent || parent->GetActor() != this)
			{
				m_root->AddChild(newComp);
				m_root->SetHierarchyDirty();
			}
			else
			{
				parent->AddChild(newComp);
				parent->SetHierarchyDirty();
			}

			return newComp;
		}

		inline void Actor::SetLocalPosition(const Vector3& position)
		{
			m_root->SetLocalPosition(position);
		}

		inline void Actor::SetLocalRotation(const Quaternion& rotation)
		{
			m_root->SetLocalRotation(rotation);
		}

		inline void Actor::SetGlobalPosition(const Vector3& position)
		{
			m_root->SetGlobalPosition(position);
		}

		inline void Actor::SetGlobalRotation(const Quaternion& rotation)
		{
			m_root->SetGlobalRotation(rotation);
		}

		inline void Actor::SetScale(float scale)
		{
			m_root->SetScale(scale);
		}

		inline void Actor::LookAt(const Vector3& target, const Vector3& up)
		{
			m_root->LookAt(target, up);
		}

		inline void Actor::Rotate_WorldSpace(const Quaternion& rotation)
		{
			m_root->Rotate_WorldSpace(rotation);
		}

		inline void Actor::Rotate_SelfSpace(const Quaternion& rotation)
		{
			m_root->Rotate_SelfSpace(rotation);
		}

		inline void Actor::Rotate_WorldSpace(const Vector3& eulerAngles)
		{
			m_root->Rotate_WorldSpace(eulerAngles);
		}

		inline void Actor::Rotate_SelfSpace(const Vector3& eulerAngles)
		{
			m_root->Rotate_SelfSpace(eulerAngles);
		}

		inline void Actor::RotateAround(const Vector3& point, const Vector3& axis, float angle)
		{
			m_root->RotateAround(point, axis, angle);
		}

		inline void Actor::Translate_WorldSpace(const Vector3& translationVector)
		{
			m_root->Translate_WorldSpace(translationVector);
		}

		inline void Actor::Translate_SelfSpace(const Vector3& translationVector)
		{
			m_root->Translate_SelfSpace(translationVector);
		}
	}
}