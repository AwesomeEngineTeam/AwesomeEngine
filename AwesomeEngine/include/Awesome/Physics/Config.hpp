#pragma once

#ifndef AWESOME_PHYSICS_CONFIG_GUARD
#define AWESOME_PHYSICS_CONFIG_GUARD

#ifdef AWESOME_PHYSICS_DLL_EXPORTS
#define AWESOME_PHYSICS_API __declspec(dllexport)
#else
#define AWESOME_PHYSICS_API __declspec(dllimport)
#endif

#define AWESOME_PHYSICS_USE_GPU 1

#endif //GUARD