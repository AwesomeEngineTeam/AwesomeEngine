namespace aw
{
	inline BufferMemoryPool::BufferMemoryPool(BufferMemoryPool&& other) noexcept :
		MemoryPool(std::move(other))
	{
		m_buffer = std::move(other.m_buffer);

		m_id = other.m_id;
		m_bufferType = other.m_bufferType;
	}

	inline uint64 BufferMemoryPool::Id() const
	{
		return m_id;
	}

	inline BufferType BufferMemoryPool::GetBufferType() const
	{
		return m_bufferType;
	}

	inline Buffer& BufferMemoryPool::Buffer()
	{
		return m_buffer;
	}

	inline BufferMemoryPool& BufferMemoryPool::operator=(BufferMemoryPool&& rhs) noexcept
	{
		MemoryPool::operator=(std::move(rhs));

		m_buffer = std::move(rhs.m_buffer);

		m_id = rhs.m_id;
		m_bufferType = rhs.m_bufferType;

		return *this;
	}
}