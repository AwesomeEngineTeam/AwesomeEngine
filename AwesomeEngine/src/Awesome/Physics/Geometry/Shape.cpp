#include <physX/extensions/PxRigidActorExt.h>

#include <Awesome/Physics/Body/Rigidbody.hpp>

namespace aw
{
	namespace ph
	{
		void Shape::Create(Rigidbody& body, const Geometry& geo, const Material& mat)
		{
			m_handle = physx::PxRigidActorExt::createExclusiveShape(*(body.Get()), geo.Get(), *(mat.m_handle));
			ASSERT(!m_handle, "Physics: CreateShape failed!");
		}

		void Shape::Create(Rigidbody& body, const Geometry& geo, const std::vector<Material>& mats)
		{
			m_handle = physx::PxRigidActorExt::createExclusiveShape(*(body.Get()), geo.Get(), reinterpret_cast<physx::PxMaterial* const*>(&mats[0]), uint16(mats.size()));
			ASSERT(!m_handle, "Physics: CreateShape failed!");
		}
	}
}