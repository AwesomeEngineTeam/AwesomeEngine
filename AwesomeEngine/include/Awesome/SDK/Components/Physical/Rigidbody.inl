namespace aw
{
	namespace sdk
	{
		inline Rigidbody::Rigidbody(Actor* actor, const ph::Geometry& geo, const ph::Material& mat, float mass, bool useGravity) :
			PhysicalComponent(actor, uint32(ComponentID::Rigidbody), false, useGravity, mat, "Unnamed Rigidbody"),
			geometryType{ geo.GetType() }
		{
			if (geometryType == ph::GeometryType::Box)
				halfScale =  reinterpret_cast<const ph::BoxGeometry&>(geo).GetHalfScale();
			else if (geometryType == ph::GeometryType::Sphere || geometryType == ph::GeometryType::Capsule)
			{
				radius = reinterpret_cast<const ph::SphereGeometry&>(geo).Radius();

				if (geometryType == ph::GeometryType::Capsule)
					halfHeight = reinterpret_cast<const ph::CapsuleGeometry&>(geo).HalfHeight();
			}


			m_shape = m_body->CreateShape(geo, material);
			this->useGravity = useGravity;
			this->mass = mass;
			//SetFiltering();
		}

		inline UniformTransform Rigidbody::GetUpdatedGlobalTransform()
		{
			//UpdateRadius();
			//UpdateHalfHeight();
			//UpdateHalfScale();

			auto body = dynamic_cast<ph::RigidbodyDynamic*>(m_body);

			linearVelocity = body->GetLinearVelocity();
			angularVelocity = body->GetAngularVelocity();

			return PhysicalComponent::GetUpdatedGlobalTransform();
		}

		inline bool Rigidbody::UpdateBuffering()
		{
			if (!PhysicalComponent::UpdateBuffering())
				return false;

			m_body->SetUseGravity(useGravity);

			auto body = dynamic_cast<ph::RigidbodyDynamic*>(m_body);

			material.Update();

			#if defined(WITH_EDITOR)
			if (m_shape.GetGeometryType() != geometryType)
			{
				body->DetachShape(m_shape);

				if (geometryType == ph::GeometryType::Box)
					m_shape = body->CreateShape(ph::BoxGeometry(halfScale), material);
				else if (geometryType == ph::GeometryType::Sphere)
					m_shape = body->CreateShape(ph::SphereGeometry(radius), material);
				else if (geometryType == ph::GeometryType::Capsule)
					m_shape = body->CreateShape(ph::CapsuleGeometry(radius, halfHeight), material);
			}
			#endif

			body->SetMass(mass);
			body->SetKinematic(isKinematic);
			body->SetLockAxis(lockAxisMask);
			body->SetCollisionDetection(collisionDetectionMode);

			if (!isKinematic)
			{
				body->SetLinearVelocity(linearVelocity);
				body->SetAngularVelocity(angularVelocity);
			}

			linearForcesMutex.lock();

			for (size_t i = 0; i < m_linearForces.size(); ++i)
				body->AddForce(m_linearForces[i].force, m_linearForces[i].mode);

			m_linearForces.clear();
			linearForcesMutex.unlock();

			angularForcesMutex.lock();

			for (size_t i = 0; i < m_angularForces.size(); ++i)
				body->AddTorque(m_angularForces[i].force, m_angularForces[i].mode);

			m_angularForces.clear();
			angularForcesMutex.unlock();

			float scale = GlobalTransform().scale;

			SetRadius(radius * scale);
			SetHalfHeight(halfHeight * scale);
			SetHalfScale(halfScale * scale);

			return true;
		}

		//inline bool Rigidbody::IsSleeping() const
		//{
		//	return reinterpret_cast<ph::RigidbodyDynamic*>(m_body)->IsSleeping();
		//}

		//inline bool Rigidbody::GetUseGravity() const
		//{
		//	return m_body->GetUseGravity();
		//}

		//inline void Rigidbody::SetUseGravity(bool b)
		//{
		//	m_body->SetUseGravity(b);
		//}

		inline bool Rigidbody::IsKinematic() const
		{
			return isKinematic;
		}

		inline void Rigidbody::SetKinematic(bool b)
		{
			isKinematic = b;
			if (b)
			{
				linearVelocity = Vector3::Zero();
				angularVelocity = Vector3::Zero();

				linearForcesMutex.lock();
				angularForcesMutex.lock();

				m_linearForces.clear();
				m_angularForces.clear();

				linearForcesMutex.unlock();
				angularForcesMutex.unlock();
			}
		}

		//inline Vector3 Rigidbody::GetLinearVelocity()
		//{
		//	return reinterpret_cast<ph::RigidbodyDynamic*>(m_body)->GetLinearVelocity();
		//}

		//inline void Rigidbody::SetLinearVelocity(const Vector3& vel)
		//{
		//	reinterpret_cast<ph::RigidbodyDynamic*>(m_body)->SetLinearVelocity(vel);
		//}

		//inline Vector3 Rigidbody::GetAngularVelocity()
		//{
		//	return reinterpret_cast<ph::RigidbodyDynamic*>(m_body)->GetAngularVelocity();
		//}

		//inline void Rigidbody::SetAngularVelocity(const Vector3& vel)
		//{
		//	reinterpret_cast<ph::RigidbodyDynamic*>(m_body)->SetAngularVelocity(vel);
		//}

		#if defined(WITH_EDITOR)
		inline void Rigidbody::SetGeometry(ph::GeometryType type)
		{
			geometryType = type;
		}
		#endif

		//inline ph::GeometryType Rigidbody::GetGeometryType() const
		//{
		//	return m_shape.GetGeometryType();
		//}

		//inline void Rigidbody::UpdateRadius()
		//{
		//	if (m_shape.GetGeometryType() == ph::GeometryType::Sphere)
		//	{
		//		ph::SphereGeometry geo;
		//		m_shape.GetSphereGeometry(geo);
		//		radius = geo.Radius();
		//	}
		//	else if (m_shape.GetGeometryType() == ph::GeometryType::Capsule)
		//	{
		//		ph::CapsuleGeometry geo;
		//		m_shape.GetCapsuleGeometry(geo);
		//		radius = geo.Radius();
		//	}
		//}

		inline void Rigidbody::SetRadius(float f)
		{
			if (m_shape.GetGeometryType() == ph::GeometryType::Sphere)
			{
				ph::SphereGeometry geo;
				geo.Radius() = f;
				m_shape.SetGeometry(geo);
			}
			else if (m_shape.GetGeometryType() == ph::GeometryType::Capsule)
			{
				ph::CapsuleGeometry geo;
				m_shape.GetCapsuleGeometry(geo);
				geo.Radius() = f;
				m_shape.SetGeometry(geo);
			}
		}

		//inline void Rigidbody::UpdateHalfHeight()
		//{
		//	if (m_shape.GetGeometryType() == ph::GeometryType::Capsule)
		//	{
		//		ph::CapsuleGeometry geo;
		//		m_shape.GetCapsuleGeometry(geo);
		//		halfHeight = geo.HalfHeight();
		//	}
		//}

		inline void Rigidbody::SetHalfHeight(float f)
		{
			if (m_shape.GetGeometryType() == ph::GeometryType::Capsule)
			{
				ph::CapsuleGeometry geo;
				m_shape.GetCapsuleGeometry(geo);
				geo.HalfHeight() = f;
				m_shape.SetGeometry(geo);
			}
		}

		//inline void Rigidbody::UpdateHalfScale()
		//{
		//	if (m_shape.GetGeometryType() == ph::GeometryType::Box)
		//	{
		//		ph::BoxGeometry geo;
		//		m_shape.GetBoxGeometry(geo);
		//		halfScale = geo.GetHalfScale();
		//	}
		//}

		inline void Rigidbody::SetHalfScale(const Vector3& v)
		{
			if (m_shape.GetGeometryType() == ph::GeometryType::Box)
			{
				ph::BoxGeometry geo;
				geo.SetHalfScale(v);
				m_shape.SetGeometry(geo);
			}
		}

		inline void Rigidbody::AddForce(const Vector3& force, ph::ForceMode mode)
		{
			//reinterpret_cast<ph::RigidbodyDynamic*>(m_body)->AddForce(force, mode);
			linearForcesMutex.lock();
			m_linearForces.emplace_back(force, mode);
			linearForcesMutex.unlock();
		}

		inline void Rigidbody::AddTorque(const Vector3& torque, ph::ForceMode mode)
		{
			//reinterpret_cast<ph::RigidbodyDynamic*>(m_body)->AddTorque(torque, mode);
			angularForcesMutex.lock();
			m_angularForces.emplace_back(torque, mode);
			angularForcesMutex.unlock();
		}
	}
}