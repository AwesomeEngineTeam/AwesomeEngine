#pragma once

#ifndef AWESOME_MATHS_MATRIX3_GUARD
#define AWESOME_MATHS_MATRIX3_GUARD

#include <Awesome/Core/Debug.hpp>

#include <Awesome/Maths/Config.hpp>
#include <Awesome/Maths/Algorithm.hpp>

namespace aw
{
	struct Vector3;
	struct Quaternion;
	struct Matrix4;

	struct Matrix3
	{
	#if (ROW_MAJOR == 1)
		float m00{ 0.0f };
		float m01{ 0.0f };
		float m02{ 0.0f };
		float m10{ 0.0f };
		float m11{ 0.0f };
		float m12{ 0.0f };
		float m20{ 0.0f };
		float m21{ 0.0f };
		float m22{ 0.0f };
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
	#endif

		Matrix3() = default;
		inline Matrix3(
			const float _m00, const float _m01, const float _m02,
			const float _m10, const float _m11, const float _m12,
			const float _m20, const float _m21, const float _m22);
		inline Matrix3(const Matrix3& other);
		AWESOME_MATHS_API explicit Matrix3(const Matrix4& other);
		inline Matrix3(Matrix3&& other);
		~Matrix3() = default;

		AWESOME_MATHS_API Matrix3 GetInversed() const;

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Returns GramSchmidt orthogonalization from column vectors
		*/
		AWESOME_MATHS_API Matrix3 GetOrthogonalized() const;
		inline Matrix3 GetTransposed() const;
		AWESOME_MATHS_API Vector3 GetRow(uint8 row) const;
		AWESOME_MATHS_API Vector3 GetColumn(uint8 column) const;

		AWESOME_MATHS_API Matrix3& Inverse();

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		GramSchmidt orthogonalization from column vectors
		*/
		AWESOME_MATHS_API Matrix3& Orthogonalize();
		inline Matrix3& Transpose();

	#if AWESOME_MATHS_SAFE
		inline Matrix3& Invalidate();
	#endif

		inline float Trace() const;
		inline float Determinant() const;

		/** 
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Remove any scaling from this matrix (ie magnitude of each column is 1)
		Return the scale vector that was initially present. 
		*/
		AWESOME_MATHS_API Vector3 ExtractScaling(float Tolerance = FLT_EPSILON);

		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Decompose the matrix into Q and R matrices where *this = Q * R, 
		Q is orthogonal and R is triangular right using householder reflections
		*/
		AWESOME_MATHS_API void QRDecompositionHR(Matrix3& Q, Matrix3& R) const;
		
		/**
		ᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔᚔ
		Decompose the matrix into Q and R matrices where *this = Q * R, 
		Q is orthogonal and R is triangular right using given rotation
		*/
		AWESOME_MATHS_API void QRDecompositionGR(Matrix3& Q, Matrix3& R) const;
		inline Matrix3& MultiplyRow(uint8 row, float factor);
		inline Matrix3& MultiplyRows(float factor1, float factor2, float factor3);
		inline Matrix3& DivideRow(uint8 row, float divisor);
		inline Matrix3& DivideRows(float divisor1, float divisor2, float divisor3);
		inline Matrix3& MultiplyColumn(uint8 column, float factor);
		inline Matrix3& MultiplyColumns(float factor1, float factor2, float factor3);
		inline Matrix3& DivideColumn(uint8 column, float divisor);
		inline Matrix3& DivideColumns(float divisor1, float divisor2, float divisor3);

		inline bool IsDiagonal() const;
		inline bool IsOrthogonal() const;

		AWESOME_MATHS_API Quaternion ToQuaternion() const;
		inline std::string ToString() const;

		AWESOME_MATHS_API static Matrix3 FromColumns(const Vector3& col1, const Vector3& col2, const Vector3& col3);
		AWESOME_MATHS_API static Matrix3 FromRows(const Vector3& row1, const Vector3& row2, const Vector3& row3);
		AWESOME_MATHS_API static Matrix3 MakeRotation(const Vector3& eulerAngles);

		inline static const Matrix3& Zero();
		inline static const Matrix3& Identity();

		inline Matrix3 operator+() const;
		inline Matrix3 operator-() const;
		inline Matrix3 operator*(const float rhs) const;
		inline Matrix3 operator/(const float rhs) const;
		AWESOME_MATHS_API Vector3 operator*(const Vector3& rhs) const;
		inline Matrix3 operator+(const Matrix3& rhs) const;
		inline Matrix3 operator-(const Matrix3& rhs) const;
		AWESOME_MATHS_API Matrix3 operator*(const Matrix3& rhs) const;

		AWESOME_MATHS_API Matrix3& operator=(const Matrix3& rhs);
		AWESOME_MATHS_API Matrix3& operator=(Matrix3&& rhs);
		AWESOME_MATHS_API Matrix3& operator*=(const float rhs);
		AWESOME_MATHS_API Matrix3& operator/=(const float rhs);
		AWESOME_MATHS_API Matrix3& operator+=(const Matrix3& rhs);
		AWESOME_MATHS_API Matrix3& operator-=(const Matrix3& rhs);
		AWESOME_MATHS_API Matrix3& operator*=(const Matrix3& rhs);

		inline bool operator==(const Matrix3& rhs) const;
		inline bool operator!=(const Matrix3& rhs) const;

		AWESOME_MATHS_API operator Matrix4() const;

		AWESOME_MATHS_API float& operator[](const uint8 index);
		AWESOME_MATHS_API float operator[](const uint8 index) const;

		inline float& operator()(const uint8 rowIndex, const uint8 columnIndex);
		inline float operator()(const uint8 rowIndex, const uint8 columnIndex) const;
	};

	inline Matrix3 operator*(const float lhs, const Matrix3& rhs);
}

#include <Awesome/Maths/Matrix3.inl>

#endif //GUARD