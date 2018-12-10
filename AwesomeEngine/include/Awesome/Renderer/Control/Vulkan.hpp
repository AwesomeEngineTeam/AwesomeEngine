#pragma once

#ifndef AWESOME_RENDERER_VULKAN_GUARD
#define AWESOME_RENDERER_VULKAN_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>

//#define VK_NO_PROTOTYPES
//#include <vulkan/vulkan.h>

#include <Awesome/Renderer/Config.hpp>
//#include <Awesome/Renderer/VulkanFunctions.hpp>

#include <Awesome/Renderer/Control/VulkanFunctions.hpp>

#include <vulkan/vulkan.hpp>

namespace aw
{
	class AWESOME_RENDERER_API Vulkan
	{
		static void* s_library;

	public:
		Vulkan() = delete;
		~Vulkan() = delete;

		static bool LoadVulkanLibrary();
		static bool LoadGlobalLevelEntryPoints();
		static bool LoadInstanceLevelEntryPoints();
		static bool LoadDeviceLevelEntryPoints();
	};
}

#endif //GUARD