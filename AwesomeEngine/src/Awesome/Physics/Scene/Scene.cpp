#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Scene/Scene.hpp>

namespace aw
{
	namespace ph
	{
		void Scene::Create(const SceneDescriptor& sceneDesc)
		{
			m_handle = Context::instance.Get()->createScene(sceneDesc.Get());
			ASSERT(!m_handle, "Physcis: Scene::Create failed!");
		}

		RaycastHit Scene::Raycast(const Vector3& origin, const Vector3& dir, float maxDist) const
		{
			physx::PxRaycastBuffer hit;

			if (m_handle->raycast(reinterpret_cast<const physx::PxVec3&>(origin), reinterpret_cast<const physx::PxVec3&>(dir), maxDist, hit))
				return RaycastHit(reinterpret_cast<Rigidbody*>(hit.block.actor->userData), hit.block.distance, reinterpret_cast<const Vector3&>(hit.block.position), reinterpret_cast<const Vector3&>(hit.block.normal));

			return RaycastHit();
		}
	}
}