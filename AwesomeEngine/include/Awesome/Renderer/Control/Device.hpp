#pragma once

#ifndef AWESOME_RENDERER_DEVICE_GUARD
#define AWESOME_RENDERER_DEVICE_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Logger/Logger.hpp>

#include <algorithm> // TODO use our clamp
#include <vector>

namespace aw
{
	class AWESOME_RENDERER_API Device
	{
	public:
		struct QueueFamilyIndices
		{
			int graphicsFamily = -1;
			int presentFamily = -1;

			inline bool IsComplete()
			{
				return graphicsFamily >= 0 && presentFamily >= 0;
			}
		};

		struct SwapchainSupportDetails
		{
			vk::SurfaceCapabilitiesKHR capabilities;
			std::vector<vk::SurfaceFormatKHR> formats;
			std::vector<vk::PresentModeKHR> presentModes;

			vk::SurfaceFormatKHR ChooseSwapSurfaceFormat() const
			{
				ASSERT(formats.empty(), "No surface formats compatible with the device found");

				if (formats.size() == 1 && formats[0].format == vk::Format::eUndefined)
					//return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
					return { vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };

				for (const vk::SurfaceFormatKHR& availableFormat : formats)
				{
					if (availableFormat.format == vk::Format::eB8G8R8A8Unorm &&
						availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
					{
						return availableFormat;
					}
				}

				return formats[0];
			}

			vk::PresentModeKHR ChooseSwapPresentMode() const
			{
				vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

				for (const vk::PresentModeKHR& availablePresentMode : presentModes)
				{
					if (availablePresentMode == vk::PresentModeKHR::eMailbox)
					{
						return availablePresentMode;
					}
					else if (availablePresentMode == vk::PresentModeKHR::eImmediate)
					{
						bestMode = availablePresentMode;
					}
				}

				return bestMode;
			}

			vk::Extent2D ChooseSwapExtent(uint32_t width, uint32_t height) const
			{
				#undef max
				if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
				{
					return capabilities.currentExtent;
				}
				else
				{
					vk::Extent2D actualExtent = { width, height };

					actualExtent.width = std::clamp( // TODO use our clamp
						actualExtent.width,
						capabilities.minImageExtent.width,
						capabilities.maxImageExtent.width
					);

					actualExtent.height = std::clamp( // TODO use our clamp
						actualExtent.height,
						capabilities.minImageExtent.height,
						capabilities.maxImageExtent.height
					);

					return actualExtent;
				}
			}
		};

	private:
		vk::UniqueDevice m_handle;
		vk::PhysicalDevice m_physicalDevice;

		QueueFamilyIndices m_queueFamilyIndices;

		vk::PhysicalDeviceProperties m_deviceProperties;
		vk::PhysicalDeviceMemoryProperties m_memoryProperties;

		vk::Queue m_graphicsQueue;
		vk::Queue m_presentQueue;
		vk::UniqueFence m_fence;

	public:
		Device() = default;
		Device(const Device&) = delete;
		Device(Device&&) = delete;
		inline ~Device();

		inline vk::Device Get() const;

		void Create(vk::PhysicalDevice physicalDevice, vk::DeviceCreateInfo createInfo, vk::SurfaceKHR surface);
		void Destroy();

		static bool IsPhysicalDeviceSuitable(vk::PhysicalDevice, vk::SurfaceKHR);
		static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice, vk::SurfaceKHR);
		static bool CheckExtensionSupport(vk::PhysicalDevice);
		static SwapchainSupportDetails QuerySwapchainSupport(vk::PhysicalDevice, vk::SurfaceKHR);

		inline uint32 GetGraphicsQueueFamily() const;
		inline uint32 GetPresentQueueFamily() const;

		void Submit(vk::ArrayProxy<const vk::SubmitInfo> submits, vk::Fence fence) const;
		void Submit(uint32 submitCount, const vk::SubmitInfo* submits, vk::Fence fence) const;

		bool Present(const vk::PresentInfoKHR& present) const;

		void WaitIdle() const;
		void WaitForFences(vk::ArrayProxy<const vk::Fence> fences, bool waitAll, uint64 timeout) const;
		void WaitGraphicsIdle() const;
		void WaitPresentIdle() const;

		inline vk::Queue GetGraphicsQueue() const;
		inline vk::Queue GetPresentQueue() const;
		inline vk::Fence GetFence() const;

		inline const QueueFamilyIndices& GetQueueFamilyIndices() const;

		inline const vk::PhysicalDeviceMemoryProperties& GetMemoryProperties() const;
		inline const vk::PhysicalDeviceProperties& GetDeviceProperties() const;

		inline vk::MemoryRequirements GetMemoryRequirements(vk::Buffer buffer) const;
		inline vk::MemoryRequirements GetMemoryRequirements(vk::Image image) const;

		inline const vk::PhysicalDevice GetPhysicalDevice() const;

		Device& operator=(const Device&) = delete;
		Device& operator=(Device&&) = delete;
	};
}

#include <Awesome/Renderer/Control/Device.inl>

#endif //GUARD