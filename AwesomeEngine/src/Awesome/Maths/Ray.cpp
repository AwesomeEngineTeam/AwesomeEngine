#include <Awesome/Maths/Ray.hpp>
#include <Awesome/Maths/Plane.hpp>
#include <Awesome/Maths/Algorithm.hpp>

#include <Awesome/Core/Debug.hpp>

namespace aw
{
	bool Ray::Cast(const struct Plane& plane, Vector3* hitPosition, float* hitDistance) const
	{
		float dDotN = direction.Dot(plane.normal);

		if (dDotN < 0.0f)
			return false;

		float distance = -(origin.Dot(plane.normal) + plane.offset) / dDotN;

		if (hitDistance != nullptr)
			*hitDistance = distance;

		if (hitPosition != nullptr)
			*hitPosition = origin + direction * distance;

		return true;
	}

	bool Ray::CastBidirectional(const struct Plane& plane, Vector3* hitPosition, float* hitDistance) const
	{
		float dDotN = direction.Dot(plane.normal);

		if (NearlyZero(dDotN))
			return false;

		float distance = -(origin.Dot(plane.normal) + plane.offset) / dDotN;

		if (hitDistance != nullptr)
			*hitDistance = distance;

		if (hitPosition != nullptr)
			*hitPosition = origin + direction * distance;

		return true;
	}
}