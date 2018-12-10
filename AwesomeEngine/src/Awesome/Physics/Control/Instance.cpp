#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Control/Cooking.hpp>

namespace aw
{
	namespace ph
	{
		void Instance::Create(TolerancesScale scale)
		{
			m_handle = PxCreatePhysics(PX_PHYSICS_VERSION, *(Context::foundation.Get()), scale, true, Context::pvd.Get());
			ASSERT(!m_handle, "Physics: Instance::Create failed!");
		}
	}
}