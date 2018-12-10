#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Body/RigidbodyStatic.hpp>

namespace aw
{
	namespace ph
	{
		void RigidbodyStatic::Create(const TransformBase& tr, void* userData)
		{
			Vector3 pos = tr.GetPosition();
			Quaternion rot = tr.GetRotation();

			physx::PxTransform pxTr(reinterpret_cast<const physx::PxVec3&>(pos), physx::PxQuat(rot.x, rot.y, rot.z, rot.w));
			m_handle = Context::instance.Get()->createRigidStatic(pxTr);
			ASSERT(!m_handle, "Physics: RigidbodyStatic::Create failed!");

			m_userData = userData;
			m_handle->userData = this;
		}
	}
}