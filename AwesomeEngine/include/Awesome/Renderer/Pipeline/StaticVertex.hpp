#pragma once

#ifndef AWESOME_RENDERER_STATIC_VERTEX_GUARD
#define AWESOME_RENDERER_STATIC_VERTEX_GUARD

#include <Awesome/Core/Tools.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertexAttribute.hpp>
#include <Awesome/Renderer/Pipeline/VertexLayout.hpp>

namespace aw
{
	template<uint32 attr, uint32... attrs>
	struct StaticVertex
	{
		static constexpr const uint16 AttributeCount = sizeof...(attrs)+1;
		static constexpr const uint8 Size = GetSizeFromIntegerAttribute(Sum(attr, attrs...));

		static const VertexLayout Layout;

		uint8 data[Size]{};

		StaticVertex() = default;
		inline explicit StaticVertex(float data[]);
		inline StaticVertex(const StaticVertex& vertex);
		inline StaticVertex(StaticVertex&& vertex) noexcept;
		~StaticVertex() = default;

		constexpr static uint16 AttributeSizeAt(uint16 index);

		template<uint32 attribute>
		inline static constexpr bool HasAttribute();

		template<uint32 attribute, typename T>
		inline constexpr T& Get();

		inline bool operator==(const StaticVertex& vertex) const;

		StaticVertex& operator=(const StaticVertex&) = default;
		StaticVertex& operator=(StaticVertex&&) noexcept = default;
	};

	typedef StaticVertex<PositionXYZ, TextureUV, NormalXYZ, TangentXYZ, Handedness> VertexPTNTaH;
	typedef StaticVertex<PositionXYZ, TextureUV, NormalXYZ, TangentXYZ> VertexPTNTa;
	typedef StaticVertex<PositionXYZ, TextureUV, NormalXYZ> VertexPTN;
	typedef StaticVertex<PositionXY, TextureUV, ColorRGBA> VertexPTC;
	typedef StaticVertex<PositionXYZ, TextureUV> VertexPT;
	typedef StaticVertex<PositionXY, TextureUV> VertexP2T;
	typedef StaticVertex<PositionXYZ, TextureUVW> VertexPT3;
	typedef StaticVertex<PositionXYZ, NormalXYZ> VertexPN;
	typedef StaticVertex<PositionXYZ, ColorRGBA> VertexPC;
	typedef StaticVertex<PositionXYZ> VertexP;

	template<uint32 attr, uint32... attrs>
	const VertexLayout StaticVertex<attr, attrs...>::Layout{ { attr, attrs... } };
}

#include <Awesome/Renderer/Pipeline/StaticVertex.inl>

#endif //GUARD