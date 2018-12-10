namespace aw
{
	namespace sdk
	{
		inline BoxCollider::BoxCollider(Actor* actor, Vector3 halfScale) :
			Collider(actor, uint32(ComponentID::BoxCollider), ph::BoxGeometry(halfScale), "Unnamed BoxCollider")
		{
			this->halfScale = halfScale;
		}

		inline bool BoxCollider::UpdateBuffering()
		{
			if (!Collider::UpdateBuffering())
				return false;

			aw::ph::BoxGeometry geo;
			geo.SetHalfScale(halfScale * GlobalTransform().scale);
			m_shape.SetGeometry(geo);

			return true;
		}

		//inline Vector3 BoxCollider::GetHalfScale() const
		//{
		//	aw::ph::BoxGeometry geo;
		//	m_shape.GetBoxGeometry(geo);
		//	return geo.GetHalfScale();
		//}

		//inline void BoxCollider::SetHalfScale(const Vector3& sc)
		//{
		//	aw::ph::BoxGeometry geo;
		//	geo.SetHalfScale(sc);
		//	m_shape.SetGeometry(geo);
		//}
	}
}