#pragma once

#ifndef AWESOME_RENDERER_VULKAN_FUNCTION_GUARD
#define AWESOME_RENDERER_VULKAN_FUNCTION_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Config.hpp>

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#define AWESOME_FORCE_VULKAN_FUNCTION
	#define VK_GLOBAL_LEVEL_FUNCTION( fun) AWESOME_RENDERER_API extern PFN_##fun fun;
	#define VK_INSTANCE_LEVEL_FUNCTION( fun ) AWESOME_RENDERER_API extern PFN_##fun fun;
	#define VK_DEVICE_LEVEL_FUNCTION( fun ) AWESOME_RENDERER_API extern PFN_##fun fun;

	#include <Awesome/Renderer/Control/ListOfFunctions.inl>
#undef AWESOME_FORCE_VULKAN_FUNCTION

#endif //GUARD