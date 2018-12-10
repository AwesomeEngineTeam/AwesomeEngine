namespace aw
{
	inline constexpr float GetClamped(const float value, const float min, const float max)
	{
		return value < min ? min : value > max ? max : value;
	}

	inline constexpr void Clamp(float& value, const float min, const float max)
	{
		value = value < min ? min : value > max ? max : value;
	}

	inline constexpr float DegreeToRadian(const float angleDegree)
	{
		return angleDegree * DEG_TO_RAD;
	}

	inline constexpr float RadianToDegree(const float angleRadian)
	{
		return angleRadian * RAD_TO_DEG;
	}

	inline constexpr float Lerp(const float start, const float end, float percent)
	{
		return (end - start) * percent + start;
	}

	inline constexpr float Square(const float a)
	{
		return a * a;
	}

	inline float InvSqrt(const float a)
	{
		long i = 0l;
		float x2 = 0.0f;
		float y = 0.0f;
		const float threehalfs = 1.5F;

		x2 = a * 0.5F;
		y = a;
		i = *(long *)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float *)&i;
		y = y * (threehalfs - (x2 * y * y));
		y = y * (threehalfs - (x2 * y * y));

		return y;
	}

	inline constexpr float SumSquare(const float last)
	{
		return Square(last);
	}

	template<typename ...Args>
	inline constexpr float SumSquare(const float first, const Args& ...others)
	{
		return Square(first) + SumSquare(float{ others }...);
	}

	inline float CopySign(float x, float y)
	{
		return x * Sign(y);
	}

	inline constexpr float Sign(float a)
	{
		return a > 0.0f ? 1.0f : a < 0.0f ? -1.0f : 0.0f;
	}

	inline bool NearlyZero(float a, float tolerance)
	{
		return fabs(a) < tolerance;
	}

	inline bool NearlyEqual(float a, float b, float tolerance)
	{
		return fabs(a - b) < tolerance;
	}
}