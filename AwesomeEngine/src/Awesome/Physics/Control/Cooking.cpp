#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Control/Cooking.hpp>

namespace aw
{
	namespace ph
	{
		void Cooking::Create()
		{
			m_handle = PxCreateCooking(PX_PHYSICS_VERSION, *(Context::foundation.Get()), physx::PxCookingParams(Context::instance.GetTolerancesScale()));
			ASSERT(!m_handle, "Physics: Cooking::Create failed!");
		}
	}
}