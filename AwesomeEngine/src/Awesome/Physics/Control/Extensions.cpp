#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Control/Extensions.hpp>

namespace aw
{
	namespace ph
	{
		void Extensions::Create()
		{
			if (m_isInit)
				return;

			ASSERT(!PxInitExtensions(*(Context::instance.Get()), Context::pvd.Get()), "Physics: Extensions::Create failed!");
			m_isInit = true;
		}
		void Extensions::Destroy()
		{
			if (!m_isInit)
				return;

			PxCloseExtensions();
			m_isInit = false;
		}
	}
}