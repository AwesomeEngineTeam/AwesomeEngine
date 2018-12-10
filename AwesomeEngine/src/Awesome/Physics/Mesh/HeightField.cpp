#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Mesh/HeightField.hpp>

namespace aw
{
	namespace ph
	{
		void HeightField::Create(const HeightFieldDescriptor& desc)
		{
			m_handle = Context::cooking.Get()->createHeightField(desc.Get(), Context::instance.Get()->getPhysicsInsertionCallback());
			ASSERT(!m_handle, "Physics: CreateHeightField failed!");
		}
	}
}