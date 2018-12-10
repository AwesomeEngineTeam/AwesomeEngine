#include <Awesome/Renderer/Memory/UniformBufferDynamicPool.hpp>

namespace aw
{
	uint64 UniformBufferDynamicPool::s_count = 0;

	UniformBufferDynamicBlock UniformBufferDynamicPool::Reserve()
	{
		for (uint64 i{ 0 }; i < m_availables.size(); i++)
		{
			if (m_availables[i] == true)
			{
				m_availables[i] = false;
				m_availableCount--;
				return { this, i, i * m_buffer.GetSource().alignment };
			}
		}

		return {
			nullptr,
			~0,
			~0
		};
	}
}