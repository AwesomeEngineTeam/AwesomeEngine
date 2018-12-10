#pragma once

#ifndef AWESOME_MATHS_VECTOR3_GUARD
#define AWESOME_MATHS_VECTOR3_GUARD

#include <Awesome/Core/Debug.hpp>
#include <Awesome/Core/Meta.hpp>
#include <Awesome/Core/Reflexion/Reflect.hpp>

#include <Awesome/Maths/Config.hpp>
#include <Awesome/Maths/Algorithm.hpp>

#include <atomic>

namespace aw
{
	struct Vector2;

	struct Vector3
	{
		std::atomic<float> x{ 0.0f };
		std::atomic<float> y{ 0.0f };
		std::atomic<float> z{ 0.0f };

		Vector3() = default;
		inline explicit Vector3(const float scale);
		inline Vector3(const float _x, const float _y, const float _z);
		inline Vector3(const Vector3& other);
		AWESOME_MATHS_API explicit Vector3(const Vector2& other, const float _z = 0.0f);
		inline Vector3(Vector3&& other);
		~Vector3() = default;

		inline Vector3 GetNormalized() const;
		inline Vector3 GetClamped(const Vector3& vMin, const Vector3& vMax) const;

		inline Vector3& Normalize();
		inline Vector3&	Clamp(const Vector3& vMin, const Vector3& vMax);

	#if AWESOME_MATHS_SAFE
		inline Vector3& Invalidate();
	#endif

		inline float Norm() const;
		inline float SquaredNorm() const;
		inline float Dot(const Vector3& other) const;
		inline float PerpDot(const Vector3& other) const;
		inline Vector3 Cross(const Vector3& other) const;
		inline Vector3 DoubleCross(const Vector3& other) const;

		inline std::string ToString() const;

		inline static Vector3 Lerp(const Vector3& start, const Vector3& end, float percent);
		inline static Vector3 LerpUnclamped(const Vector3& start, const Vector3& end, float percent);
		inline static Vector3 Nlerp(const Vector3& start, const Vector3& end, float percent);
		inline static Vector3 NlerpUnclamped(const Vector3& start, const Vector3& end, float percent);
		AWESOME_MATHS_API static Vector3 Slerp(const Vector3& start, const Vector3& end, float percent);
		AWESOME_MATHS_API static Vector3 SlerpUnclamped(const Vector3& start, const Vector3& end, float percent);
		AWESOME_MATHS_API static Vector3 Slerp(const Vector3& start, const Vector3& end, const Vector3& orientation, float percent);
		AWESOME_MATHS_API static Vector3 SlerpUnclamped(const Vector3& start, const Vector3& end, const Vector3& orientation, float percent);
		AWESOME_MATHS_API static void GramSchmidt(Vector3& v1, Vector3& v2, Vector3& v3);

	private:
		AWESOME_MATHS_API static Vector3 SlerpIntermediate(const Vector3& first, const Vector3& intermediate, const Vector3& last, float percent);

	public:
		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Returns the vector from param1 to param2
		*/
		inline static Vector3 FromTo(const Vector3& from, const Vector3& to);
		inline static float	DistanceBetween(const Vector3& v1, const Vector3& v2);

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
			Returns the positive angle in radian between param1 and param2
		*/
		AWESOME_MATHS_API static float AngleBetween(const Vector3& v1, const Vector3& v2);

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
			Returns the signed angle in radian between param1 and param2
		*/
		AWESOME_MATHS_API static float AngleSignedBetween(const Vector3& v1, const Vector3& v2, const Vector3& axis);

		inline static const Vector3& Zero();
		inline static const Vector3& One();
		inline static const Vector3& Right();
		inline static const Vector3& Left();
		inline static const Vector3& Up();
		inline static const Vector3& Down();
		inline static const Vector3& Forward();
		inline static const Vector3& Backward();

		inline Vector3 operator+() const;
		inline Vector3 operator-() const;
		inline Vector3 operator*(const float rhs) const;
		inline Vector3 operator/(const float rhs) const;
		inline Vector3 operator+(const Vector3& rhs) const;
		inline Vector3 operator-(const Vector3& rhs) const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
			Cross product
		*/
		inline Vector3 operator^(const Vector3& rhs) const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
			Dot product
		*/
		inline float operator|(const Vector3& rhs) const;

		inline Vector3& operator=(const Vector3& rhs);
		inline Vector3& operator=(Vector3&& rhs);
		inline Vector3& operator*=(const float rhs);
		inline Vector3& operator/=(const float rhs);
		inline Vector3& operator+=(const Vector3& rhs);
		inline Vector3& operator-=(const Vector3& rhs);

		inline bool operator==(const Vector3& rhs) const;
		inline bool operator!=(const Vector3& rhs) const;

		AWESOME_MATHS_API std::atomic<float>& operator[](const uint8 index);
		AWESOME_MATHS_API float operator[](const uint8 index) const;

		REFLECT_BEGIN(Vector3)
			REFLECT_MEMBER(x)
			REFLECT_MEMBER(y)
			REFLECT_MEMBER(z)
		REFLECT_END()
	};

	inline Vector3 operator*(const float lhs, const Vector3& rhs);

    REGISTER_CLASS_BEGIN(Vector3, 3)
        REGISTER_FIELD(x)
        REGISTER_FIELD(y)
        REGISTER_FIELD(z)
    REGISTER_CLASS_END
}

#include <Awesome/Maths/Vector3.inl>

#endif //GUARD