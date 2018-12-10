#pragma once

#ifndef AWESOME_MATHS_CONFIG_GUARD
#define AWESOME_MATHS_CONFIG_GUARD

//#ifdef AWESOME_MATHS_DLL_EXPORTS
//#define AWESOME_MATHS_API __declspec(dllexport)
//#else
//#define AWESOME_MATHS_API __declspec(dllimport)
//#endif

#define AWESOME_MATHS_API

#ifdef _DEBUG
#define AWESOME_MATHS_SAFE 1
#else
#define AWESOME_MATHS_SAFE 0
#endif

#define ROW_MAJOR 0

#endif //GUARD