#pragma once

#ifndef AWESOME_RENDERER_CONFIG_GUARD
#define AWESOME_RENDERER_CONFIG_GUARD

#ifdef AWESOME_RENDERER_DLL_EXPORTS
#define AWESOME_RENDERER_API __declspec(dllexport)
#else
#define AWESOME_RENDERER_API __declspec(dllimport)
#endif

/*
#if defined(AWESOME_RENDERER_DLL_EXPORTS)
	#define AWESOME_RENDERER_API __declspec(dllexport)
#elif defined(AWESOME_RENDERER_BUILD_AS_DLL)
	#define AWESOME_RENDERER_API __declspec(dllimport)
#else
	#define AWESOME_RENDERER_API extern
#endif
*/

#define VULKAN_VALIDATION_LAYERS 1

#endif //GUARD
