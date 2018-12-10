namespace aw
{
	inline VertexLayout::VertexLayout(const std::vector<uint32>& attributes)// : m_attributes{attributes}
	{
		m_attributes.resize(attributes.size());

		for (uint32 i = 0; i < uint32(attributes.size()); i++)
			m_attributes[i] = VertexAttribute(attributes[i]);
	}

	inline void VertexLayout::PushAttribute(VertexAttribute attribute)
	{
		m_attributes.push_back(attribute);
	}

	inline VertexAttribute VertexLayout::GetAttributeAt(uint32 index) const
	{
		return m_attributes[index];
	}

	inline uint64 VertexLayout::GetAttributeCount() const
	{
		return m_attributes.size();
	}

	inline bool VertexLayout::operator==(const VertexLayout& other) const
	{
		if (m_attributes.size() != other.m_attributes.size())
			return false;

		return std::memcmp(m_attributes.data(), other.m_attributes.data(), m_attributes.size() * sizeof(VertexAttribute)) == 0;
	}

	inline bool VertexLayout::operator!=(const VertexLayout& other) const
	{
		return !operator==(other);
	}
}