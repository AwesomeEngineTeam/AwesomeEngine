#include <array>

namespace aw
{
	template<uint32 attr, uint32... attrs>
	inline StaticVertex<attr, attrs...>::StaticVertex(float data[])
	{
		std::memcpy(this->data, data, Size);
	}

	template<uint32 attr, uint32 ...attrs>
	inline StaticVertex<attr, attrs...>::StaticVertex(const StaticVertex& vertex)
	{
		std::memcpy(data, vertex.data, Size);
	}

	template<uint32 attr, uint32 ...attrs>
	inline StaticVertex<attr, attrs...>::StaticVertex(StaticVertex&& vertex) noexcept
	{
		std::memcpy(data, vertex.data, Size);
	}

	template<uint32 attr, uint32 ...attrs>
	constexpr uint16 StaticVertex<attr, attrs...>::AttributeSizeAt(uint16 index)
	{
		std::array<uint32, AttributeCount> sizes = { attr, attrs... };
		return DisassembleRHS<uint16, uint32>(sizes[index]);
	}

	template<uint32 attr, uint32 ...attrs>
	inline bool StaticVertex<attr, attrs...>::operator==(const StaticVertex& vertex) const
	{
		for (uint16 i = 0; i != Size; i++)
			if (data[i] != vertex.data[i])
				return false;
		return true;
	}

	template<uint32 attr, uint32 ...attrs>
	template<uint32 attribute>
	constexpr bool StaticVertex<attr, attrs...>::HasAttribute()
	{
		return PackContains<uint32, attr, attrs...>(attribute);
	}

	template<uint32 attr, uint32 ...attrs>
	template<uint32 attribute, typename T>
//	inline constexpr decltype(VertexAttributeToType<attribute>())& StaticVertex<attr, attrs...>::Get()
	inline constexpr T& StaticVertex<attr, attrs...>::Get()
	{
		static_assert(HasAttribute<attribute>(),
			"StaticVertex struct don't have this attribute");

		uint8 attrsOffset = GetSizeFromIntegerAttribute(SumParamsUntilEquals<uint32, attr, attrs...>(attribute));

//		return *(reinterpret_cast<decltype(VertexAttributeToType<attribute>())*>(&data[attrsOffset]));
		return reinterpret_cast<T&>(data[attrsOffset]);
	}
}
