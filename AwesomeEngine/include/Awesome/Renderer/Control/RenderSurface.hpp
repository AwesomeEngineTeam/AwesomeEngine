#pragma once

#ifndef AWESOME_RENDERER_RENDER_SURFACE_GUARD
#define AWESOME_RENDERER_RENDER_SURFACE_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Resource/Texture.hpp>
#include <Awesome/Renderer/RenderingResources.hpp>

namespace aw
{
	class RenderSurface
	{
		vk::UniqueSurfaceKHR m_handle;
		vk::UniqueSwapchainKHR m_swapchain;
		vk::Extent2D m_extent;

		std::vector<vk::UniqueImageView> m_uniqueImageViews;
		std::vector<TextureHandle> m_images;

		//uint32 m_imageIndex{ ~0u };

		std::function<void()> m_onOutOfDate{};

		AWESOME_RENDERER_API static vk::Format s_presentFormat;
		AWESOME_RENDERER_API static vk::Format s_depthFormat;

	public:
		RenderSurface() = default;
		inline RenderSurface(const vk::Win32SurfaceCreateInfoKHR& createInfo);
		RenderSurface(const RenderSurface&) = delete;
		inline RenderSurface(RenderSurface&&);
		inline ~RenderSurface();

		inline void SetOnOutOfDateCallback(const std::function<void()>&);

		AWESOME_RENDERER_API void Create(const vk::Win32SurfaceCreateInfoKHR& createInfo);
		AWESOME_RENDERER_API void Destroy();

		AWESOME_RENDERER_API void InitSwapchain(uint32 width, uint32 height);

		inline vk::SwapchainKHR GetSwapchain() const;
		inline vk::SurfaceKHR GetSurface() const;
		inline const vk::Extent2D& GetExtent() const;

		AWESOME_RENDERER_API uint32 AcquireNextImage(vk::Semaphore semaphore, vk::Fence fence) const;

		inline uint32 GetImageCount() const;
		inline static vk::Format GetPresentFormat();
		inline static vk::Format GetDepthFormat();
		inline const std::vector<TextureHandle>& GetImages() const;

		AWESOME_RENDERER_API vk::PhysicalDevice PickCompatiblePhysicalDevice() const;

		RenderSurface& operator=(const RenderSurface&) = delete;
		RenderSurface& operator=(RenderSurface&&) = delete;
	};
}

#include <Awesome/Renderer/Control/RenderSurface.inl>

#endif //GUARD