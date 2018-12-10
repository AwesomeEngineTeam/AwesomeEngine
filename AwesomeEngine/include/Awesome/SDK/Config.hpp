#pragma once

#ifndef AWESOME_SDK_CONFIG_GUARD
#define AWESOME_SDK_CONFIG_GUARD

#define AWESOME_SDK_API
#if defined(AWESOME_SDK_STATIC)
	#define AWESOME_SDK_API
#else
#define AWESOME_SDK_API
#endif

#endif //GUARD