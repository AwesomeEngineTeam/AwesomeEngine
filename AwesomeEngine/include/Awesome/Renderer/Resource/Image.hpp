#pragma once

#ifndef AWESOME_RENDERER_IMAGE_GUARD
#define AWESOME_RENDERER_IMAGE_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>

#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Memory/MemInfo.hpp>

namespace aw
{
	class Image
	{
		vk::UniqueImage m_handle;
		MemInfo m_memInfo;

	public:
		Image() = default;
		inline Image(const vk::ImageCreateInfo& createInfo, vk::MemoryPropertyFlagBits properties);
		Image(const Image&) = delete;
		Image(Image&& other) = default;
		~Image() = default;

		AWESOME_RENDERER_API void Create(const vk::ImageCreateInfo& createInfo, vk::MemoryPropertyFlagBits properties);
		inline void Create(vk::ImageType type, vk::Format format, const vk::Extent3D& extent, vk::ImageUsageFlagBits usage, vk::MemoryPropertyFlagBits properties, vk::ImageLayout layout, uint32 sampleCount = 1u, uint32 mipLevels = 1u, uint32 arrayLayers = 1u, bool optimal = true, bool concurrent = false);
		AWESOME_RENDERER_API void Destroy();

		inline vk::Image Get() const;

		inline void TransitionLayout(vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlags, vk::AccessFlags srcAccessMask, vk::AccessFlags dstAccessMask);
		static AWESOME_RENDERER_API void TransitionLayout(vk::Image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlags, vk::AccessFlags srcAccessMask, vk::AccessFlags dstAccessMask);

		inline void TransitionLayout(vk::CommandBuffer, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlags, vk::AccessFlags srcAccessMask, vk::AccessFlags dstAccessMask);
		static AWESOME_RENDERER_API void TransitionLayout(vk::CommandBuffer, vk::Image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::ImageAspectFlags, vk::AccessFlags srcAccessMask, vk::AccessFlags dstAccessMask);

		Image& operator=(const Image&) = delete;
		Image& operator=(Image&& other) = default;
	};
}

#include <Awesome/Renderer/Resource/Image.inl>

#endif //GUARD