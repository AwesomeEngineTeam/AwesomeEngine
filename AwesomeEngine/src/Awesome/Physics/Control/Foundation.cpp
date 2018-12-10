#include <Awesome/Physics/Control/Foundation.hpp>

namespace aw
{
	namespace ph
	{
		void Foundation::Create(AllocatorCallback& allocCallback, DebugCallback& debCallback)
		{
			m_handle = PxCreateFoundation(PX_FOUNDATION_VERSION, allocCallback, debCallback);
			ASSERT(!m_handle, "Physics: Foundation::Create failed!");
		}
	}
}