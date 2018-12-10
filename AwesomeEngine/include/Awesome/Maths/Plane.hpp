#pragma once

#ifndef AWESOME_MATHS_PLANE_GUARD
#define AWESOME_MATHS_PLANE_GUARD

#include <Awesome/Maths/Config.hpp>
#include <Awesome/Maths/Vector3.hpp>

namespace aw
{
	struct Plane
	{
		Vector3 normal{};
		float offset{ 0.0f };

		Plane() = default;
		inline Plane(const Vector3& normal, float offset);
		inline Plane(const Vector3& normal, const Vector3& point);
		~Plane() = default;

		inline Vector3 Constraint(const Vector3& point) const;
	};
}

#include <Awesome/Maths/Plane.inl>

#endif //GUARD