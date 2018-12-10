#include <Awesome/Renderer/Pipeline/VertexLayout.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertexAttribute.hpp>

namespace aw
{
	uint32 VertexLayout::GetSize() const
	{
		uint32 size = 0;

		for (VertexAttribute attribute : m_attributes)
			size += attribute.size;

		return size;
	}

	uint32 VertexLayout::GetAttributeOffset(VertexAttribute attribute) const
	{
		uint32 offset = 0;

		for (VertexAttribute a : m_attributes)
		{
			if (a == attribute)
				return offset;

			offset += a.size;
		}

		return ~0u;
	}

	vk::VertexInputBindingDescription VertexLayout::CreateBindingDescription(uint32 binding, bool forInstantiation) const
	{
		vk::VertexInputBindingDescription bindingDescription{
			binding,																			// uint32_t             binding;
			GetSize(),																			// uint32_t             stride;
			forInstantiation ? vk::VertexInputRate::eInstance : vk::VertexInputRate::eVertex	// VkVertexInputRate    inputRate; 
		};

		return bindingDescription;
	}

	std::vector<vk::VertexInputAttributeDescription> VertexLayout::CreateAttributeDescriptions(uint32 binding) const
	{
		std::vector<vk::VertexInputAttributeDescription> descriptions;

		uint32 offset = 0;
		for (uint32 i = 0; i < m_attributes.size(); i++)
		{
			uint32 attributeSize = m_attributes[i].size;
			uint32 formatIndex = m_attributes[i].formatIndex;

			vk::VertexInputAttributeDescription attributeDescription{
				i,										// uint32_t    location;
				binding,								// uint32_t    binding;
				VertexAttributeFormats[formatIndex],	// VkFormat    format;
				offset									// uint32_t    offset; 
			};

			descriptions.push_back(attributeDescription);

			offset += attributeSize;
		}

		return descriptions;
	}
}