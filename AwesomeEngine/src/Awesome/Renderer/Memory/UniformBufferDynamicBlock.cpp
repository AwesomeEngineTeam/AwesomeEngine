#include <Awesome/Renderer/Memory/UniformBufferDynamicBlock.hpp>
#include <Awesome/Renderer/Memory/UniformBufferDynamicPool.hpp>

namespace aw
{
	void UniformBufferDynamicBlock::Write(const void* data, uint64 size, uint64 offset)
	{
		ASSERT_DEBUG(m_pool == nullptr, "m_pool is nullptr");

		m_pool->Write(m_index, data, size, offset);
	}

	void UniformBufferDynamicBlock::Write(const void* data)
	{
		ASSERT_DEBUG(m_pool == nullptr, "m_pool is nullptr");

		m_pool->Write(m_index, data);
	}

	void UniformBufferDynamicBlock::Free()
	{
		if (m_pool != nullptr)
		{
			m_pool->Free(m_index);

			Clear();
		}
	}
}