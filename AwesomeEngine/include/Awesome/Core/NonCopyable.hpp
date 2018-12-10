#pragma once

#ifndef AWESOME_CORE_NON_COPYABLE_GUARD
#define AWESOME_CORE_NON_COPYABLE_GUARD

#include <Awesome/Core/Config.hpp>

namespace aw
{
	class AWESOME_CORE_API NonCopyable
	{
	protected:
		NonCopyable() = default;
		virtual ~NonCopyable() = default;

		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};
}

#endif //GUARD