#include <Awesome/Renderer/Memory/SubBuffer.hpp>
#include <Awesome/Renderer/Memory/BufferManager.hpp>

namespace aw
{
	void SubBuffer::Free()
	{
		if (block != nullptr)
			BufferManager::Free(poolId, block);
		block = nullptr;
		poolId = 0;
	}
}