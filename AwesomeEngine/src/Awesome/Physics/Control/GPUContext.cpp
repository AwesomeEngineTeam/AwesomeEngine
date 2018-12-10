#include <physX/gpu/PxGpu.h>

#include <Awesome/Physics/Control/Context.hpp>

namespace aw
{
	namespace ph
	{
		void GPUContext::Create(const GPUContextDescriptor& desc)
		{
			#if PX_WINDOWS && AWESOME_PHYSICS_USE_GPU
			m_handle = PxCreateCudaContextManager(*(Context::foundation.Get()), desc.Get());
			#endif
		}
	}
}