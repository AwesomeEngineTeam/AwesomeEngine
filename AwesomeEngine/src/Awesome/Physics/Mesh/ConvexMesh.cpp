#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Mesh/ConvexMesh.hpp>

namespace aw
{
	namespace ph
	{
		void ConvexMesh::Create(const ConvexMeshDescriptor& desc)
		{
			physx::PxDefaultMemoryOutputStream buf;
			physx::PxConvexMeshCookingResult::Enum result;

			ASSERT(!Context::cooking.Get()->cookConvexMesh(desc.Get(), buf, &result), "Physics: CookConvexMesh failed!");

			physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			m_handle = Context::instance.Get()->createConvexMesh(input);
		}
	}
}