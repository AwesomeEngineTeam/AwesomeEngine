//namespace aw
//{
//	inline RenderTarget::RenderTarget(RenderTarget&& other) :
//		m_renderSurface{ std::move(other.m_renderSurface) },
//		m_framebuffers{ std::move(other.m_framebuffers) },
//		m_commandBuffer{ std::move(other.m_commandBuffer) },
//		m_passSecondaryCommandBuffers{ std::move(other.m_passSecondaryCommandBuffers) },
//		m_prePassSecondaryCommandBuffers{ std::move(other.m_prePassSecondaryCommandBuffers) },
//		m_renderPass{ m_renderPass },
//		m_fence{ std::move(other.m_fence) }
//	{
//	}
//
//	inline void RenderTarget::InitRenderSurface(const vk::Win32SurfaceCreateInfoKHR& createInfo)
//	{
//		m_renderSurface.Create(createInfo);
//	}
//
//	inline void RenderTarget::SetPassSecondaryCommandBuffers(std::vector<vk::CommandBuffer> buffers)
//	{
//		m_passSecondaryCommandBuffers = std::move(buffers);
//	}
//
//	inline void RenderTarget::SetPrePassSecondaryCommandBuffers(std::vector<vk::CommandBuffer> buffers)
//	{
//		m_prePassSecondaryCommandBuffers = std::move(buffers);
//	}
//
//	inline void RenderTarget::SetRenderPass(Pass* pass)
//	{
//		m_renderPass = pass;
//	}
//
//	inline const RenderSurface& RenderTarget::GetRenderSurface() const
//	{
//		return m_renderSurface;
//	}
//
//	inline vk::SwapchainKHR RenderTarget::GetSwapchain() const
//	{
//		return m_renderSurface.GetSwapchain();
//	}
//
//	inline const vk::Extent2D& RenderTarget::GetExtent() const
//	{
//		return m_renderSurface.GetExtent();
//	}
//
//	inline const Pass* RenderTarget::GetRenderPass() const
//	{
//		return m_renderPass;
//	}
//
//	inline void RenderTarget::WaitFence() const
//	{
//		Context::GetDevice().Get().waitForFences(m_fence.get(), true, ~0ULL);
//	}
//
//	inline void RenderTarget::ResetFence() const
//	{
//		Context::GetDevice().Get().resetFences(m_fence.get());
//	}
//}