#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Material.hpp>

namespace aw
{
	namespace ph
	{
		void Material::Create(float staticFriction, float dynamicFriction, float restitution)
		{
			m_handle = Context::instance.Get()->createMaterial(staticFriction, dynamicFriction, restitution);
			ASSERT(!m_handle, "Physics: Material::Create failed!")
		}
	}
}