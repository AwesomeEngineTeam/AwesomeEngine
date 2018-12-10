#pragma once

#ifndef AWESOME_WINDOWING_CONFIG_GUARD
#define AWESOME_WINDOWING_CONFIG_GUARD

#ifdef AWESOME_WINDOWING_DLL_EXPORTS
#define AWESOME_WINDOWING_API __declspec(dllexport)
#else
#define AWESOME_WINDOWING_API __declspec(dllimport)
#endif

#endif //GUARD
