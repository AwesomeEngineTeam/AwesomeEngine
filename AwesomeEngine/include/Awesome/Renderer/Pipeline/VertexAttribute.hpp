#pragma once

#ifndef AWESOME_RENDERER_VERTEX_ATTRIBUTE_GUARD
#define AWESOME_RENDERER_VERTEX_ATTRIBUTE_GUARD

#include <Awesome/Core/Tools.hpp>
#include <Awesome/Core/Types.hpp>

#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>

#include <array>

namespace aw
{
	constexpr const vk::Format VertexAttributeFormats[] = {
		vk::Format::eR32Sfloat,
		vk::Format::eR32G32Sfloat,
		vk::Format::eR32G32B32Sfloat,
		vk::Format::eR32G32B32A32Sfloat,
		vk::Format::eR8G8B8A8Unorm,
	};

	struct VertexAttribute
	{
		uint16 formatIndex{ 0 };
		uint8 size{ 0 };
		uint8 id{ 0 };

		VertexAttribute() = default;
		inline VertexAttribute(uint16 formatIndex, uint8 size, uint8 id);
		inline explicit VertexAttribute(uint32 attributeInteger);
		VertexAttribute(const VertexAttribute&) = default;
		VertexAttribute(VertexAttribute&&) = default;
		~VertexAttribute() = default;

		VertexAttribute& operator=(const VertexAttribute&) = default;
		VertexAttribute& operator=(VertexAttribute&&) = default;

		inline bool operator==(const VertexAttribute& rhs);
	};

	inline constexpr uint32 VertexAttributeToInteger(uint16 formatIndex, uint8 size, uint8 id);

	inline constexpr uint8 GetSizeFromIntegerAttribute(uint32 attribute);

	template<uint16 formatIndex>
	inline constexpr uint8 GetSizeFromFormat();

	template<uint32 attributeDescriptionInteger>
	static inline constexpr auto VertexAttributeToType();
}

#include <Awesome/Renderer/Pipeline/VertexAttribute.inl>

#endif //GUARD