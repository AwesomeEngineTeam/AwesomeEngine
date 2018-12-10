#pragma once

#ifndef AWESOME_MATHS_VECTOR2_GUARD
#define AWESOME_MATHS_VECTOR2_GUARD

#include <Awesome/Core/Debug.hpp>

#include <Awesome/Maths/Config.hpp>
#include <Awesome/Maths/Algorithm.hpp>

namespace aw
{
	struct Vector3;

	struct Vector2
	{
		float x{ 0.0f };
		float y{ 0.0f };

		Vector2() = default;
		inline explicit Vector2(float scale);
		inline Vector2(float _x, float _y);
		inline Vector2(const Vector2& other);
		AWESOME_MATHS_API explicit Vector2(const Vector3& other);
		inline Vector2(Vector2&& other) noexcept;
		~Vector2() = default;

		inline Vector2 GetNormalized() const;
		inline Vector2 GetClamped(const Vector2& vMin, const Vector2& vMax) const;
		
		inline Vector2&	Normalize();
		inline Vector2&	Clamp(const Vector2& vMin, const Vector2& vMax);

	#if AWESOME_MATHS_SAFE
	private:
		inline Vector2& Invalidate();

	public:
	#endif
		
		inline float Norm() const;
		inline float SquaredNorm() const;
		inline float Dot(const Vector2& other) const;
		
		inline std::string ToString() const;

		inline static Vector2 Lerp(const Vector2& start, const Vector2& end, float percent);
		inline static Vector2 LerpUnclamped(const Vector2& start, const Vector2& end, float percent);
		inline static Vector2 Nlerp(const Vector2& start, const Vector2& end, float percent);
		inline static Vector2 NlerpUnclamped(const Vector2& start, const Vector2& end, float percent);
		inline static Vector2 Slerp(const Vector2& start, const Vector2& end, float percent);
		inline static Vector2 SlerpUnclamped(const Vector2& start, const Vector2& end, float percent);

		inline static float	DistanceBetween(const Vector2& v1, const Vector2& v2);

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Returns the signed angle in radian between param1 and param2
		*/
		inline static float	AngleBetween(const Vector2& v1, const Vector2& v2);

		inline static const Vector2& Zero();
		inline static const Vector2& One();
		inline static const Vector2& Right();
		inline static const Vector2& Left();
		inline static const Vector2& Up();
		inline static const Vector2& Down();

		inline Vector2 operator+() const;
		inline Vector2 operator-() const;
		inline Vector2 operator*(const float rhs) const;
		inline Vector2 operator/(const float rhs) const;
		inline Vector2 operator+(const Vector2& rhs) const;
		inline Vector2 operator-(const Vector2& rhs) const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Dot product
		*/
		inline float operator|(const Vector2& rhs) const;

		inline Vector2& operator=(const Vector2& rhs);
		inline Vector2& operator=(Vector2&& rhs);
		inline Vector2& operator*=(const float rhs);
		inline Vector2& operator/=(const float rhs);
		inline Vector2& operator+=(const Vector2& rhs);
		inline Vector2& operator-=(const Vector2& rhs);

		inline bool operator==(const Vector2& rhs) const;
		inline bool operator!=(const Vector2& rhs) const;
		
		AWESOME_MATHS_API float& operator[](const uint8 index);
		AWESOME_MATHS_API float operator[](const uint8 index) const;
	};

	inline Vector2 operator*(const float lhs, const Vector2& rhs);
}

#include <Awesome/Maths/Vector2.inl>

#endif //GUARD