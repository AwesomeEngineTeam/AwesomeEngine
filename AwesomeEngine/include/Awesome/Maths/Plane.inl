namespace aw
{
	inline Plane::Plane(const Vector3& normal, float offset) :
		normal(normal),
		offset(offset)
	{
	}

	inline Plane::Plane(const Vector3& normal, const Vector3& point) :
		normal(normal),
		offset(-(point.x * normal.x + point.y * normal.y + point.z * normal.z))
	{
	}

	inline Vector3 Plane::Constraint(const Vector3& point) const
	{
		float dist = point.Dot(normal);

		Vector3 nDist = normal * dist;

		Vector3 constrainedPoint = point - nDist;

		constrainedPoint += normal * offset;

		return constrainedPoint;
	}
}