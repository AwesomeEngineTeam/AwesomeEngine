#include <Awesome/Renderer/Memory/AllocatorMemoryPool.hpp>

namespace aw
{
	uint64 Allocator::AllocatorMemoryPool::s_count = 0;

	template MemInfo Allocator::AllocatorMemoryPool::Bind<vk::Buffer>(vk::MemoryRequirements memRequirements, vk::Buffer object);
	template MemInfo Allocator::AllocatorMemoryPool::Bind<vk::Image>(vk::MemoryRequirements memRequirements, vk::Image object);

	void Allocator::AllocatorMemoryPool::Init(uint64 allocSize, uint8 memTypeId, bool isHostVisible)
	{
		MemoryPool::Init(allocSize);

		m_deviceMemory.Allocate(allocSize, memTypeId);

		m_id = s_count++;
		m_memTypeId = memTypeId;

		if (isHostVisible)
			m_deviceMemory.Map(0, m_size, &m_memPtr);
	}

	/*Allocator::AllocatorMemoryPool::Block* Allocator::AllocatorMemoryPool::FindSuitableBlock(vk::MemoryRequirements memRequirements)
	{
		uint64 requiredSize = memRequirements.size;
		memRequirements.alignment = std::max(memRequirements.alignment, s_bufferImageGranularity);

		MEMORY_LOG("Required alignment = %lluB", memRequirements.alignment);

		uint64 nbBlocks = m_blocks.size();
		for (int i = 0; i< nbBlocks; ++i)
		{
			if (m_blocks[i]->free == false || m_blocks[i]->size < requiredSize)
				continue;

			if (m_blocks[i]->size == requiredSize)
			{
				if (m_blocks[i]->IsAligned(memRequirements.alignment) == false)
					continue;

				return m_blocks[i].get();
			}
			else if (m_blocks[i]->IsAligned(memRequirements.alignment) == true)//TODO Split block in three (first one for alignment) if possible
				return SplitBlock(m_blocks[i].get(), requiredSize);
		}

		return PushBack(requiredSize, memRequirements.alignment);
	}*/

	template<typename T>
	MemInfo Allocator::AllocatorMemoryPool::Bind(vk::MemoryRequirements memRequirements, T object)
	{
		MEMORY_LOG("[AllocatorMemoryPool] Pool : %u", m_id);
        MEMORY_LOG("[AllocatorMemoryPool] Pool Memory Type ID: %u", m_memTypeId);

		MemoryBlock* block = FindSuitableBlock(
			memRequirements.size,
			std::max(memRequirements.alignment, s_bufferImageGranularity)
		);

		if (block == nullptr)
			return {};

		Bind(block, object);

		return MemInfo(block, m_id, static_cast<char*>(m_memPtr) + block->offset * !!m_memPtr);
	}

	void Allocator::AllocatorMemoryPool::Unbind(MemoryBlock* block)
	{
		FreeBlock(block);
	}

	void Allocator::AllocatorMemoryPool::Bind(MemoryBlock* block, vk::Buffer buffer)
	{
		Context::GetDevice().Get().bindBufferMemory(buffer, m_deviceMemory.Get(), block->offset);
		MEMORY_LOG("[AllocatorMemoryPool #%d] buffer %p (size = %lfMB) bound to block %p.", m_id, buffer, block->size * BYTES_TO_MB, block);

		block->free = false;
		m_memoryLeft -= block->size;

		MEMORY_LOG("[AllocatorMemoryPool #%d] Reserve %d in block %p (%d)", m_id, block->size, block, buffer);

		MEMORY_LOG("[AllocatorMemoryPool] Memory left in pool %u : %lf MB", m_id, static_cast<double>(m_memoryLeft * BYTES_TO_MB));
	}

	void Allocator::AllocatorMemoryPool::Bind(MemoryBlock* block, vk::Image image)
	{
		Context::GetDevice().Get().bindImageMemory(image, m_deviceMemory.Get(), block->offset);
		MEMORY_LOG("image %p (size = %lfMB) bound to block %p.", image, block->size * BYTES_TO_MB, block);

		block->free = false;
		m_memoryLeft -= block->size;

		MEMORY_LOG("[AllocatorMemoryPool #%d] Reserve %d in block %p (%d)", m_id, block->size, block, image);

		MEMORY_LOG("[AllocatorMemoryPool] Memory left in pool %u : %lf MB", m_id, static_cast<double>(m_memoryLeft * BYTES_TO_MB));
	}
}