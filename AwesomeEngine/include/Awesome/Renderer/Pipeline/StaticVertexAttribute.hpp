#pragma once

#ifndef AWESOME_RENDERER_STATIC_VERTEX_ATTRIBUTE_GUARD
#define AWESOME_RENDERER_STATIC_VERTEX_ATTRIBUTE_GUARD

#include <Awesome/Renderer/Pipeline/VertexAttribute.hpp>

namespace aw
{
	template<uint16 formatIndex, uint8 id>
	struct StaticVertexAttribute
	{
		static constexpr const uint16 FormatIndex = formatIndex;
		static constexpr const uint8 Size{ GetSizeFromFormat<formatIndex>() };
		static constexpr const uint8 Id = id;
		static constexpr const uint32 Integer = VertexAttributeToInteger(FormatIndex, Size, Id);

		static constexpr inline VertexAttribute ToDescription();
	};

	using PositionXYZAttribute = StaticVertexAttribute<2, 0>;
	using PositionXYAttribute = StaticVertexAttribute<1, 1>;
	using NormalXYZAttribute = StaticVertexAttribute<2, 2>;
	using NormalXYAttribute = StaticVertexAttribute<1, 3>;
	using TangentXYZAttribute = StaticVertexAttribute<2, 4>;
	using TangentXYZHandednessAttribute = StaticVertexAttribute<3, 5>;
	using TangentXYAttribute = StaticVertexAttribute<1, 6>;
	using TextureUVWAttribute = StaticVertexAttribute<2, 7>;
	using TextureUVAttribute = StaticVertexAttribute<1, 8>;
	using ColorRGBAAttribute = StaticVertexAttribute<4, 9>;
	using HandednessAttribute = StaticVertexAttribute<0, 10>;

	static constexpr const uint32 PositionXYZ(PositionXYZAttribute::Integer);
	static constexpr const uint32 PositionXY(PositionXYAttribute::Integer);
	static constexpr const uint32 NormalXYZ(NormalXYZAttribute::Integer);
	static constexpr const uint32 NormalXY(NormalXYAttribute::Integer);
	static constexpr const uint32 TangentXYZ(TangentXYZAttribute::Integer);
	static constexpr const uint32 TangentXYZHandedness(TangentXYZHandednessAttribute::Integer);
	static constexpr const uint32 TangentXY(TangentXYAttribute::Integer);
	static constexpr const uint32 TextureUVW(TextureUVWAttribute::Integer);
	static constexpr const uint32 TextureUV(TextureUVAttribute::Integer);
	static constexpr const uint32 ColorRGBA(ColorRGBAAttribute::Integer);
	static constexpr const uint32 Handedness(HandednessAttribute::Integer);
}

#include <Awesome/Renderer/Pipeline/StaticVertexAttribute.inl>

#endif //GUARD