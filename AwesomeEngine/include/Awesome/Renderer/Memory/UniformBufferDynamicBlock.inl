namespace aw
{
	inline UniformBufferDynamicBlock::UniformBufferDynamicBlock(UniformBufferDynamicPool* pool, uint64 index, uint64 offset) :
		m_pool(pool),
		m_index(index),
		m_offset(offset)
	{
	}

	inline UniformBufferDynamicBlock::UniformBufferDynamicBlock(UniformBufferDynamicBlock&& other) noexcept :
		m_pool(other.m_pool),
		m_index(other.m_index),
		m_offset(other.m_offset)
	{
		other.Clear();
	}

	inline UniformBufferDynamicBlock::~UniformBufferDynamicBlock()
	{
		Free();
	}

	inline void UniformBufferDynamicBlock::Clear()
	{
		m_pool = nullptr;
		m_index = ~0;
		m_offset = ~0;
	}

	inline uint64 UniformBufferDynamicBlock::GetOffset() const
	{
		return m_offset;
	}

	inline UniformBufferDynamicBlock& UniformBufferDynamicBlock::operator=(UniformBufferDynamicBlock&& other) noexcept
	{
		Free();

		m_pool = other.m_pool;
		m_index = other.m_index;
		m_offset = other.m_offset;

		other.Clear();

		return *this;
	}
}