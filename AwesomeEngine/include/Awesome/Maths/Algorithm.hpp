#pragma once

#ifndef AWESOME_MATHS_ALGORITHM_GUARD
#define AWESOME_MATHS_ALGORITHM_GUARD

#include <cmath>

namespace aw
{
#ifndef M_PI
#define M_PI 3.14159265f
#endif

#ifndef M_PI2
#define M_PI2 1.57079632f
#endif

#ifndef M_PI4
#define M_PI4 0.78539816f
#endif

#ifndef M_2PI
#define M_2PI 6.28318530f
#endif

#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.01745329f
#endif

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.29577951f
#endif

#ifndef SQRT2
#define SQRT2 1.41421356f
#endif

#ifndef SQRT3
#define SQRT3 1.73205080f
#endif

#ifndef FLT_EPSILON
#define FLT_EPSILON 1.192092896E-7f
#endif

	inline constexpr void Clamp(float& value, const float min, const float max);
	inline constexpr float GetClamped(const float value, const float min, const float max);
	inline constexpr float DegreeToRadian(const float angleDegree);
	inline constexpr float RadianToDegree(const float angleRadian);
	inline constexpr float Lerp(const float start, const float end, float percent);
	inline constexpr float Square(const float a);
	inline float InvSqrt(const float a);
	inline constexpr float SumSquare(const float last);

	template<typename... Args>
	inline constexpr float SumSquare(const float first, const Args&... others);

	inline float CopySign(float x, float y);
	inline constexpr float Sign(float a);
	inline bool NearlyZero(float a, float tolerance = FLT_EPSILON);
	inline bool NearlyEqual(float a, float b, float tolerance = FLT_EPSILON);
}

#include <Awesome/Maths/Algorithm.inl>

#endif //GUARD