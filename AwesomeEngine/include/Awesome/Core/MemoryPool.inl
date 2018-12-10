namespace aw
{
	inline MemoryPool::MemoryPool(MemoryPool&& other) noexcept
	{
		m_blocks = std::move(other.m_blocks);

		m_size = other.m_size;
		m_memoryLeft = other.m_memoryLeft;
	}

	inline void MemoryPool::Init(uint64 size)
	{
		m_memoryLeft = m_size = size;
	}

	inline uint64 MemoryPool::MemoryLeft() const
	{
		return m_memoryLeft;
	}

	inline uint64 MemoryPool::BiggestFreeBlock() const
	{
		if (m_biggestFreeBlock == 0)
			return m_memoryLeft;

		return m_biggestFreeBlock;
	}

	inline MemoryPool& MemoryPool::operator=(MemoryPool&& rhs) noexcept
	{
		m_blocks = std::move(rhs.m_blocks);

		m_size = rhs.m_size;
		m_memoryLeft = rhs.m_memoryLeft;

		return *this;
	}

	inline uint64 MemoryPool::GetRequiredPadding(MemoryBlock* prev, uint64 alignment) const
	{
		return alignment - ((prev->offset + prev->size) % alignment);
	}
}