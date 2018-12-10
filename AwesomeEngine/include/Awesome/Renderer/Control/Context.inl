namespace aw
{
	// inline vk::Extent2D Context::GetExtent(uint32 width, uint32 height)
	// {
	// 	const Device::SwapchainSupportDetails& swapchainSupport = Device::QuerySwapchainSupport(s_device.GetPhysicalDevice(), reinterpret_cast<vk::SurfaceKHR&>(s_sampleRenderSurface));
	// 	return swapchainSupport.ChooseSwapExtent(width, height);
	// }

	inline vk::Instance Context::GetInstance()
	{
		return s_instance.get();
	}

	inline const Device& Context::GetDevice()
	{
		return s_device;
	}

	inline vk::CommandPool Context::GetCommandPool()
	{
		return s_commandPool.get();
	}

	// inline vk::Format Context::GetDepthFormat()
	// {
	// 	return s_depthFormat;
	// }

	// inline vk::Format Context::GetFormat()
	// {
	// 	return s_format;
	// }

	inline constexpr const std::array<const char*, 1>& Context::GetRequiredExtensions()
	{
		return s_requiredExtensions;
	}

	// inline bool Context::ShouldQuit()
	// {
	// 	return s_shouldQuit;
	// }

	inline vk::Format Context::FindDepthFormat(vk::PhysicalDevice physicalDevice)
	{
		return FindSupportedFormat(
			physicalDevice,
			{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
			vk::ImageTiling::eOptimal,
			vk::FormatFeatureFlagBits::eDepthStencilAttachment
		);
	}
}