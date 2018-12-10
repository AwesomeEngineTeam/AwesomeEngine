#pragma once

#ifndef AWESOME_RENDERER_CONTEXT_GUARD
#define AWESOME_RENDERER_CONTEXT_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Control/Device.hpp>
#include <Awesome/Renderer/Descriptor/DescriptorSetLayoutInfo.hpp>
#include <Awesome/Renderer/Descriptor/DescriptorPool.hpp>

#ifdef _WIN32
#include <windows.h>
#else
#error Platform not supported
#endif

#include <limits>
#include <vector>
#include <array>
#include <unordered_map>

namespace aw
{
	class RenderSurface;

	class AWESOME_RENDERER_API Context
	{
		static vk::UniqueInstance s_instance;
		static Device s_device;

		static constexpr const std::array<const char*, 1> s_requiredExtensions = { { VK_KHR_SWAPCHAIN_EXTENSION_NAME } };

		static vk::UniqueCommandPool s_commandPool;

#ifdef _DEBUG
		static vk::UniqueDebugReportCallbackEXT s_callback;
#endif

		static bool s_shouldQuit;

	public:
		static bool LogVulkan;

		Context() = delete;
		~Context() = delete;

		static void Init();
		static void UnInit();

		static inline vk::Instance GetInstance();
		static inline const Device& GetDevice();
		static inline vk::CommandPool GetCommandPool();

		static inline constexpr const std::array<const char*, 1>& GetRequiredExtensions();

		static void CreateDevice(vk::PhysicalDevice, vk::SurfaceKHR);
		static inline vk::Format FindDepthFormat(vk::PhysicalDevice physicalDevice);
		static vk::Format FindSupportedFormat(vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);

	private:
		#ifdef _DEBUG
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugReportFlagsEXT flags,
			VkDebugReportObjectTypeEXT objType,
			uint64 obj,
			size_t location,
			int32_t code,
			const char* layerPrefix,
			const char* msg,
			void* userData);
		#endif
	};
}

#include <Awesome/Renderer/Control/Context.inl>

#endif //GUARD