#include <Awesome/Physics/Control/Context.hpp>

namespace aw
{
	namespace ph
	{
		AllocatorCallback Context::allocatorCallback;
		DebugCallback Context::debugCallback;

		Foundation Context::foundation;
		Pvd Context::pvd;
		Instance Context::instance;
		Cooking Context::cooking;
		Extensions Context::extensions;
		GPUContext Context::gpuContext;
	}
}