namespace aw
{
	inline Buffer::Buffer(vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, uint32 size, bool concurent)
	{
		Create(usage, properties, size, concurent);
	}

	inline Buffer::Buffer(Buffer&& other) noexcept
	{
		if (m_handle)
			Destroy();

		m_handle.swap(other.m_handle);
		m_memInfo = std::move(other.m_memInfo);
		//m_memInfo.id = other.m_memInfo.id;
		//m_memInfo.poolID = other.m_memInfo.poolID;
		//m_memInfo.memPtr = other.m_memInfo.memPtr;

		////other.m_handle = nullptr;
		//other.m_memInfo.id = 0;
		//other.m_memInfo.poolID = 0;
		//other.m_memInfo.memPtr = nullptr;
	}

	inline Buffer::~Buffer()
	{
		Destroy();
	}
	
	inline vk::Buffer Buffer::Get() const
	{
		return m_handle.get();
	}

	inline Buffer& Buffer::operator=(Buffer&& rhs) noexcept
	{
		if (m_handle)
			Destroy();

		m_handle.swap(rhs.m_handle);
		m_memInfo = std::move(rhs.m_memInfo);
		//m_memInfo.id = rhs.m_memInfo.id;
		//m_memInfo.poolID = rhs.m_memInfo.poolID;
		//m_memInfo.memPtr = rhs.m_memInfo.memPtr;

		////rhs.m_handle = nullptr;
		//rhs.m_memInfo.id = 0;
		//rhs.m_memInfo.poolID = 0;
		//rhs.m_memInfo.memPtr = nullptr;

		return *this;
	}
}