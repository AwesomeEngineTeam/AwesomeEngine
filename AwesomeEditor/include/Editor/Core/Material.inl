namespace ed
{
	inline Material::Material(aw::uint32 index, MaterialType type) :
		m_index(index),
		m_type(type)
	{
	}

	inline MaterialType Material::GetType() const
	{
		return m_type;
	}
}