#pragma once

#ifndef AWESOME_CORE_PERMUTABLE_DOUBLET_GUARD
#define AWESOME_CORE_PERMUTABLE_DOUBLET_GUARD

namespace aw
{
	template<typename T>
	struct PermutableDoublet
	{
		T first;
		T second;

		PermutableDoublet() = default;
		inline explicit PermutableDoublet(const T& _first, const T& _second);
		inline PermutableDoublet(const PermutableDoublet<T>&) = default;
		inline PermutableDoublet(PermutableDoublet<T>&&) = default;

		PermutableDoublet<T>& operator=(const PermutableDoublet<T>& rhs) = default;
		PermutableDoublet<T>& operator=(PermutableDoublet<T>&& rhs) = default;

		inline bool operator==(const PermutableDoublet<T>& rhs) const;
		inline bool operator!=(const PermutableDoublet<T>& rhs) const;
	};
}

#include <Awesome/Core/PermutableDoublet.inl>

#endif // GUARD