#include <Awesome/Maths/Vector3.hpp>

#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Matrix3.hpp>
#include <Awesome/Maths/Matrix4.hpp>

namespace aw
{
	Vector3::Vector3(const Vector2& other, float _z) :
		x{ other.x },
		y{ other.y },
		z{ _z }
	{
	}

	Vector3 Vector3::Slerp(const Vector3& start, const Vector3& end, float percent)
	{
		aw::Clamp(percent, 0.0f, 1.0f);
		return SlerpUnclamped(start, end, percent);
	}

	Vector3 Vector3::SlerpUnclamped(const Vector3& start, const Vector3& end, float percent)
	{
		float dot = start.Dot(end);

		if (fabs(dot) > 0.9999995f)
			return Vector3::Lerp(start, end, percent);

		float startNorm = start.Norm();
		float endNorm	= end.Norm();

		Vector3 startN	= start / startNorm;
		Vector3 endN	= end	/ endNorm;

		float angle = acosf(dot) * percent;

		return (startN * cosf(angle) + (endN - startN * dot).Normalize() * sinf(angle)) *
			aw::Lerp(startNorm, endNorm, percent);
	}

	Vector3 Vector3::Slerp(const Vector3& start, const Vector3& end, const Vector3& orientation, float percent)
	{
		aw::Clamp(percent, 0.0f, 1.0f);
		return SlerpUnclamped(start, end, orientation, percent);
	}

	Vector3 Vector3::SlerpUnclamped(const Vector3& start, const Vector3& end, const Vector3& orientation, float percent)
	{
		float startNorm = start.Norm();
		float endNorm	= end.Norm();

		Vector3 startN	= start / startNorm;
		Vector3 endN	= end	/ endNorm;

		float dot	= aw::GetClamped(startN.Dot(endN), -1.0f, 1.0f);;
		float norm	= aw::Lerp(startNorm, endNorm, percent);

		if (fabs(dot) > 0.9999995f)
		{
			if (dot < 0.0f)
			{
				if (startN.Cross(orientation) != Vector3::Zero())
					return SlerpIntermediate(startN, orientation.GetNormalized(), endN, percent) * norm;
				else
					return Lerp(start, end, percent);
			}
			else
				return start;
		}

		float angle = acosf(dot) * percent;
		return (startN * cosf(angle) + (endN - startN * dot).Normalize() * sinf(angle)) * norm;
	}

	void Vector3::GramSchmidt(Vector3& v0, Vector3& v1, Vector3& v2)
	{
		v0 = (v0).Normalize();
		v1 = (v1 - v1.Dot(v0) * v0).Normalize();
		v2 = (v2 - v2.Dot(v0) * v0 - v2.Dot(v1) * v1).Normalize();
	}

	Vector3 Vector3::SlerpIntermediate(const Vector3& first, const Vector3& intermediate, const Vector3& last, float percent)
	{
		float dot = first.Dot(intermediate);
		float angle = acosf(dot);

		float ratio = angle / M_PI;

		if (ratio < 1.0f)
		{
			angle *= percent / ratio;
			return (first * cosf(angle) + (intermediate - first * dot).Normalize() * sinf(angle));
		}

		dot = intermediate.Dot(last);
		angle = acosf(dot) * percent;

		return (intermediate * cosf(angle) + (last - intermediate * dot).Normalize() * sinf(angle));
	}

	float Vector3::AngleBetween(const Vector3& v1, const Vector3& v2)
	{
		float divisor = sqrtf(v1.SquaredNorm() * v2.SquaredNorm());

	#if AWESOME_MATHS_SAFE
		if (NearlyEqual(divisor, 0.0f))
		{
			LOG_ERROR("Division by zero");
			return 0.0f;
		}
	#endif

		float alpha = (v1 | v2) / divisor;
		return acosf(aw::GetClamped(alpha, -1.0f, 1.0f));
	}

	float Vector3::AngleSignedBetween(const Vector3& v1, const Vector3& v2, const Vector3& axis)
	{
		Vector3 c = v1 ^ v2;
		float angle = atan2f(c.Norm(), v1 | v2);
		return c.Dot(axis) < 0.0f ? -angle : angle;
	}

	std::atomic<float>& Vector3::operator[](const uint8 index)
	{
		if (index == 0)
			return x;
		if (index == 1)
			return y;
		if (index == 2)
			return z;
			
		LOG_ERROR("float& Vector3[" + std::to_string(index) + "] Element out of Array");
			return x;
	}

	float Vector3::operator[](const uint8 index) const
	{
		if (index == 0)
			return x;
		if (index == 1)
			return y;
		if (index == 2)
			return z;
		
		LOG_ERROR("float Vector3[" + std::to_string(index) + "] Element out of Array");
			return 0.0f;
	}
}