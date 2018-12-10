#pragma once

#ifndef AWESOME_CORE_CONFIG_GUARD
#define AWESOME_CORE_CONFIG_GUARD

#ifdef AWESOME_CORE_DLL_EXPORTS
#define AWESOME_CORE_API __declspec(dllexport)
#else
#define AWESOME_CORE_API __declspec(dllimport)
#endif

#endif //GUARD
