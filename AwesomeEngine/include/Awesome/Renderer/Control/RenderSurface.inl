namespace aw
{
	inline RenderSurface::RenderSurface(RenderSurface&& other) :
		m_handle{ std::move(other.m_handle) },
		m_swapchain{ std::move(other.m_swapchain) },
		m_extent{ other.m_extent },
		m_images{ std::move(other.m_images) }
	{
	}

	inline RenderSurface::RenderSurface(const vk::Win32SurfaceCreateInfoKHR& createInfo)
	{
		Create(createInfo);
	}

	inline RenderSurface::~RenderSurface()
	{
		Destroy();
	}

	inline void RenderSurface::SetOnOutOfDateCallback(const std::function<void()>& callback)
	{
		m_onOutOfDate = callback;
	}

	inline vk::SwapchainKHR RenderSurface::GetSwapchain() const
	{
		return m_swapchain.get();
	}

	inline vk::SurfaceKHR RenderSurface::GetSurface() const
	{
		return m_handle.get();
	}

	inline const vk::Extent2D& RenderSurface::GetExtent() const
	{
		return m_extent;
	}

	inline uint32 RenderSurface::GetImageCount() const
	{
		return uint32(m_images.size());
	}

	inline vk::Format RenderSurface::GetPresentFormat()
	{
		return s_presentFormat;
	}

	inline vk::Format RenderSurface::GetDepthFormat()
	{
		return s_depthFormat;
	}

	inline const std::vector<TextureHandle>& RenderSurface::GetImages() const
	{
		return m_images;
	}
}