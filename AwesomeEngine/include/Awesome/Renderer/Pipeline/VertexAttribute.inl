namespace aw
{
	inline VertexAttribute::VertexAttribute(uint16 formatIndex, uint8 size, uint8 id) :
			formatIndex{ formatIndex },
			size{ size },
			id{ id }
	{
	}

	inline VertexAttribute::VertexAttribute(uint32 attributeInteger)
	{
		formatIndex = uint16(attributeInteger >> 16u);
		size = uint8(attributeInteger >> 8u);
		id = uint8(attributeInteger);
	}

	inline bool VertexAttribute::operator==(const VertexAttribute& rhs)
	{
		return (formatIndex == rhs.formatIndex) &&
			(size == rhs.size) &&
			(id == rhs.id);
	}

	inline constexpr uint32 VertexAttributeToInteger(uint16 formatIndex, uint8 size, uint8 id)
	{
		uint32 f = formatIndex;
		uint32 s = size;
		uint32 i = id;

		return (f << 16) | (s << 8) | i;
	}

	inline constexpr uint8 GetSizeFromIntegerAttribute(uint32 attribute)
	{
		return uint8(attribute >> 8);
	}

	template<uint16 formatIndex>
	inline constexpr uint8 GetSizeFromFormat()
	{
		if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR32Sfloat)
		{
			return 4;
		}
		else if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR32G32Sfloat)
		{
			return 8;
		}
		else if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR32G32B32Sfloat)
		{
			return 12;
		}
		else if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR32G32B32A32Sfloat)
		{
			return 16;
		}
		else if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR8G8B8A8Unorm)
		{
			return 4;
		}

		//TODO Remove hard-coded value for max formatIndex value
		static_assert(formatIndex < 5, "Invalid Format Index");
	}

	template<uint32 attributeDescriptionInteger>
	inline constexpr auto VertexAttributeToType()
	{
		constexpr uint16 formatIndex = uint16(attributeDescriptionInteger >> 16);

		if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR32Sfloat)
		{
			return float();
		}
		else if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR32G32Sfloat)
		{
			return Vector2(0, 0);
		}
		else if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR32G32B32Sfloat)
		{
			return Vector3(0, 0, 0);
		}
		else if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR32G32B32A32Sfloat)
		{
			return std::array<float, 4>({ {0.0f} });
		}
		else if constexpr (VertexAttributeFormats[formatIndex] == vk::Format::eR8G8B8A8Unorm)
		{
			return uint32();
		}

		//TODO Remove hard-coded value for max formatIndex value
		static_assert(formatIndex < 5, "Invalid Format Index");
	}
}