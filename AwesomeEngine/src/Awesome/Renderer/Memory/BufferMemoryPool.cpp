#include <Awesome/Renderer/Memory/BufferMemoryPool.hpp>
#include <Awesome/Renderer/Control/Context.hpp>

namespace aw
{
	uint64 BufferMemoryPool::s_count = 0;

	void BufferMemoryPool::Init(uint64 size, BufferType type, bool concurrent)
	{
		MemoryPool::Init(size);

		m_id = s_count++;
		m_bufferType = type;

		vk::BufferUsageFlags usageFlags{};
		vk::MemoryPropertyFlags memoryFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;

		if ((uint8(type) & uint8(BufferType::Geometry)) != 0)
		{
			usageFlags |= vk::BufferUsageFlagBits::eIndexBuffer;
			usageFlags |= vk::BufferUsageFlagBits::eVertexBuffer;
			usageFlags |= vk::BufferUsageFlagBits::eTransferDst;
		}
		if ((uint8(type) & uint8(BufferType::Staging)) != 0)
		{
			usageFlags |= vk::BufferUsageFlagBits::eTransferSrc;
			memoryFlags = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
		}
		if ((uint8(type) & uint8(BufferType::Uniform)) != 0)
		{
			usageFlags |= vk::BufferUsageFlagBits::eUniformBuffer;
			usageFlags |= vk::BufferUsageFlagBits::eTransferDst;
		}
		if ((uint8(type) & uint8(BufferType::View)) != 0)
		{
			usageFlags |= vk::BufferUsageFlagBits::eTransferDst;
			memoryFlags = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
		}

		m_buffer.Create(usageFlags, memoryFlags, size, concurrent);
	}

	SubBuffer BufferMemoryPool::Reserve(uint64 size, bool aligned)
	{
		static uint64 alignment = Context::GetDevice().GetDeviceProperties().limits.minUniformBufferOffsetAlignment;

		MemoryBlock* block = FindSuitableBlock(size, alignment * (aligned ? 1 : 0));

		if (block == nullptr)
			return {};

		block->free = false;
		m_memoryLeft -= block->size;

		MEMORY_LOG("[BufferMemoryPool #%d] Reserve %d in block %p (%d)", m_id, size, block, m_buffer.Get());

		return {
			&m_buffer,
			block->size,
			block->offset,
			block,
			m_id
		};
	}

	void BufferMemoryPool::Free(MemoryBlock* block)
	{
		FreeBlock(block);
	}

	void BufferMemoryPool::Write(MemoryBlock* block, void* data, uint64 size, uint64 offset)
	{
		ASSERT_DEBUG(block == nullptr, "block is nullptr");
		ASSERT_DEBUG(block->size < size, "Trying to write out of the sub buffer (size: %d, tried: %d)", block->size, size);

		m_buffer.Write(data, size, offset + block->offset);
	}

	void BufferMemoryPool::Write(MemoryBlock* block, void* data)
	{
		ASSERT_DEBUG(block == nullptr, "block is nullptr");

		m_buffer.Write(data, block->size, block->offset);
	}
}