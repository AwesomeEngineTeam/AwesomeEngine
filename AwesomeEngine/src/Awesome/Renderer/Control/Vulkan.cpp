#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Core/Logger/Logger.hpp>

namespace aw
{
	void* Vulkan::s_library = nullptr;

	bool Vulkan::LoadVulkanLibrary()
	{
		s_library = LoadLibrary(TEXT("vulkan-1.dll"));

		return s_library != nullptr;
	}

	bool Vulkan::LoadGlobalLevelEntryPoints()
	{
		#define VK_GLOBAL_LEVEL_FUNCTION(f)													\
		if ((f = (PFN_##f)GetProcAddress(static_cast<HMODULE>(s_library), #f )) == nullptr)	\
			LOG_ERROR("Could not load function " #f);

		#include <Awesome/Renderer/Control/ListOfFunctions.inl>

		return true;
	}

	bool Vulkan::LoadInstanceLevelEntryPoints()
	{
		#define VK_INSTANCE_LEVEL_FUNCTION(f)												\
		if ((f = (PFN_##f)vkGetInstanceProcAddr(Context::GetInstance(), #f)) == nullptr)	\
			LOG_ERROR("Could not load function " #f);

		#include <Awesome/Renderer/Control/ListOfFunctions.inl>

		return true;
	}

	bool Vulkan::LoadDeviceLevelEntryPoints()
	{
		#define VK_DEVICE_LEVEL_FUNCTION(f)													\
		if ((f = (PFN_##f)vkGetDeviceProcAddr(Context::GetDevice().Get(), #f)) == nullptr)	\
			LOG_ERROR("Could not load function " #f);

		#include <Awesome/Renderer/Control/ListOfFunctions.inl>

		return true;
	}
}