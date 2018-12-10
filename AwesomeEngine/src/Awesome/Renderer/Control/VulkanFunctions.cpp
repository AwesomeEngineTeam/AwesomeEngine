#include <Awesome/Renderer/Control/VulkanFunctions.hpp>

#define AWESOME_FORCE_VULKAN_FUNCTION
	#define VK_GLOBAL_LEVEL_FUNCTION( fun ) PFN_##fun fun;
	#define VK_INSTANCE_LEVEL_FUNCTION( fun ) PFN_##fun fun;
	#define VK_DEVICE_LEVEL_FUNCTION( fun ) PFN_##fun fun;

	#include <Awesome/Renderer/Control/ListOfFunctions.inl>
#undef AWESOME_FORCE_VULKAN_FUNCTION