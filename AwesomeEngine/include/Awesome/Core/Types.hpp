#pragma once

#ifndef AWESOME_CORE_TYPES_GUARD
#define AWESOME_CORE_TYPES_GUARD

#ifdef __GNUG__
#include <_mingw.h>
#endif

namespace aw
{
	using int8 = __int8;
	using uint8 = unsigned __int8;
	using int16 = __int16;
	using uint16 = unsigned __int16;
	using int32 = __int32;
	using uint32 = unsigned __int32;
	using int64 = __int64;
	using uint64 = unsigned __int64;
}

#endif //GUARD