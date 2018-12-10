namespace aw
{
	inline void UniformBuffer::Create(uint64 size)
	{
		m_srcSub = BufferManager::Reserve(size, false, BufferType::Staging);
		m_dstSub = BufferManager::Reserve(size, true, BufferType::Uniform);
	}

	inline void UniformBuffer::Write(const void* data, uint64 size, uint64 offset)
	{
		m_srcSub.Write(data, size, offset);
	}

	inline void UniformBuffer::Write(const void* data)
	{
		m_srcSub.Write(data);
	}

	inline void UniformBuffer::Copy(vk::CommandBuffer commandBuffer)
	{
		vk::BufferCopy copy;
		copy.size = m_srcSub.size;
		copy.srcOffset = m_srcSub.offset;
		copy.dstOffset = m_dstSub.offset;

		if (m_srcSub.size < m_dstSub.size)
		{
			LOG_WARNING("Copy will not fill entirely the destination");
		}
		else if (m_dstSub.size < m_srcSub.size)
		{
			LOG_WARNING("Destination does not have enough space for the full copy");
			copy.size = m_dstSub.size;
		}

		commandBuffer.copyBuffer(m_srcSub.buffer->Get(), m_dstSub.buffer->Get(), copy);
	}

	inline SubBuffer& UniformBuffer::GetDestination()
	{
		return m_dstSub;
	}

	inline const SubBuffer& UniformBuffer::GetDestination() const
	{
		return m_dstSub;
	}

	inline void UniformBuffer::SetLayout(UniformBufferLayout layout)
	{
		m_layout = std::move(layout);
	}
}