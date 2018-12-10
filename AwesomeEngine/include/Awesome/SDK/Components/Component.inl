namespace aw
{
	namespace sdk
	{
		inline Component::Component(Actor* actor) :
			Component(actor, uint32(ComponentID::Component), "Unnamed Component")
		{
		}

		inline void Component::MarkForGarbageCollection()
		{
			for (auto child : m_children)
				child->MarkForGarbageCollection();

			m_pendingGC = true;
		}

		inline bool Component::IsDirty() const
		{
			return m_isHierarchyDirty;
		}

		inline Actor* Component::GetActor()
		{
			return m_actor;
		}

		inline const Actor* Component::GetActor() const
		{
			return m_actor;
		}

		inline Vector3 Component::GetLocalPosition() const
		{
			m_trMutex.lock();
			Vector3 position = m_localTransform.GetPosition();
			m_trMutex.unlock();

			return position;
		}

		inline Quaternion Component::GetLocalRotation() const
		{
			m_trMutex.lock();
			Quaternion rotation = m_localTransform.GetRotation();
			m_trMutex.unlock();

			return rotation;
		}

		inline UniformTransform Component::GetLocalTransform() const
		{
			m_trMutex.lock();
			UniformTransform tr = m_localTransform;
			m_trMutex.unlock();

			return tr;
		}

		inline UniformTransform Component::GlobalTransform()
		{
			if (m_isHierarchyDirty)
				CalculateGlobalTransform();

			m_trMutex.lock();
			UniformTransform tr = m_globalTransform;
			m_trMutex.unlock();

			return tr;
		}

		inline Vector3 Component::GlobalPosition()
		{
			return GlobalTransform().GetPosition();
		}

		inline Quaternion Component::GlobalRotation()
		{
			return GlobalTransform().GetRotation();
		}

		inline float Component::Scale()
		{
			m_trMutex.lock();
			float s = m_localTransform.scale;
			m_trMutex.unlock();

			return s;
		}

		inline void Component::SetLocalTransform(const UniformTransform& transform)
		{
			m_trMutex.lock();
			m_localTransform = transform;
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::SetLocalPosition(const Vector3& position)
		{
			m_trMutex.lock();
			m_localTransform.SetPosition(position);
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::SetLocalRotation(const Quaternion& rotation)
		{
			m_trMutex.lock();
			m_localTransform.SetRotation(rotation);
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::SetGlobalPosition(const Vector3& position)
		{
			m_trMutex.lock();
			m_localTransform.SetPosition(GetHierarchyGlobalTransform().Inverse() * position);
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::SetGlobalRotation(const Quaternion& rotation)
		{
			m_trMutex.lock();
			m_localTransform.SetRotation(GetHierarchyGlobalTransform().Inverse() * rotation);
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::SetGlobalTransform(const UniformTransform& transform)
		{
			//Should be checked.
			m_trMutex.lock();
			m_localTransform = GetHierarchyGlobalTransform().Inverse() * transform;
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::SetScale(float scale)
		{
			m_trMutex.lock();
			m_localTransform.scale = scale;
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::LookAt(const Vector3& target, const Vector3& up)
		{
			Vector3 globalPosition = GlobalPosition();

			Vector3 zaxis = (globalPosition - target).Normalize();
			Vector3 xaxis = (up ^ zaxis).Normalize();
			Vector3 yaxis = zaxis ^ xaxis;

			Matrix3 rot(
				xaxis.x, yaxis.x, zaxis.x,
				xaxis.y, yaxis.y, zaxis.y,
				xaxis.z, yaxis.z, zaxis.z
			);

			m_trMutex.lock();
			m_localTransform.SetRotation(GetHierarchyGlobalTransform().Inverse() * rot.ToQuaternion());
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::Rotate_WorldSpace(const Quaternion& rotation)
		{
			m_trMutex.lock();
			m_localTransform.SetRotation(rotation * m_localTransform.GetRotation());
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::Rotate_SelfSpace(const Quaternion& rotation)
		{
			m_trMutex.lock();
			m_localTransform.SetRotation(m_localTransform.GetRotation() * rotation);
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::Rotate_WorldSpace(const Vector3& eulerAngles)
		{
			m_trMutex.lock();
			m_localTransform.SetRotation(Quaternion::FromEulerAngles(eulerAngles) * m_localTransform.GetRotation());
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::Rotate_SelfSpace(const Vector3& eulerAngles)
		{
			m_trMutex.lock();
			m_localTransform.SetRotation(m_localTransform.GetRotation() * Quaternion::FromEulerAngles(eulerAngles));
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		// TODO : Do
		inline void Component::RotateAround(const Vector3& point, const Vector3& axis, float angle)
		{
			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::Translate_WorldSpace(const Vector3& translationVector) // TODO : Check
		{
			m_trMutex.lock();
			m_localTransform.SetPosition(m_localTransform.GetPosition() + translationVector);
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}

		inline void Component::_Translate_WorldSpace(const Vector3& translationVector)
		{
			Translate_WorldSpace(translationVector);
		}

		inline void Component::_Translate_SelfSpace(const Vector3& translationVector)
		{
			Translate_SelfSpace(translationVector);
		}

		inline void Component::_SetGlobalPosition(const Vector3& position)
		{
			SetGlobalPosition(position);
		}

		inline void Component::_SetGlobalRotation(const Quaternion& rotation)
		{
			SetGlobalRotation(rotation);
		}

		inline void Component::_SetGlobalTransform(const UniformTransform& transform)
		{
			SetGlobalTransform(transform);
		}

		inline void Component::Translate_SelfSpace(const Vector3& translationVector)
		{
			Quaternion globalRotation = GlobalRotation();

			m_trMutex.lock();
			m_localTransform.SetPosition(m_localTransform.GetPosition() + globalRotation * translationVector);
			m_trMutex.unlock();

			CalculateGlobalTransform();

			m_trMutex.lock();
			SetChildHierarchyDirty();
			m_trMutex.unlock();
		}
	}
}
