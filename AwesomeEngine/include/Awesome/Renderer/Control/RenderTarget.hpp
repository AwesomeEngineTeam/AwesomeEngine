/*#pragma once

#ifndef AWESOME_RENDERER_RENDER_WINDOW_GUARD
#define AWESOME_RENDERER_RENDER_WINDOW_GUARD

#include <Awesome/Renderer/Control/RenderSurface.hpp>

namespace aw
{
	struct Pass;

	class AWESOME_RENDERER_API RenderTarget
	{
		RenderSurface m_renderSurface;

		std::vector<vk::UniqueFramebuffer> m_framebuffers;
		//std::vector<vk::UniqueCommandBuffer> m_commandBuffers;
		vk::UniqueCommandBuffer m_commandBuffer;
		uint32 m_imageIndex{ ~0u };

		std::vector<vk::CommandBuffer> m_passSecondaryCommandBuffers;
		std::vector<vk::CommandBuffer> m_prePassSecondaryCommandBuffers;

		Pass* m_renderPass{ nullptr };

		vk::UniqueFence m_fence;

	public:
		RenderTarget();
		RenderTarget(const RenderTarget&) = delete;
		inline RenderTarget(RenderTarget&& other);
		~RenderTarget() = default;

		inline void InitRenderSurface(const vk::Win32SurfaceCreateInfoKHR& createInfo);
		void InitSwapchain(const Device& device, uint32 resX, uint32 resY);
		void InitFramebuffers(Pass* renderPass, const std::vector<vk::ImageView>& attachments);

		inline void SetPassSecondaryCommandBuffers(std::vector<vk::CommandBuffer> buffers);
		inline void SetPrePassSecondaryCommandBuffers(std::vector<vk::CommandBuffer> buffers);
		inline void SetRenderPass(Pass* pass);

		void Update(vk::Semaphore imageAvailableSemaphore);
		bool Render(vk::Semaphore imageAvailableSemaphore, vk::Semaphore frameFinishedSemaphore);

		inline const RenderSurface& GetRenderSurface() const;
		inline vk::SwapchainKHR GetSwapchain() const;
		inline const vk::Extent2D& GetExtent() const;
		inline const Pass* GetRenderPass() const;
		inline void WaitFence() const;
		inline void ResetFence() const;
	};
}

#include <Awesome/Renderer/Control/RenderTarget.inl>

#endif //Guard*/