#pragma once

#ifndef AWESOME_MATHS_MATRIX3x4_GUARD
#define AWESOME_MATHS_MATRIX3x4_GUARD

#include <Awesome/Core/Debug.hpp>

#include <Awesome/Maths/Config.hpp>
#include <Awesome/Maths/Algorithm.hpp>

#include <Awesome/Maths/Vector3.hpp>

namespace aw
{
	struct Quaternion;
	struct Matrix4;

	/**
	ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
	Optimisation for Transform Matrix
	Same as Matrix4 with the last row (0,0,0,1)
	*/
	struct AWESOME_MATHS_API Matrix3x4
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
	#else
		float m00{ 0.0f };
		float m10{ 0.0f };
		float m20{ 0.0f };
		float m01{ 0.0f };
		float m11{ 0.0f };
		float m21{ 0.0f };
		float m02{ 0.0f };
		float m12{ 0.0f };
		float m22{ 0.0f };
		float m03{ 0.0f };
		float m13{ 0.0f };
		float m23{ 0.0f };
	#endif

		Matrix3x4() = default;
		inline Matrix3x4(
			const float _m00, const float _m01, const float _m02, const float _m03,
			const float _m10, const float _m11, const float _m12, const float _m13,
			const float _m20, const float _m21, const float _m22, const float _m23);
		inline Matrix3x4(const Matrix3x4& other);
		inline Matrix3x4(Matrix3x4&& other);
		~Matrix3x4() = default;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Return the inverse of the matrix considering it is a Matrix4 with the last row (0,0,0,1)
		*/
		Matrix3x4 GetInversed() const;

		Vector3 GetRow(uint8 row) const;
		Vector3 GetColumn(uint8 column) const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Inverse the matrix considering it is a Matrix4 with the last row (0,0,0,1)
		*/
		Matrix3x4& Inverse();

	#if AWESOME_MATHS_SAFE
		inline Matrix3x4& Invalidate();
	#endif

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Remove any translation from this matrix 
		Return the translation vector that was initially present.
		*/
		Vector3 ExtractTranslation();

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Remove any rotation from this matrix 
		Return the quaternion that was initially present.
		*/
		Quaternion ExtractRotation();

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Remove any scaling from this matrix (ie magnitude of each column is 1)
		Return the scale vector that was initially present.
		*/
		Vector3 ExtractScaling();

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Return the determinant of the matrix considering it is a Matrix4 
		with the last row (0,0,0,1) (same as determinant of Matrix3)
		*/
		inline float Determinant() const;

		inline Matrix3x4& MultiplyRow(uint8 row, float factor);
		inline Matrix3x4& MultiplyRows(float factor1, float factor2, float factor3);
		inline Matrix3x4& DivideRow(uint8 row, float divisor);
		inline Matrix3x4& DivideRows(float divisor1, float divisor2, float divisor3);
		inline Matrix3x4& MultiplyColumn(uint8 column, float factor);
		inline Matrix3x4& MultiplyColumns(float factor1, float factor2, float factor3, float factor4);
		inline Matrix3x4& DivideColumn(uint8 column, float divisor);
		inline Matrix3x4& DivideColumns(float divisor1, float divisor2, float divisor3, float divisor4);

		inline std::string ToString() const;

		static Matrix3x4 FromColumns(const Vector3& col1, const Vector3& col2, const Vector3& col3, const Vector3& col4 = Vector3::Zero());
		static Matrix3x4 FromRows(const Vector3& row1, const Vector3& row2, const Vector3& row3);

		inline static const Matrix3x4& Zero();

		inline Matrix3x4 operator+() const;
		inline Matrix3x4 operator-() const;
		inline Matrix3x4 operator*(const float rhs) const;
		inline Matrix3x4 operator/(const float rhs) const;
		Vector3 operator*(const Vector3& rhs) const;
		inline Matrix3x4 operator+(const Matrix3x4& rhs) const;
		inline Matrix3x4 operator-(const Matrix3x4& rhs) const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Return the multiplication of the matrices considering 
		they both are Matrix4 with the last row (0,0,0,1)
		*/
		Matrix3x4 operator*(const Matrix3x4& rhs) const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Return the multiplication of the matrices considering
		left handside element is a Matrix4 with the last row (0,0,0,1)
		*/
		Matrix4 operator*(const Matrix4& rhs) const;

		Matrix3x4& operator=(const Matrix3x4& rhs);
		Matrix3x4& operator=(Matrix3x4&& rhs);
		Matrix3x4& operator*=(const float rhs);
		Matrix3x4& operator/=(const float rhs);
		Matrix3x4& operator+=(const Matrix3x4& rhs);
		Matrix3x4& operator-=(const Matrix3x4& rhs);

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Multiply the matrix with another Matrix3x4 considering
		they both are Matrix4 with the last row (0,0,0,1)
		*/
		Matrix3x4& operator*=(const Matrix3x4& rhs);

		inline bool operator==(const Matrix3x4& rhs) const;
		inline bool operator!=(const Matrix3x4& rhs) const;

		operator Matrix4() const;

		float& operator[](const uint8 index);
		float operator[](const uint8 index) const;

		inline float& operator()(const uint8 rowIndex, const uint8 columnIndex);
		inline float operator()(const uint8 rowIndex, const uint8 columnIndex) const;
	};

	inline Matrix3x4 operator*(const float lhs, const Matrix3x4& rhs);
}

#include <Awesome/Maths/Matrix3x4.inl>

#endif //GUARD