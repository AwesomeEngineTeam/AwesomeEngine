#pragma once

#ifndef AWESOME_RENDERER_IMAGE_VIEW_GUARD
#define AWESOME_RENDERER_IMAGE_VIEW_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>

#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Resource/Image.hpp>

namespace aw
{
	class AWESOME_RENDERER_API ImageView
	{
		VkImageView m_handle{ VK_NULL_HANDLE };

	public:
		ImageView() = default;
		inline ImageView(const VkImageViewCreateInfo* createInfo);
		inline ImageView(const Image& image, ImageViewType type, Format format, ComponentMapping swizzle, ImageAspectBit aspectMask);
		ImageView(const ImageView&) = delete;
		inline ImageView(ImageView&&);
		inline ~ImageView();

		void Create(const VkImageViewCreateInfo* createInfo);
		inline void Create(const Image& image, ImageViewType type, Format format, ComponentMapping swizzle, ImageAspectBit aspectMask, uint32 layerCount = 1);
		void Destroy();

		ImageView& operator=(const ImageView&) = delete;
		ImageView& operator=(ImageView&&) = delete;
	};

	static_assert(sizeof(ImageView) == sizeof(VkImageView), "handle and wrapper have different size!");
}

#include <Awesome/Renderer/ImageView.inl>

#endif //GUARD