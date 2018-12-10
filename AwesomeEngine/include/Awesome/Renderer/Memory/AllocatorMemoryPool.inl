namespace aw
{
	inline Allocator::AllocatorMemoryPool::AllocatorMemoryPool(AllocatorMemoryPool&& other) noexcept :
		MemoryPool(std::move(other))
	{
		m_deviceMemory = std::move(other.m_deviceMemory);

		m_memTypeId = other.m_memTypeId;
		m_memPtr = other.m_memPtr;
		m_id = other.m_id;

		other.m_memPtr = nullptr;
	}

	inline Allocator::AllocatorMemoryPool::~AllocatorMemoryPool()
	{
		if (m_memPtr != nullptr)
			m_deviceMemory.Unmap();
	}

	inline uint64 Allocator::AllocatorMemoryPool::Id() const
	{
		return m_id;
	}

	inline uint16 Allocator::AllocatorMemoryPool::MemTypeBits() const
	{
		return 1 << m_memTypeId;
	}

	inline Allocator::AllocatorMemoryPool& Allocator::AllocatorMemoryPool::operator=(AllocatorMemoryPool&& rhs) noexcept
	{
		MemoryPool::operator=(std::move(rhs));

		m_deviceMemory = std::move(rhs.m_deviceMemory);

		m_memTypeId = rhs.m_memTypeId;
		m_memPtr = rhs.m_memPtr;
		m_id = rhs.m_id;

		rhs.m_memPtr = nullptr;

		return *this;
	}
}