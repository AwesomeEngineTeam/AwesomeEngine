namespace aw
{
	inline void UniformBufferDynamic::Create(uint64 elementSize, uint64 count)
	{
		uint64 totalSize = AlignedSubBuffer::ComputeFullSize(elementSize, count);
		SubBuffer srcSub = BufferManager::Reserve(totalSize, false, BufferType::Staging);
		SubBuffer dstSub = BufferManager::Reserve(totalSize, true, BufferType::Uniform);

		m_srcSub = std::move(srcSub);
		m_dstSub = std::move(dstSub);

		m_srcSub.elementSize = elementSize;
		m_srcSub.count = count;
		m_srcSub.alignment = m_srcSub.ComputeAlignment(elementSize);

		m_dstSub.elementSize = m_srcSub.elementSize;
		m_dstSub.count = m_srcSub.count;
		m_dstSub.alignment = m_srcSub.alignment;
	}

	inline void UniformBufferDynamic::Write(uint64 index, const void* data, uint64 size, uint64 offset)
	{
		m_srcSub.Write(index, data, size, offset);
	}

	inline void UniformBufferDynamic::Write(uint64 index, const void* data)
	{
		m_srcSub.Write(index, data);
	}

	inline void UniformBufferDynamic::Copy(vk::CommandBuffer commandBuffer)
	{
		vk::BufferCopy copy;
		copy.size = m_srcSub.ComputeFullSize();
		copy.srcOffset = m_srcSub.offset;
		copy.dstOffset = m_dstSub.offset;

		if (m_srcSub.ComputeFullSize() < m_dstSub.ComputeFullSize())
		{
			LOG_WARNING("Copy will not fill entirely the destination");
		}
		else if (m_dstSub.ComputeFullSize() < m_srcSub.ComputeFullSize())
		{
			LOG_WARNING("Destination does not have enough space for the full copy");
			copy.size = m_dstSub.ComputeFullSize();
		}

		commandBuffer.copyBuffer(m_srcSub.buffer->Get(), m_dstSub.buffer->Get(), copy);
	}

	inline AlignedSubBuffer& UniformBufferDynamic::GetDestination()
	{
		return m_dstSub;
	}

	inline const AlignedSubBuffer& UniformBufferDynamic::GetDestination() const
	{
		return m_dstSub;
	}

	inline AlignedSubBuffer& UniformBufferDynamic::GetSource()
	{
		return m_srcSub;
	}

	inline const AlignedSubBuffer& UniformBufferDynamic::GetSource() const
	{
		return m_srcSub;
	}

	inline void UniformBufferDynamic::SetLayout(UniformBufferLayout layout)
	{
		m_layout = std::move(layout);
	}
}