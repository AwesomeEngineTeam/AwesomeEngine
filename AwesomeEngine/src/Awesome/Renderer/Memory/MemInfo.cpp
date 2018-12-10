#include <Awesome/Renderer/Memory/MemInfo.hpp>
#include <Awesome/Renderer/Memory/Allocator.hpp>

namespace aw
{
	void MemInfo::Destroy()
	{
		if (block != nullptr)
			Allocator::Unbind(poolID, block);

		block = nullptr;
		poolID = 0;
		memPtr = nullptr;
	}
}