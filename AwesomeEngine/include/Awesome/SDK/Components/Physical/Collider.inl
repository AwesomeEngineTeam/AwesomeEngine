namespace aw
{
	namespace sdk
	{
		inline Collider::Collider(Actor* actor, uint32 typeId, const ph::Geometry& geo, const std::string& name) :
			PhysicalComponent(actor, typeId, true, false, ph::Material(0.5f, 0.0f, 0.0f), name)
		{
			m_shape = m_body->CreateShape(geo, material);
		}

		inline bool Collider::UpdateBuffering()
		{
			if (!PhysicalComponent::UpdateBuffering())
				return false;

			m_shape.SetTrigger(isTrigger);

			return true;
		}
	}
}