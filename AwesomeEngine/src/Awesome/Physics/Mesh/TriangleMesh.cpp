#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Mesh/TriangleMesh.hpp>

namespace aw
{
	namespace ph
	{
		void TriangleMesh::Create(const TriangleMeshDescriptor& desc)
		{
			m_handle = Context::cooking.Get()->createTriangleMesh(desc.Get(), Context::instance.Get()->getPhysicsInsertionCallback());
			ASSERT(!m_handle, "Physics: CreateTriangleMesh failed!");
		}
	}
}