#include <Awesome/Maths/Quaternion.hpp>
#include <Awesome/Maths/Vector3.hpp>
#include <Awesome/Maths/Matrix3.hpp>

namespace aw
{
	Quaternion::Quaternion(const float angle, Vector3 axis) : w{ cosf(angle/2.0f) }
	{
		axis.Normalize();
		axis *= sinf(angle / 2.0f);

		x = axis.x.load();
		y = axis.y.load();
		z = axis.z.load();
	}

	Vector3 Quaternion::EulerAngles() const
	{
		Vector3 res;

		float _CosXCosZ = operator()(1, 1);
		float _CosXSinZ = operator()(1, 0);

		float _CosXCosY = operator()(2, 2);
		float _CosXSinY = operator()(0, 2);

		res.x = atan2f(-operator()(1, 2), sqrtf(Square(_CosXCosY) + Square(_CosXSinY)));
		
		float cosX = cosf(res.x);

		res.y = atan2f(_CosXSinY / cosX, _CosXCosY / cosX);
		res.z = atan2f(_CosXSinZ / cosX, _CosXCosZ / cosX);

		return res * RAD_TO_DEG;
	}

	Matrix3 Quaternion::ToMatrix() const
	{
		return Matrix3(
			operator()(0, 0), operator()(0, 1), operator()(0, 2),
			operator()(1, 0), operator()(1, 1), operator()(1, 2),
			operator()(2, 0), operator()(2, 1), operator()(2, 2)
		);
	}

	Quaternion Quaternion::FromEulerAngles(const Vector3& eulerAngles)
	{
		Quaternion q;

		float cy = cosf(eulerAngles.z * 0.5f);
		float sy = sinf(eulerAngles.z * 0.5f);
		float cr = cosf(eulerAngles.x * 0.5f);
		float sr = sinf(eulerAngles.x * 0.5f);
		float cp = cosf(eulerAngles.y * 0.5f);
		float sp = sinf(eulerAngles.y * 0.5f);

		q.w = cy * cr * cp + sy * sr * sp;
		q.x = cy * sr * cp - sy * cr * sp;
		q.y = cy * cr * sp + sy * sr * cp;
		q.z = sy * cr * cp - cy * sr * sp;

		return q;
	}

    Quaternion Quaternion::FromAxis(float angle, const Vector3& axis)
    {
        return Quaternion(angle, axis);
    }

	Quaternion Quaternion::SLerp(Quaternion start, Quaternion end, float percent)
	{
		float dot = start.Dot(end);

		if (dot > 0.9995f)
			return Lerp(start, end, percent);
		if (dot < 0.0f) 
		{
			end = -end;
			dot = -dot;
		}

		aw::Clamp(percent, 0.0f, 1.0f);
		aw::Clamp(dot, -1.0f, 1.0f);
		float theta = acosf(dot) * percent;

		Quaternion temp = end - start * dot;

		return start * cosf(theta) + temp * sinf(theta);
	}

	Vector3 Quaternion::operator*(const Vector3& rhs) const
	{
		Quaternion normed = GetNormalized();

		Vector3 uv, uuv;
		Vector3 qvec(normed.x, normed.y, normed.z);
		uv = qvec ^ rhs;
		uuv = qvec ^ uv;
		uv *= (2.0f * normed.w);
		uuv *= 2.0f;

		return rhs + uv + uuv;
	}

	Quaternion Quaternion::operator*(const Quaternion& rhs) const
	{
		float resW = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		float resX = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		float resY = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		float resZ = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

		return Quaternion(resW, resX, resY, resZ);
	}

	Quaternion& Quaternion::operator*=(const Quaternion& rhs)
	{
		float resW = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		float resX = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		float resY = w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x;
		float resZ = w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w;

		w = resW;
		x = resX;
		y = resY;
		z = resZ;

		return *this;
	}

	std::atomic<float>& Quaternion::operator[](const uint8 index)
	{
		if (index == 0)
			return w;
		if (index == 1)
			return x;
		else if (index == 2)
			return y;
		else if (index == 3)
			return z;
			
		LOG_ERROR("float& Quaternion[" + std::to_string(index) + "] Element out of Array");
			return x;
	}

	float Quaternion::operator[](const uint8 index) const
	{
		if (index == 0)
			return w;
		if (index == 1)
			return x;
		if (index == 2)
			return y;
		if (index == 3)
			return z;
		
		LOG_ERROR("float Quaternion[" + std::to_string(index) + "] Element out of Array");
			return 0.0f;
	}
}