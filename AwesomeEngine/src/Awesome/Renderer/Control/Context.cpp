#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Memory/Allocator.hpp>
#include <Awesome/Renderer/Memory/BufferManager.hpp>

#include <set>

namespace aw
{
	vk::UniqueInstance Context::s_instance;
	Device Context::s_device;
	bool Context::LogVulkan = true;

	vk::UniqueCommandPool Context::s_commandPool;

#ifdef _DEBUG
	vk::UniqueDebugReportCallbackEXT Context::s_callback;
#endif

	bool Context::s_shouldQuit = false;

	void Context::Init()
	{
		LOG_LOG("Initializing context...");

		ASSERT(Vulkan::LoadVulkanLibrary() == false, "Failed to load Vulkan library");
		ASSERT(Vulkan::LoadGlobalLevelEntryPoints() == false, "Failed to init Vulkan");

		vk::ApplicationInfo appInfo;
		appInfo.pApplicationName = "NoName";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "AwesomeEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		std::vector<const char*> extensions = {
			"VK_KHR_surface",
			"VK_KHR_win32_surface"
		};

		vk::InstanceCreateInfo createInfo;
		createInfo.pApplicationInfo = &appInfo;

		#ifdef _DEBUG
		constexpr const std::array<const char*, 4> validationLayers{
			{
				"VK_LAYER_GOOGLE_threading",
				"VK_LAYER_LUNARG_standard_validation",
				//"VK_LAYER_LUNARG_api_dump",
				"VK_LAYER_LUNARG_parameter_validation",
				//"VK_LAYER_LUNARG_object_tracker",
				"VK_LAYER_LUNARG_core_validation",
				//"VK_LAYER_GOOGLE_unique_objects",
			}
		};
		extensions.push_back("VK_EXT_debug_report");

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		#endif

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		s_instance = vk::createInstanceUnique(createInfo);

		ASSERT(Vulkan::LoadInstanceLevelEntryPoints() == false, "Failed to init Vulkan");

		#ifdef _DEBUG
		vk::DebugReportCallbackCreateInfoEXT callbackCreateInfo;
		callbackCreateInfo.flags =
			vk::DebugReportFlagBitsEXT::eError |
			vk::DebugReportFlagBitsEXT::eWarning |
			vk::DebugReportFlagBitsEXT::eInformation |
			vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eDebug;
		callbackCreateInfo.pfnCallback = &DebugCallback;

		s_callback = s_instance->createDebugReportCallbackEXTUnique(callbackCreateInfo);
		#endif

		LOG_LOG("Context initialized !");
	}

	void Context::UnInit()
	{
		s_commandPool.reset();

		BufferManager::Clear();
		Allocator::ClearMemory();

		s_device.Destroy();

		#ifdef _DEBUG
		s_callback.reset();
		#endif

		s_instance.reset();

		LOG_LOG("Context uninitialized !");
	}

	void Context::CreateDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR renderSurface)
	{
		if (s_device.Get())
			return;

		vk::PhysicalDeviceFeatures deviceFeatures{};

		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.fillModeNonSolid = VK_TRUE;
		deviceFeatures.independentBlend = VK_TRUE;

		vk::DeviceCreateInfo deviceCreateInfo;
		deviceCreateInfo.enabledExtensionCount = uint32(s_requiredExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = s_requiredExtensions.data();
		deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

		s_device.Create(physicalDevice, deviceCreateInfo, renderSurface);

		vk::CommandPoolCreateInfo cmdPoolInfo(
			vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			s_device.GetGraphicsQueueFamily()
		);

		s_commandPool = s_device.Get().createCommandPoolUnique(cmdPoolInfo);

		Allocator::Init(); //sets bufferImageGranularity
	}

	vk::Format Context::FindSupportedFormat(vk::PhysicalDevice physicalDevice, const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features)
	{
		for (vk::Format format : candidates)
		{
			vk::FormatProperties props = physicalDevice.getFormatProperties(format);

			if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		return vk::Format::eUndefined;
	}

	#ifdef _DEBUG
	VKAPI_ATTR VkBool32 VKAPI_CALL Context::DebugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64 obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData)
	{
		if (LogVulkan == true)
		{
			LOG_NONE("       %s %d validation layer: obj: %d, location: %d %s\n", layerPrefix, code, obj, location, msg);

			if (code != 0)
			{
				//system("pause");
				//LOG_STACK_TRACE(Severity::Error);
				LOG_NONE("TODO Log stacktrace");
				//exit(1);
			}
		}

		return VK_FALSE;
	}
	#endif
}