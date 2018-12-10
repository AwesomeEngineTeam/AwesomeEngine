#pragma once

#ifndef AWESOME_RENDERER_VERTEX_LAYOUT_GUARD
#define AWESOME_RENDERER_VERTEX_LAYOUT_GUARD

#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Pipeline/VertexAttribute.hpp>

#include <Awesome/Core/Tools.hpp>

#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>

#include <vector>

namespace aw
{
	class VertexLayout
	{
		std::vector<VertexAttribute> m_attributes;

	public:
		VertexLayout() = default;
		inline explicit VertexLayout(const std::vector<uint32>& attributes);
		VertexLayout(const VertexLayout&) = default;
		VertexLayout(VertexLayout&&) = default;
		~VertexLayout() = default;

		inline void PushAttribute(VertexAttribute attribute);

		AWESOME_RENDERER_API uint32 GetSize() const;

		AWESOME_RENDERER_API uint32 GetAttributeOffset(VertexAttribute attribute) const;
		inline VertexAttribute GetAttributeAt(uint32 index) const;
		inline uint64 GetAttributeCount() const;

		AWESOME_RENDERER_API vk::VertexInputBindingDescription CreateBindingDescription(uint32 binding, bool forInstantiation = false) const;
		AWESOME_RENDERER_API std::vector<vk::VertexInputAttributeDescription> CreateAttributeDescriptions(uint32 binding) const;

		inline bool operator==(const VertexLayout& other) const;
		inline bool operator!=(const VertexLayout& other) const;

		VertexLayout& operator=(const VertexLayout&) = default;
		VertexLayout& operator=(VertexLayout&&) = default;
	};
}

#include <Awesome/Renderer/Pipeline/VertexLayout.inl>

#endif //GUARD