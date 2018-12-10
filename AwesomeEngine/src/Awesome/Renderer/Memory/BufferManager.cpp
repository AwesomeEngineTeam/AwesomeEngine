#include <Awesome/Renderer/Memory/BufferManager.hpp>
#include <algorithm>

#ifdef min
	#undef min
#endif

#ifdef max
	#undef max
#endif

namespace aw
{
	std::vector<std::unique_ptr<BufferMemoryPool>> BufferManager::s_pools;

	void BufferManager::InitMemoryPool(uint64 size, BufferType bufferType, bool concurrent)
	{
		s_pools.emplace_back(std::make_unique<BufferMemoryPool>());

		s_pools.back()->Init(size, bufferType, concurrent);
	}

	BufferMemoryPool& BufferManager::FindOrCreateSuitablePool(uint64 size, bool aligned, BufferType bufferType, bool concurrent)
	{
		for (auto& pool : s_pools)
		{
			if ((uint8(pool->GetBufferType()) & uint8(bufferType)) == uint8(pool->GetBufferType()) &&
				pool->BiggestFreeBlock() >= size * 2) // TODO handle alignment restriction
				return *pool;
		}

		InitMemoryPool(BASE_BUFFER_POOL_ALLOC_SIZE > size ? BASE_BUFFER_POOL_ALLOC_SIZE : size, bufferType, concurrent);
		return *s_pools.back();
	}

	SubBuffer BufferManager::Reserve(uint64 size, bool aligned, BufferType bufferType, bool concurrent)
	{
		BufferMemoryPool& pool = FindOrCreateSuitablePool(size, aligned, bufferType, concurrent);

		return pool.Reserve(size, aligned);
	}

	void BufferManager::Free(uint64 poolId, MemoryBlock* block)
	{
		for (auto& pool : s_pools)
		{
			if (pool->Id() == poolId)
			{
				pool->Free(block);

				return;
			}
		}
	}
}