namespace aw
{
	namespace sdk
	{
		inline PhysicalComponent::PhysicalComponent(Actor* actor, uint32 typeId, bool isStatic, bool useGravity, const ph::Material& mat, const std::string& name, const UniformTransform& localTransform) :
			Component(actor, typeId, name, localTransform),
			material{ mat }
		{
			if (isStatic)
				m_body = new ph::RigidbodyStatic(UniformTransform::Identity(), this);
			else
				m_body = new ph::RigidbodyDynamic(UniformTransform::Identity(), this, useGravity);
		}

		inline PhysicalComponent::~PhysicalComponent()
		{
			if (m_body)
				delete m_body;
		}

		//inline ph::Rigidbody* PhysicalComponent::GetPhysicBody()
		//{
		//	return m_body;
		//}

		//inline const ph::Rigidbody* PhysicalComponent::GetPhysicBody() const
		//{
		//	return m_body;
		//}

		//inline void PhysicalComponent::SetActive(bool b)
		//{
		//	Component::SetActive(b);

		//	//m_body->SetActive(b);
		//}

		//inline CollisionLayer PhysicalComponent::GetFilterGroup() const
		//{
		//	return static_cast<CollisionLayer>(m_shape.GetFilterGroup());
		//}

		//inline void PhysicalComponent::SetFilterGroup(CollisionLayer group)
		//{
		//	m_shape.SetFilterGroup(static_cast<uint32>(group));
		//}

		//inline uint32 PhysicalComponent::GetFilterMask() const
		//{
		//	return m_shape.GetFilterMask();
		//}

		//inline void PhysicalComponent::SetFilterMask(uint32 mask)
		//{
		//	m_shape.SetFilterMask(mask);
		//}

		//inline void PhysicalComponent::SetFiltering(CollisionLayer group, uint32 mask)
		//{
		//	m_shape.SetFiltering(static_cast<uint32>(group), mask);
		//}

		inline UniformTransform PhysicalComponent::GetUpdatedGlobalTransform()
		{
			UniformTransform temp = m_body->FetchGlobalTransform(m_shape);
			return temp;
		}

		inline bool PhysicalComponent::UpdateBuffering()
		{
			m_body->SetActive(IsActive());

			if (!IsActive())
				return false;

			m_shape.SetFilterGroup(static_cast<uint32>(collisionLayer.load()));
			m_shape.SetFilterMask(filterMask);

			UniformTransform temp = GlobalTransform();
			m_body->SetGlobalTransform(temp);

			return true;
		}

		inline void PhysicalComponent::TriggerCallback(void* comp1, void* comp2)
		{
			PhysicalComponent* phComp1 = static_cast<PhysicalComponent*>(comp1);
			PhysicalComponent* phComp2 = static_cast<PhysicalComponent*>(comp2);

			phComp1->OnTriggerCallback(phComp2);
			phComp2->OnTriggerCallback(phComp1);
		}

		inline void PhysicalComponent::CollisionCallback(void* comp1, void* comp2)
		{
			PhysicalComponent* phComp1 = static_cast<PhysicalComponent*>(comp1);
			PhysicalComponent* phComp2 = static_cast<PhysicalComponent*>(comp2);

			phComp1->OnCollisionCallback(phComp2);
			phComp2->OnCollisionCallback(phComp1);
		}
	}
}