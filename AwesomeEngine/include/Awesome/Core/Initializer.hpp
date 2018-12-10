#pragma once

#ifndef AWESOME_CORE_INITIALIZER_GUARD
#define AWESOME_CORE_INITIALIZER_GUARD

namespace aw
{
	template<typename... Args>
	class Initializer
	{
		template <typename...>
		friend class Initializer;

		static int s_refCount;

		template<typename FirstClass, typename... OtherClass>
		inline void InitClass();

		template<typename FirstClass, typename... OtherClass>
		inline void UninitClass();

	public:
		inline Initializer();
		inline ~Initializer();
	};
}

#include <Awesome/Core/Initializer.inl>

#endif //GUARD