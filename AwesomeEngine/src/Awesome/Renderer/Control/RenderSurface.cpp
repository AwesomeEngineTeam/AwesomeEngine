#include <Awesome/Renderer/Control/RenderSurface.hpp>
#include <Awesome/Renderer/Control/Context.hpp>

namespace aw
{
	vk::Format RenderSurface::s_presentFormat;
	vk::Format RenderSurface::s_depthFormat;

	void RenderSurface::Create(const vk::Win32SurfaceCreateInfoKHR& createInfo)
	{
		m_handle = Context::GetInstance().createWin32SurfaceKHRUnique(createInfo);
	}

	void RenderSurface::Destroy()
	{
		m_swapchain.reset();
		m_handle.reset();
	}

	void RenderSurface::InitSwapchain(uint32 width, uint32 height)
	{
		const Device::SwapchainSupportDetails& swapchainSupport = Device::QuerySwapchainSupport(Context::GetDevice().GetPhysicalDevice(), m_handle.get());

		vk::SurfaceFormatKHR surfaceFormat = swapchainSupport.ChooseSwapSurfaceFormat();
		vk::PresentModeKHR presentMode = swapchainSupport.ChooseSwapPresentMode();
		m_extent = swapchainSupport.ChooseSwapExtent(width, height);

		uint32 imageCount = swapchainSupport.capabilities.minImageCount + 1;
		if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount)
			imageCount = swapchainSupport.capabilities.maxImageCount;

		const Device::QueueFamilyIndices& indices = Context::GetDevice().GetQueueFamilyIndices();
		uint32 queueFamilyIndices[] = { static_cast<uint32>(indices.graphicsFamily), static_cast<uint32>(indices.presentFamily) };

		bool hasMultipleIndices = (indices.graphicsFamily != indices.presentFamily);

		vk::SwapchainCreateInfoKHR createInfo;
		createInfo.setSurface(m_handle.get())
			.setMinImageCount(imageCount)
			.setImageFormat(surfaceFormat.format)
			.setImageColorSpace(surfaceFormat.colorSpace)
			.setImageExtent(m_extent)
			.setImageArrayLayers(1)
			.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferDst)
			.setImageSharingMode(vk::SharingMode(hasMultipleIndices))
			.setQueueFamilyIndexCount(hasMultipleIndices ? 2 : 0)
			.setPQueueFamilyIndices(queueFamilyIndices)
			.setPreTransform(swapchainSupport.capabilities.currentTransform)
			.setPresentMode(presentMode)
			.setClipped(true);

		m_swapchain.reset();
		m_swapchain = Context::GetDevice().Get().createSwapchainKHRUnique(createInfo);

		std::vector<vk::Image> images = Context::GetDevice().Get().getSwapchainImagesKHR(m_swapchain.get());

		for (auto image : images)
		{
			Image::TransitionLayout(image, vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR, vk::ImageAspectFlagBits::eColor, vk::AccessFlagBits(0), vk::AccessFlagBits(0));
		}

		s_presentFormat = createInfo.imageFormat;
		s_depthFormat = Context::FindDepthFormat(Context::GetDevice().GetPhysicalDevice());

		vk::ImageViewCreateInfo imageViewCreateInfo;
		imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
		imageViewCreateInfo.format = s_presentFormat;
		imageViewCreateInfo.subresourceRange = {
			vk::ImageAspectFlagBits::eColor,
			0, 1, 0, 1
		};

		m_uniqueImageViews = std::vector<vk::UniqueImageView>(imageCount);
		m_images.resize(imageCount);
		for (uint8 i = 0; i < imageCount; i++)
		{
			imageViewCreateInfo.image = images[i];
			m_uniqueImageViews[i] = Context::GetDevice().Get().createImageViewUnique(imageViewCreateInfo);
			m_images[i] = TextureHandle(images[i], m_uniqueImageViews[i].get());
		}
	}

	uint32 RenderSurface::AcquireNextImage(vk::Semaphore semaphore, vk::Fence fence) const
	{
		uint32 imageIndex{ 0 };
		vk::Result result = Context::GetDevice().Get().acquireNextImageKHR(m_swapchain.get(), std::numeric_limits<uint64>::max(), semaphore, fence, &imageIndex);

		if (result == vk::Result::eErrorOutOfDateKHR && m_onOutOfDate)
		{
			m_onOutOfDate();

			result = Context::GetDevice().Get().acquireNextImageKHR(m_swapchain.get(), std::numeric_limits<uint64>::max(), semaphore, fence, &imageIndex);

			ASSERT(result != vk::Result::eSuccess, "Could not acquire next image");
		}

		return imageIndex;
	}

	vk::PhysicalDevice RenderSurface::PickCompatiblePhysicalDevice() const
	{
		std::vector<vk::PhysicalDevice> devices = Context::GetInstance().enumeratePhysicalDevices();

		for (const vk::PhysicalDevice& handle : devices)
			if (Device::IsPhysicalDeviceSuitable(handle, m_handle.get()))
				return handle;

		THROW("No suitable physical device found");

		return vk::PhysicalDevice();
	}
}

#undef CLASS