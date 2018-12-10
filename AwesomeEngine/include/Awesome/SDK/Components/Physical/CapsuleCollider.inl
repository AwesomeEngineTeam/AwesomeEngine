namespace aw
{
	namespace sdk
	{
		inline CapsuleCollider::CapsuleCollider(Actor* actor, float radius, float halfHeight) :
			Collider(actor, uint32(ComponentID::CapsuleCollider), ph::CapsuleGeometry(radius, halfHeight), "Unnamed CapsuleCollider")
		{
			this->radius = radius;
			this->halfHeight = halfHeight;
		}

		inline bool CapsuleCollider::UpdateBuffering()
		{
			if (!Collider::UpdateBuffering())
				return false;

			aw::ph::CapsuleGeometry geo;
			m_shape.GetCapsuleGeometry(geo);

			geo.Radius() = radius * GlobalTransform().scale;
			geo.HalfHeight() = halfHeight * GlobalTransform().scale;

			m_shape.SetGeometry(geo);

			return true;
		}

		//inline float CapsuleCollider::GetRadius() const
		//{
		//	aw::ph::CapsuleGeometry geo;
		//	m_shape.GetCapsuleGeometry(geo);
		//	return geo.Radius();
		//}

		//inline void CapsuleCollider::SetRadius(float f)
		//{
		//	aw::ph::CapsuleGeometry geo;
		//	m_shape.GetCapsuleGeometry(geo);
		//	geo.Radius() = f;
		//	m_shape.SetGeometry(geo);
		//}

		//inline float CapsuleCollider::GetHalfHeight() const
		//{
		//	aw::ph::CapsuleGeometry geo;
		//	m_shape.GetCapsuleGeometry(geo);
		//	return geo.HalfHeight();
		//}

		//inline void CapsuleCollider::SetHalfHeight(float f)
		//{
		//	aw::ph::CapsuleGeometry geo;
		//	m_shape.GetCapsuleGeometry(geo);
		//	geo.HalfHeight() = f;
		//	m_shape.SetGeometry(geo);			
		//}

	}
}