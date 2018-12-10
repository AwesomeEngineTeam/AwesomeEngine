namespace aw
{
	inline UniformBufferLayout::UniformBufferLayout(std::vector<UniformBufferLayout::Member> attributes, std::string_view name, bool isArray, uint64 arraySize) :
		m_attributes{ std::move(attributes) },
		m_name{ name },
		m_arraySize{ arraySize },
		m_isArray{ isArray }
	{
	}

	inline bool UniformBufferLayout::IsArray() const
	{
		return m_isArray;
	}

	inline uint64 UniformBufferLayout::GetArraySize() const
	{
		return m_arraySize;
	}
}