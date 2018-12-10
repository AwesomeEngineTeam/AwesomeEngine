#pragma once

#ifndef AWESOME_MATHS_MATRIX4_GUARD
#define AWESOME_MATHS_MATRIX4_GUARD

#include <Awesome/Core/Debug.hpp>
#include <Awesome/Core/Reflexion/Reflect.hpp>

#include <Awesome/Maths/Config.hpp>
#include <Awesome/Maths/Algorithm.hpp>
#include <Awesome/Maths/Vector3.hpp>

namespace aw
{
	struct Quaternion;
	struct Matrix3;
	struct Matrix3x4;

	struct AWESOME_MATHS_API Matrix4
	{
	#if (ROW_MAJOR == 1)
		float m00{ 0.0f };
		float m01{ 0.0f };
		float m02{ 0.0f };
		float m03{ 0.0f };
		float m10{ 0.0f };
		float m11{ 0.0f };
		float m12{ 0.0f };
		float m13{ 0.0f };
		float m20{ 0.0f };
		float m21{ 0.0f };
		float m22{ 0.0f };
		float m23{ 0.0f };
		float m30{ 0.0f };
		float m31{ 0.0f };
		float m32{ 0.0f };
		float m33{ 0.0f };
	#else
		float m00{ 0.0f };
		float m10{ 0.0f };
		float m20{ 0.0f };
		float m30{ 0.0f };
		float m01{ 0.0f };
		float m11{ 0.0f };
		float m21{ 0.0f };
		float m31{ 0.0f };
		float m02{ 0.0f };
		float m12{ 0.0f };
		float m22{ 0.0f };
		float m32{ 0.0f };
		float m03{ 0.0f };
		float m13{ 0.0f };
		float m23{ 0.0f };
		float m33{ 0.0f };
	#endif

		Matrix4() = default;
		inline Matrix4(
			const float _m00, const float _m01, const float _m02, const float _m03,
			const float _m10, const float _m11, const float _m12, const float _m13,
			const float _m20, const float _m21, const float _m22, const float _m23,
			const float _m30, const float _m31, const float _m32, const float _m33);
		inline Matrix4(const float[16]);
		inline Matrix4(const Matrix4& other);
		explicit Matrix4(const Matrix3& other);
		inline Matrix4(Matrix4&& other);
		~Matrix4() = default;

		Matrix4 GetInversed() const;
		inline Matrix4 GetTransposed() const;
		Vector3 GetRow(uint8 row);
		Vector3 GetColumn(uint8 column);

		Matrix4& Inverse();
		inline Matrix4& Transpose();
		inline Matrix4& DiscardTranslation();

	#if AWESOME_MATHS_SAFE
		inline Matrix4& Invalidate();
	#endif

		float Determinant() const;
		inline float Trace() const;

		Vector3 Project(const Vector3&) const;
		Vector3 ProjectAndDivideByW(const Vector3&) const;

		bool IsOrthogonal() const;
		inline bool IsHomogenous() const;

		inline std::string ToString() const;

		static Matrix4 FromColumns(const Vector3& col1, const Vector3& col2, const Vector3& col3, const Vector3& col4 = Vector3::Zero());
		static Matrix4 FromRows(const Vector3& row1, const Vector3& row2, const Vector3& row3, const Vector3& row4 = Vector3::Zero());
		static Matrix4 MakeTranslation(const Vector3& pos);
		static Matrix4 MakeRotation(const Quaternion& rot);
		static Matrix4 MakeRotation(const Vector3& eulerAngles);
		static Matrix4 MakeScale(const Vector3& scale);
		static Matrix4 MakePerspective(float angle, float ratio, float zNear, float zFar);

		inline static const Matrix4& Zero();
		inline static const Matrix4& Identity();

		inline Matrix4 operator+() const;
		inline Matrix4 operator-() const;
		inline Matrix4 operator*(const float rhs) const;
		inline Matrix4 operator/(const float rhs) const;
		Vector3 operator*(const Vector3& rhs) const;
		inline Matrix4 operator+(const Matrix4& rhs) const;
		inline Matrix4 operator-(const Matrix4& rhs) const;
		Matrix4 operator*(const Matrix4& rhs) const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Return the multiplication of the matrices considering
		right handside element is a Matrix4 with the last row (0,0,0,1)
		*/
		Matrix4 operator*(const Matrix3x4& rhs) const;

		Matrix4& operator=(const Matrix4& rhs);
		Matrix4& operator=(Matrix4&& rhs);
		Matrix4& operator*=(const float rhs);
		Matrix4& operator/=(const float rhs);
		Matrix4& operator+=(const Matrix4& rhs);
		Matrix4& operator-=(const Matrix4& rhs);
		Matrix4& operator*=(const Matrix4& rhs);

		inline bool operator==(const Matrix4& rhs) const;
		inline bool operator!=(const Matrix4& rhs) const;

		float& operator[](const uint8 index);
		float operator[](const uint8 index) const;

		inline float& operator()(const uint8 rowIndex, const uint8 columnIndex);
		inline float operator()(const uint8 rowIndex, const uint8 columnIndex) const;

		REFLECT_BEGIN(Matrix4)
			REFLECT_MEMBER(m00)
			REFLECT_MEMBER(m10)
			REFLECT_MEMBER(m20)
			REFLECT_MEMBER(m30)
			REFLECT_MEMBER(m01)
			REFLECT_MEMBER(m11)
			REFLECT_MEMBER(m21)
			REFLECT_MEMBER(m31)
			REFLECT_MEMBER(m02)
			REFLECT_MEMBER(m12)
			REFLECT_MEMBER(m22)
			REFLECT_MEMBER(m32)
			REFLECT_MEMBER(m03)
			REFLECT_MEMBER(m13)
			REFLECT_MEMBER(m23)
			REFLECT_MEMBER(m33)
		REFLECT_END()
	};

	inline Matrix4 operator*(const float lhs, const Matrix4& rhs);
}

#include <Awesome/Maths/Matrix4.inl>

#endif //GUARD