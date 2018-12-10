namespace aw
{
	inline void UniformBufferDynamicPool::Init(uint64 size, uint64 count)
	{
		m_id = s_count++;

		m_buffer.Create(size, count);
		m_availables.resize(count, true);
		m_availableCount = count;
	}
	
	inline UniformBufferDynamicPool::UniformBufferDynamicPool(UniformBufferDynamicPool&& other) noexcept
	{
		m_id = other.m_id;
		m_buffer = std::move(other.m_buffer);
		m_availables = std::move(other.m_availables);
		m_availableCount = other.m_availableCount;
	}

	void UniformBufferDynamicPool::Free(uint64 blockIndex)
	{
		m_availableCount++;
		m_availables[blockIndex] = true;
	}

	void UniformBufferDynamicPool::Write(uint64 blockIndex, const void* data, uint64 size, uint64 offset)
	{
		m_buffer.Write(blockIndex, data, size, offset);
	}

	void UniformBufferDynamicPool::Write(uint64 blockIndex, const void* data)
	{
		m_buffer.Write(blockIndex, data);
	}

	inline uint64 UniformBufferDynamicPool::Id() const
	{
		return m_id;
	}

	inline UniformBufferDynamic& UniformBufferDynamicPool::UniformBufferDynamic()
	{
		return m_buffer;
	}

	inline uint64 UniformBufferDynamicPool::AvailableBlockCount()
	{
		return m_availableCount;
	}

	inline UniformBufferDynamicPool& UniformBufferDynamicPool::operator=(UniformBufferDynamicPool&& rhs) noexcept
	{
		m_id = rhs.m_id;
		m_buffer = std::move(rhs.m_buffer);
		m_availables = std::move(rhs.m_availables);
		m_availableCount = rhs.m_availableCount;

		return *this;
	}
}