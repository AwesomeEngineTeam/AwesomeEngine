#include <Awesome/Renderer/Memory/UniformBufferLayout.hpp>

namespace aw
{
	uint32 UniformBufferLayout::GetSize() const
	{
		return uint32(m_attributes.size());
	}

	const UniformBufferLayout::Member& UniformBufferLayout::GetAttributeAt(uint32 index) const
	{
		return m_attributes[index];
	}

	std::string UniformBufferLayout::GetName() const
	{
		return m_name;
	}
}