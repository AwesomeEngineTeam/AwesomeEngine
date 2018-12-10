namespace aw
{
	namespace sdk
	{
		inline SphereCollider::SphereCollider(Actor* actor, float radius) :
			Collider(actor, uint32(ComponentID::SphereCollider), ph::SphereGeometry(radius), "Unnamed SphereCollider")
		{
			this->radius = radius;
		}

		inline bool SphereCollider::UpdateBuffering()
		{
			if (!Collider::UpdateBuffering())
				return false;

			aw::ph::SphereGeometry geo;
			geo.Radius() = radius * GlobalTransform().scale;
			m_shape.SetGeometry(geo);

			return true;
		}

		//inline float SphereCollider::GetRadius() const
		//{
		//	aw::ph::SphereGeometry geo;
		//	m_shape.GetSphereGeometry(geo);
		//	return geo.Radius();
		//}
		//
		//inline void SphereCollider::SetRadius(float f)
		//{
		//	aw::ph::SphereGeometry geo;
		//	geo.Radius() = f;
		//	m_shape.SetGeometry(geo);
		//}
	}
}