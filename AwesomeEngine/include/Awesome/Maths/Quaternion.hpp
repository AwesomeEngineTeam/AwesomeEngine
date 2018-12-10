#pragma once

#ifndef AWESOME_MATHS_QUATERNION_GUARD
#define AWESOME_MATHS_QUATERNION_GUARD

#include <Awesome/Core/Debug.hpp>
#include <Awesome/Core/Reflexion/Reflect.hpp>

#include <Awesome/Maths/Config.hpp>
#include <Awesome/Maths/Algorithm.hpp>

#include <atomic>

namespace aw
{
	struct Vector3;
	struct Matrix3;

	struct Quaternion
	{
		std::atomic<float> w{ 0.0f };
		std::atomic<float> x{ 0.0f };
		std::atomic<float> y{ 0.0f };
		std::atomic<float> z{ 0.0f };

		Quaternion() = default;
		inline Quaternion(float w, float x, float y, float z);
		AWESOME_MATHS_API Quaternion(const float angle, Vector3 axis);
		inline Quaternion(const Quaternion& other);
		inline Quaternion(Quaternion&& other);
		virtual ~Quaternion() = default;
		
		inline Quaternion GetConjugated() const;
		inline Quaternion GetInversed() const;
		inline Quaternion GetNormalized() const;
		inline Quaternion GetClamped(const Quaternion& min, const Quaternion& max) const;

		inline Quaternion& Conjugate();
		inline Quaternion& Inverse();
		inline Quaternion& Normalize();
		inline Quaternion& Clamp(const Quaternion& min, const Quaternion& max);

	#if AWESOME_MATHS_SAFE
		inline Quaternion& Invalidate();
	#endif
		
		inline float Norm() const;
		inline float SquaredNorm() const;
		inline float Dot(const Quaternion& other) const;
		inline Quaternion Cross(const Quaternion& other) const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Return a Vector3 of angles in degree corresponding to the 
		rotation made by this quaternion 
		*/
		AWESOME_MATHS_API Vector3 EulerAngles() const;

		AWESOME_MATHS_API Matrix3 ToMatrix() const;
		inline std::string ToString() const;


		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Return a Quaternion from a Vector3 of angles in degree, 
		x is the rotation alongside x axis and so on
		*/
		AWESOME_MATHS_API static Quaternion FromEulerAngles(const Vector3& eulerAngles);
		static Quaternion FromAxis(float angle, const Vector3& axis);
		inline static Quaternion Lerp(const Quaternion& start, const Quaternion& end, float percent);
		inline static Quaternion NLerp(const Quaternion& start, const Quaternion& end, float percent);
		AWESOME_MATHS_API static Quaternion SLerp(Quaternion start, Quaternion end, float percent);

		inline static const Quaternion& Zero();
		inline static const Quaternion& Identity();

		inline Quaternion operator+() const;
		inline Quaternion operator-() const;
		inline Quaternion operator*(const float rhs) const;
		inline Quaternion operator/(const float rhs) const;
		AWESOME_MATHS_API Vector3 operator*(const Vector3& rhs) const;
		inline Quaternion operator+(const Quaternion& rhs) const;
		inline Quaternion operator-(const Quaternion& rhs) const;
		AWESOME_MATHS_API Quaternion operator*(const Quaternion& rhs) const;

        /**
        ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
            Cross product
        */
        inline Quaternion operator^(const Quaternion& rhs) const;

        /**
        ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
            Dot product
        */
        inline float operator|(const Quaternion& rhs) const;

		inline Quaternion& operator=(const Quaternion& rhs);
		inline Quaternion& operator=(Quaternion&& rhs);
		inline Quaternion& operator*=(const float rhs);
		inline Quaternion& operator/=(const float rhs);
		inline Quaternion& operator+=(const Quaternion& rhs);
		inline Quaternion& operator-=(const Quaternion& rhs);
		AWESOME_MATHS_API Quaternion& operator*=(const Quaternion& rhs);

		inline bool operator==(const Quaternion& rhs) const;
		inline bool operator!=(const Quaternion& rhs) const;

		AWESOME_MATHS_API std::atomic<float>& operator[](const uint8 index);
		AWESOME_MATHS_API float operator[](const uint8 index) const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Return the (row, column) element of the matrix formed from this quaternion
		*/
		inline float operator()(const uint8 row, const uint8 column) const;

		REFLECT_BEGIN(Quaternion)
			REFLECT_MEMBER(w)
			REFLECT_MEMBER(x)
			REFLECT_MEMBER(y)
			REFLECT_MEMBER(z)
		REFLECT_END()
	};

	inline Quaternion operator*(const float lhs, const Quaternion& rhs);	
}

#include <Awesome/Maths/Quaternion.inl>

#endif //GUARD