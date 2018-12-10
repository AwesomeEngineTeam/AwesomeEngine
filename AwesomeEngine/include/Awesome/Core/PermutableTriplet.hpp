#pragma once

#ifndef AWESOME_CORE_PERMUTABLE_TRIPLET_GUARD
#define AWESOME_CORE_PERMUTABLE_TRIPLET_GUARD

namespace aw
{
	template<typename T>
	struct PermutableTriplet
	{
		T first;
		T second;
		T third;

		PermutableTriplet() = default;
		inline explicit PermutableTriplet(const T& _first, const T& _second, const T& _third);
		inline PermutableTriplet(const PermutableTriplet<T>&) = default;
		inline PermutableTriplet(PermutableTriplet<T>&&) = default;

		PermutableTriplet<T>& operator=(const PermutableTriplet<T>& rhs) = default;
		PermutableTriplet<T>& operator=(PermutableTriplet<T>&& rhs) = default;

		inline bool operator==(const PermutableTriplet<T>& rhs) const;
		inline bool operator!=(const PermutableTriplet<T>& rhs) const;
	};
}

#include <Awesome/Core/PermutableTriplet.inl>

#endif // GUARD