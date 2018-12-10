#include <Awesome/Renderer/Control/Device.hpp>
#include <Awesome/Renderer/Control/Context.hpp>

#include <set>

namespace aw
{
	void Device::Create(vk::PhysicalDevice physicalDevice, vk::DeviceCreateInfo createInfo, vk::SurfaceKHR surface)
	{
		m_queueFamilyIndices = FindQueueFamilies(physicalDevice, surface);

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		std::set<int> uniqueQueueFamilies = { m_queueFamilyIndices.graphicsFamily, m_queueFamilyIndices.presentFamily };

		float queuePriority = 1.0f;

		for (int queueFamily : uniqueQueueFamilies)
		{
			vk::DeviceQueueCreateInfo queueCreateInfo;
			queueCreateInfo.queueFamilyIndex = uint32(queueFamily);
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		m_handle = physicalDevice.createDeviceUnique(createInfo);

		ASSERT(Vulkan::LoadDeviceLevelEntryPoints() == false, "Failed to init Vulkan");

		m_deviceProperties = physicalDevice.getProperties();
		m_memoryProperties = physicalDevice.getMemoryProperties();

		m_physicalDevice = physicalDevice;

		LOG_LOG("Device (%p) interface for %s created.", m_handle.get(), m_deviceProperties.deviceName);

		m_graphicsQueue = m_handle->getQueue(m_queueFamilyIndices.graphicsFamily, 0);
		m_presentQueue = m_handle->getQueue(m_queueFamilyIndices.presentFamily, 0);
		m_fence = m_handle->createFenceUnique(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
	}

	void Device::Destroy()
	{
		m_fence.reset();
		m_handle.reset();
	}

	bool Device::IsPhysicalDeviceSuitable(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
	{
		vk::PhysicalDeviceFeatures physicalDeviceFeatures = physicalDevice.getFeatures();

		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice, surface);

		bool extensionsSupported = CheckExtensionSupport(physicalDevice);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapchainSupportDetails swapChainSupport = QuerySwapchainSupport(physicalDevice, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return queueFamilyIndices.IsComplete() && extensionsSupported && swapChainAdequate && physicalDeviceFeatures.samplerAnisotropy;
	}

	Device::QueueFamilyIndices Device::FindQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
	{
		QueueFamilyIndices indices;

		std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

		uint32 i = 0;
		for (const auto& queueFamily : queueFamilies)
		{
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphicsFamily = i;
			}

			vk::Bool32 presentSupport = VK_FALSE;

			physicalDevice.getSurfaceSupportKHR(i, surface, &presentSupport);

			if (queueFamily.queueCount > 0 && presentSupport)
			{
				indices.presentFamily = i;
			}

			if (indices.IsComplete())
			{
				break;
			}

			i++;
		}

		return indices;
	}

	bool Device::CheckExtensionSupport(vk::PhysicalDevice physicalDevice)
	{
		std::vector<vk::ExtensionProperties> availableExtensions = physicalDevice.enumerateDeviceExtensionProperties();
		
		const auto& exts = Context::GetRequiredExtensions();
		std::set<std::string> requiredExtensions(exts.begin(), exts.end());

		for (const VkExtensionProperties& extension : availableExtensions)
			requiredExtensions.erase(extension.extensionName);

		return requiredExtensions.empty();
	}

	Device::SwapchainSupportDetails Device::QuerySwapchainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface)
	{
		SwapchainSupportDetails details;
		
		details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

		details.formats = physicalDevice.getSurfaceFormatsKHR(surface);
		
		details.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
		
		return details;
	}

	void Device::Submit(vk::ArrayProxy<const vk::SubmitInfo> submits, vk::Fence fence) const
	{
		m_graphicsQueue.submit(submits, fence);
	}

	void Device::Submit(uint32 submitCount, const vk::SubmitInfo* submits, vk::Fence fence) const
	{
		m_graphicsQueue.submit(submitCount, submits, fence);
	}

	bool Device::Present(const vk::PresentInfoKHR& present) const
	{
		return m_presentQueue.presentKHR(&present) == vk::Result::eSuccess;
	}

	void Device::WaitIdle() const
	{
		m_handle->waitIdle();
	}

	void Device::WaitForFences(vk::ArrayProxy<const vk::Fence> fences, bool waitAll, uint64 timeout) const
	{
		m_handle->waitForFences(fences, waitAll, timeout);
	}

	void Device::WaitGraphicsIdle() const
	{
		m_graphicsQueue.waitIdle();
	}

	void Device::WaitPresentIdle() const
	{
		m_presentQueue.waitIdle();
	}
}

#undef CLASS