#pragma once

#ifndef AWESOME_MATHS_RAY_GUARD
#define AWESOME_MATHS_RAY_GUARD

#include <Awesome/Maths/Config.hpp>
#include <Awesome/Maths/Vector3.hpp>

namespace aw
{
	struct Ray
	{
		Vector3 origin{};
		Vector3 direction{};

		Ray() = default;
		inline Ray(const Vector3& origin, const Vector3& direction);
		~Ray() = default;

		AWESOME_MATHS_API bool Cast(const struct Plane&, Vector3* hitPosition = nullptr, float* hitDistance = nullptr) const;
		AWESOME_MATHS_API bool CastBidirectional(const struct Plane&, Vector3* hitPosition = nullptr, float* hitDistance = nullptr) const;
	};
}

#include <Awesome/Maths/Ray.inl>

#endif //GUARD