#include <Awesome/Physics/Control/Context.hpp>
#include <Awesome/Physics/Control/Pvd.hpp>

namespace aw
{
	namespace ph
	{
		void Pvd::Create()
		{
			m_handle = PxCreatePvd(*(Context::foundation.Get()));
			ASSERT(!m_handle, "Physics: Pvd::Create failed!");

			//PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(nullptr, 5425, 10);
			//s_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
			// Usefull ?
		}
	}
}