#include <Awesome/Maths/Matrix4.hpp>

#include <Awesome/Maths/Matrix3.hpp>
#include <Awesome/Maths/Matrix3x4.hpp>
#include <Awesome/Maths/Quaternion.hpp>

namespace aw
{
	Matrix4::Matrix4(const Matrix3& other) :
		m00{ other.m00 },
		m10{ other.m10 },
		m20{ other.m20 },
		m30{ 0.0f },
		m01{ other.m01 },
		m11{ other.m11 },
		m21{ other.m21 },
		m31{ 0.0f },
		m02{ other.m02 },
		m12{ other.m12 },
		m22{ other.m22 },
		m32{ 0.0f },
		m03{ 0.0f },
		m13{ 0.0f },
		m23{ 0.0f },
		m33{ 1.0f }
	{
	}

	Matrix4 Matrix4::GetInversed() const
	{
		if (IsOrthogonal())
			return GetTransposed();

		float det = Determinant();

		if (NearlyZero(det))
			return Zero();

		if (IsHomogenous())
		{
			return {
				(m11 * m22 - m21 * m12) / det, (m02 * m21 - m01 * m22) / det, (m01 * m12 - m11 * m02) / det, 0.0f,
				(m12 * m20 - m10 * m22) / det, (m00 * m22 - m20 * m02) / det, (m02 * m10 - m00 * m12) / det, 0.0f,
				(m10 * m21 - m20 * m11) / det, (m01 * m20 - m00 * m21) / det, (m00 * m11 - m10 * m01) / det, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		Matrix4 inv;
		inv.m00 =
			m11 * m22 * m33 -
			m11 * m23 * m32 -
			m21 * m12 * m33 +
			m21 * m13 * m32 +
			m31 * m12 * m23 -
			m31 * m13 * m22;

		inv.m10 =
			-m10 * m22 * m33 +
			m10 * m23 * m32 +
			m20 * m12 * m33 -
			m20 * m13 * m32 -
			m30 * m12 * m23 +
			m30 * m13 * m22;

		inv.m20 =
			m10 * m21 * m33 -
			m10 * m23 * m31 -
			m20 * m11 * m33 +
			m20 * m13 * m31 +
			m30 * m11 * m23 -
			m30 * m13 * m21;

		inv.m30 =
			-m10 * m21 * m32 +
			m10 * m22 * m31 +
			m20 * m11 * m32 -
			m20 * m12 * m31 -
			m30 * m11 * m22 +
			m30 * m12 * m21;

		inv.m01 =
			-m01 * m22 * m33 +
			m01 * m23 * m32 +
			m21 * m02 * m33 -
			m21 * m03 * m32 -
			m31 * m02 * m23 +
			m31 * m03 * m22;

		inv.m11 =
			m00 * m22 * m33 -
			m00 * m23 * m32 -
			m20 * m02 * m33 +
			m20 * m03 * m32 +
			m30 * m02 * m23 -
			m30 * m03 * m22;

		inv.m21 =
			-m00 * m21 * m33 +
			m00 * m23 * m31 +
			m20 * m01 * m33 -
			m20 * m03 * m31 -
			m30 * m01 * m23 +
			m30 * m03 * m21;

		inv.m31 =
			m00 * m21 * m32 -
			m00 * m22 * m31 -
			m20 * m01 * m32 +
			m20 * m02 * m31 +
			m30 * m01 * m22 -
			m30 * m02 * m21;

		inv.m02 =
			m01 * m12 * m33 -
			m01 * m13 * m32 -
			m11 * m02 * m33 +
			m11 * m03 * m32 +
			m31 * m02 * m13 -
			m31 * m03 * m12;

		inv.m12 =
			-m00 * m12 * m33 +
			m00 * m13 * m32 +
			m10 * m02 * m33 -
			m10 * m03 * m32 -
			m30 * m02 * m13 +
			m30 * m03 * m12;

		inv.m22 =
			m00 * m11 * m33 -
			m00 * m13 * m31 -
			m10 * m01 * m33 +
			m10 * m03 * m31 +
			m30 * m01 * m13 -
			m30 * m03 * m11;

		inv.m32 =
			-m00 * m11 * m32 +
			m00 * m12 * m31 +
			m10 * m01 * m32 -
			m10 * m02 * m31 -
			m30 * m01 * m12 +
			m30 * m02 * m11;

		inv.m03 =
			-m01 * m12 * m23 +
			m01 * m13 * m22 +
			m11 * m02 * m23 -
			m11 * m03 * m22 -
			m21 * m02 * m13 +
			m21 * m03 * m12;

		inv.m13 =
			m00 * m12 * m23 -
			m00 * m13 * m22 -
			m10 * m02 * m23 +
			m10 * m03 * m22 +
			m20 * m02 * m13 -
			m20 * m03 * m12;

		inv.m23 =
			-m00 * m11 * m23 +
			m00 * m13 * m21 +
			m10 * m01 * m23 -
			m10 * m03 * m21 -
			m20 * m01 * m13 +
			m20 * m03 * m11;

		inv.m33 =
			m00 * m11 * m22 -
			m00 * m12 * m21 -
			m10 * m01 * m22 +
			m10 * m02 * m21 +
			m20 * m01 * m12 -
			m20 * m02 * m11;

		inv /= det;

		return inv;
	}

	Vector3 Matrix4::GetRow(uint8 row)
	{
		if (row == 0u)
			return Vector3(m00, m01, m02);
		if (row == 1u)
			return Vector3(m10, m11, m12);
		if (row == 2u)
			return Vector3(m20, m21, m22);
		if (row == 3u)
			return Vector3(m30, m31, m32);

		LOG_ERROR("Matrix4::GetRow - row inaccessible : row " + std::to_string(row + 1))
			return Vector3::Zero();
	}

	Vector3 Matrix4::GetColumn(uint8 column)
	{
		if (column == 0u)
			return Vector3(m00, m10, m20);
		if (column == 1u)
			return Vector3(m01, m11, m21);
		if (column == 2u)
			return Vector3(m02, m12, m22);
		if (column == 3u)
			return Vector3(m03, m13, m23);

		LOG_ERROR("Matrix4::GetColumn - column inaccessible : column " + std::to_string(column + 1));
		return Vector3::Zero();
	}

	Matrix4& Matrix4::Inverse()
	{
		if (IsOrthogonal())
			return Transpose();

		float det = Determinant();

	#if AWESOME_MATHS_SAFE 
		if (NearlyZero(det))
			return Invalidate();
	#endif

		float invDet = (1.0f / det);

		float recM00 = m00;
		float recM10 = m10;
		float recM20 = m20;

		float recM01 = m01;
		float recM11 = m11;

		float recM02 = m02;

		if (IsHomogenous())
		{
			m00 = invDet * (recM11	* m22		-	m21		* m12);
			m10 = invDet * (m12		* recM20	-	recM10	* m22);
			m20 = invDet * (recM10	* m21		-	recM20	* recM11);
			m01 = invDet * (recM02	* m21		-	recM01	* m22);
			m11 = invDet * (recM00	* m22		-	recM20	* recM02);
			m21 = invDet * (recM01	* recM20	-	recM00	* m21);
			m02 = invDet * (recM01	* m12		-	recM11	* recM02);
			m12 = invDet * (recM02	* recM10	-	recM00	* m12);
			m22 = invDet * (recM00	* recM11	-	recM10	* recM01);

			return *this;
		}

		float recM30 = m30;

		float recM21 = m21;
		float recM31 = m31;

		float recM12 = m12;
		float recM22 = m22;

		float recM03 = m03;
		float recM13 = m13;

		m00 = invDet * (
			recM11 * recM22 * m33 -
			recM11 * m23 * m32 -
			recM21 * recM12 * m33 +
			recM21 * recM13 * m32 +
			recM31 * recM12 * m23 -
			recM31 * recM13 * recM22);

		m10 = invDet * (
			-recM10 * recM22 * m33 +
			recM10 * m23 * m32 +
			recM20 * recM12 * m33 -
			recM20 * recM13 * m32 -
			recM30 * recM12 * m23 +
			recM30 * recM13 * recM22
		);

		m20 = invDet * (
			recM10 * recM21 * m33 -
			recM10 * m23 * recM31 -
			recM20 * recM11 * m33 +
			recM20 * recM13 * recM31 +
			recM30 * recM11 * m23 -
			recM30 * recM13 * recM21
		);

		m30 = invDet * (
			-recM10 * recM21 * m32 +
			recM10 * recM22 * recM31 +
			recM20 * recM11 * m32 -
			recM20 * recM12 * recM31 -
			recM30 * recM11 * recM22 +
			recM30 * recM12 * recM21
		);

		m01 = invDet * (
			-recM01 * recM22 * m33 +
			recM01 * m23 * m32 +
			recM21 * recM02 * m33 -
			recM21 * recM03 * m32 -
			recM31 * recM02 * m23 +
			recM31 * recM03 * recM22
		);

		m11 = invDet * (
			recM00 * recM22 * m33 -
			recM00 * m23 * m32 -
			recM20 * recM02 * m33 +
			recM20 * recM03 * m32 +
			recM30 * recM02 * m23 -
			recM30 * recM03 * recM22
		);

		m21 = invDet * (
			-recM00 * recM21 * m33 +
			recM00 * m23 * recM31 +
			recM20 * recM01 * m33 -
			recM20 * recM03 * recM31 -
			recM30 * recM01 * m23 +
			recM30 * recM03 * recM21
		);

		m31 = invDet * (
			recM00 * recM21 * m32 -
			recM00 * recM22 * recM31 -
			recM20 * recM01 * m32 +
			recM20 * recM02 * recM31 +
			recM30 * recM01 * recM22 -
			recM30 * recM02 * recM21
		);

		m02 = invDet * (
			recM01 * recM12 * m33 -
			recM01 * recM13 * m32 -
			recM11 * recM02 * m33 +
			recM11 * recM03 * m32 +
			recM31 * recM02 * recM13 -
			recM31 * recM03 * recM12
		);

		m12 = invDet * (
			-recM00 * recM12 * m33 +
			recM00 * recM13 * m32 +
			recM10 * recM02 * m33 -
			recM10 * recM03 * m32 -
			recM30 * recM02 * recM13 +
			recM30 * recM03 * recM12
		);

		m22 = invDet * (
			recM00 * recM11 * m33 -
			recM00 * recM13 * recM31 -
			recM10 * recM01 * m33 +
			recM10 * recM03 * recM31 +
			recM30 * recM01 * recM13 -
			recM30 * recM03 * recM11
		);

		m32 = invDet * (
			-recM00 * recM11 * m32 +
			recM00 * recM12 * recM31 +
			recM10 * recM01 * m32 -
			recM10 * recM02 * recM31 -
			recM30 * recM01 * recM12 +
			recM30 * recM02 * recM11
		);

		m03 = invDet * (
			-recM01 * recM12 * m23 +
			recM01 * recM13 * recM22 +
			recM11 * recM02 * m23 -
			recM11 * recM03 * recM22 -
			recM21 * recM02 * recM13 +
			recM21 * recM03 * recM12
		);

		m13 = invDet * (
			recM00 * recM12 * m23 -
			recM00 * recM13 * recM22 -
			recM10 * recM02 * m23 +
			recM10 * recM03 * recM22 +
			recM20 * recM02 * recM13 -
			recM20 * recM03 * recM12
		);

		m23 = invDet * (
			-recM00 * recM11 * m23 +
			recM00 * recM13 * recM21 +
			recM10 * recM01 * m23 -
			recM10 * recM03 * recM21 -
			recM20 * recM01 * recM13 +
			recM20 * recM03 * recM11
		);

		m33 = invDet * (
			recM00 * recM11 * recM22 -
			recM00 * recM12 * recM21 -
			recM10 * recM01 * recM22 +
			recM10 * recM02 * recM21 +
			recM20 * recM01 * recM12 -
			recM20 * recM02 * recM11
		);

		return *this;
	}

	float Matrix4::Determinant() const
	{
		if (IsHomogenous())
		{
			return
				m00 * m11 * m22 +
				m01 * m12 * m20 +
				m02 * m10 * m21 -
				m20 * m11 * m02 -
				m21 * m12 * m00 -
				m22 * m10 * m01;
		}

		float det1 = m11 * (m22 * m33 - m32 * m23) - m21 * (m12 * m33 - m32 * m13) + m31 * (m12 * m23 - m22 * m13);
		float det2 = m01 * (m22 * m33 - m32 * m23) - m21 * (m02 * m33 - m32 * m03) + m31 * (m02 * m23 - m22 * m03);
		float det3 = m01 * (m12 * m33 - m32 * m13) - m11 * (m02 * m33 - m32 * m03) + m31 * (m02 * m13 - m12 * m03);
		float det4 = m01 * (m12 * m23 - m22 * m13) - m11 * (m02 * m23 - m22 * m03) + m21 * (m02 * m13 - m12 * m03);

		return m00 * det1 - m10 * det2 + m20 * det3 - m30 * det4;
	}

	Vector3 Matrix4::Project(const Vector3& vec) const
	{
		return {
			m00 * vec.x + m01 * vec.y + m02 * vec.z + m03,
			m10 * vec.x + m11 * vec.y + m12 * vec.z + m13,
			m20 * vec.x + m21 * vec.y + m22 * vec.z + m23
		};
	}

	Vector3 Matrix4::ProjectAndDivideByW(const Vector3& vec) const
	{
		float w = m30 * vec.x + m31 * vec.y + m32 * vec.z + m33;

		return {
			(m00 * vec.x + m01 * vec.y + m02 * vec.z + m03) / w,
			(m10 * vec.x + m11 * vec.y + m12 * vec.z + m13) / w,
			(m20 * vec.x + m21 * vec.y + m22 * vec.z + m23) / w
		};
	}

	bool Matrix4::IsOrthogonal() const
	{
		return
			NearlyEqual(m00 * m01 + m10 * m11 + m20 * m21 + m30 * m31, 0.0f) &&
			NearlyEqual(m00 * m02 + m10 * m12 + m20 * m22 + m30 * m32, 0.0f) &&
			NearlyEqual(m00 * m03 + m10 * m13 + m20 * m23 + m30 * m33, 0.0f) &&
			NearlyEqual(m01 * m02 + m11 * m12 + m21 * m22 + m31 * m32, 0.0f) &&
			NearlyEqual(m01 * m03 + m11 * m13 + m21 * m23 + m31 * m33, 0.0f) &&
			NearlyEqual(m02 * m03 + m12 * m13 + m22 * m23 + m32 * m33, 0.0f) &&
			NearlyEqual(m00 * m00 + m10 * m10 + m20 * m20 + m30 * m30, 1.0f) &&
			NearlyEqual(m01 * m01 + m11 * m11 + m21 * m21 + m31 * m31, 1.0f) &&
			NearlyEqual(m02 * m02 + m12 * m12 + m22 * m22 + m32 * m32, 1.0f) &&
			NearlyEqual(m03 * m03 + m13 * m13 + m23 * m23 + m33 * m33, 1.0f);
	}

	Matrix4 Matrix4::FromColumns(const Vector3& col1, const Vector3& col2, const Vector3& col3, const Vector3& col4)
	{
		return {
			col1.x, col2.x, col3.x, col4.x,
			col1.y, col2.y, col3.y, col4.x,
			col1.z, col2.z, col3.z, col4.x,
			0.0f, 0.0f, 0.0f, 1.0f 
		};
	}

	Matrix4 Matrix4::FromRows(const Vector3& row1, const Vector3& row2, const Vector3& row3, const Vector3& row4)
	{
		return {
			row1.x, row1.y, row1.z, 0.0f,
			row2.x, row2.y, row2.z, 0.0f,
			row3.x, row3.y, row3.z, 0.0f,
			row4.x, row4.y, row4.z, 1.0f
		};
	}

	Matrix4 Matrix4::MakeTranslation(const Vector3& pos)
	{
		return {
			1.0f, 0.0f, 0.0f, pos.x,
			0.0f, 1.0f, 0.0f, pos.y,
			0.0f, 0.0f, 1.0f, pos.z,
			0.0f, 0.0f, 0.0f, 1.0f 
		};
	}
	
	Matrix4 Matrix4::MakeRotation(const Quaternion& rot)
	{
		Matrix4 result = Identity();

		result.m00 = rot(0,0);
		result.m10 = rot(1,0);
		result.m20 = rot(2,0);
		result.m01 = rot(0,1);
		result.m11 = rot(1,1);
		result.m21 = rot(2,1);
		result.m02 = rot(0,2);
		result.m12 = rot(1,2);
		result.m22 = rot(2,2);

		return result;
	}

	// TODO : Simplify
	Matrix4 Matrix4::MakeRotation(const Vector3& eulerAngles)
	{
		Matrix4 RX = Identity();
		RX.m11 = cosf(eulerAngles.x);
		RX.m12 = -sinf(eulerAngles.x);
		RX.m21 = sinf(eulerAngles.x);
		RX.m22 = cosf(eulerAngles.x);

		Matrix4 RY = Identity();
		RY.m00 = cosf(eulerAngles.y);
		RY.m02 = sinf(eulerAngles.y);
		RY.m20 = -sinf(eulerAngles.y);
		RY.m22 = cosf(eulerAngles.y);

		Matrix4 RZ = Identity();
		RZ.m00 = cosf(eulerAngles.z);
		RZ.m01 = -sinf(eulerAngles.z);
		RZ.m10 = sinf(eulerAngles.z);
		RZ.m11 = cosf(eulerAngles.z);

		return RY * RX * RZ;
	}
	
	Matrix4 Matrix4::MakeScale(const Vector3& scale)
	{
		return {
			scale.x, 0.0f, 0.0f, 0.0f,
			0.0f, scale.y, 0.0f, 0.0f,
			0.0f, 0.0f, scale.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f 
		};
	}

	Matrix4 Matrix4::MakePerspective(float angle, float ratio, float zNear, float zFar)
	{
		const float invTanHalfFovy = 1.0f / tanf(angle * 0.5f);

		const float k = zFar / (2.0f * (zNear - zFar));

		return {
			invTanHalfFovy / ratio, 0,					0,			0,
			0,						-invTanHalfFovy,	0,			0,
			0,						0,					k,			k * (1.0f + 2.0f * zNear),
			0,						0,					-1.0f,		0
		};
	}

	Vector3 Matrix4::operator*(const Vector3& rhs) const
	{
		return {
			m00 * rhs.x + m01 * rhs.y + m02 * rhs.z,
			m10 * rhs.x + m11 * rhs.y + m12 * rhs.z,
			m20 * rhs.x + m21 * rhs.y + m22 * rhs.z
		};
	}

	Matrix4 Matrix4::operator*(const Matrix4& rhs) const
	{
		return {
			m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30,
			m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31,
			m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32,
			m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33,

			m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30,
			m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31,
			m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32,
			m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33,

			m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30,
			m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31,
			m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32,
			m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33,

			m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30,
			m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31,
			m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32,
			m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33
		};
	}

	Matrix4 Matrix4::operator*(const Matrix3x4& rhs) const
	{
		return {
			m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
			m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
			m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,
			m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03,

			m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
			m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
			m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,
			m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13,

			m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
			m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
			m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22,
			m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23,

			m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20,
			m30 * rhs.m01 + m31 * rhs.m11 + m32 * rhs.m21,
			m30 * rhs.m02 + m31 * rhs.m12 + m32 * rhs.m22,
			m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33
		};
	}

	Matrix4& Matrix4::operator=(const Matrix4& rhs)
	{
		m00 = rhs.m00;
		m10 = rhs.m10;
		m20 = rhs.m20;
		m30 = rhs.m30;

		m01 = rhs.m01;
		m11 = rhs.m11;
		m21 = rhs.m21;
		m31 = rhs.m31;

		m02 = rhs.m02;
		m12 = rhs.m12;
		m22 = rhs.m22;
		m32 = rhs.m32;

		m03 = rhs.m03;
		m13 = rhs.m13;
		m23 = rhs.m23;
		m33 = rhs.m33;

		return *this;
	}

	Matrix4& Matrix4::operator=(Matrix4&& rhs)
	{
		m00 = std::move(rhs.m00);
		m10 = std::move(rhs.m10);
		m20 = std::move(rhs.m20);
		m30 = std::move(rhs.m30);

		m01 = std::move(rhs.m01);
		m11 = std::move(rhs.m11);
		m21 = std::move(rhs.m21);
		m31 = std::move(rhs.m31);

		m02 = std::move(rhs.m02);
		m12 = std::move(rhs.m12);
		m22 = std::move(rhs.m22);
		m32 = std::move(rhs.m32);

		m03 = std::move(rhs.m03);
		m13 = std::move(rhs.m13);
		m23 = std::move(rhs.m23);
		m33 = std::move(rhs.m33);

		return *this;
	}
	
	Matrix4& Matrix4::operator*=(const float rhs)
	{
		m00 *= rhs;
		m10 *= rhs;
		m20 *= rhs;
		m30 *= rhs;

		m01 *= rhs;
		m11 *= rhs;
		m21 *= rhs;
		m31 *= rhs;

		m02 *= rhs;
		m12 *= rhs;
		m22 *= rhs;
		m32 *= rhs;

		m03 *= rhs;
		m13 *= rhs;
		m23 *= rhs;
		m33 *= rhs;

		return *this;
	}
	
	Matrix4& Matrix4::operator/=(const float rhs)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
		{
			LOG_ERROR("Matrix4::operator/ - divisor null")
				return Invalidate();
		}
	#endif

		m00 /= rhs;
		m10 /= rhs;
		m20 /= rhs;
		m30 /= rhs;

		m01 /= rhs;
		m11 /= rhs;
		m21 /= rhs;
		m31 /= rhs;

		m02 /= rhs;
		m12 /= rhs;
		m22 /= rhs;
		m32 /= rhs;

		m03 /= rhs;
		m13 /= rhs;
		m23 /= rhs;
		m33 /= rhs;

		return *this;
	}
	
	Matrix4& Matrix4::operator+=(const Matrix4& rhs)
	{
		m00 += rhs.m00;
		m10 += rhs.m10;
		m20 += rhs.m20;
		m30 += rhs.m30;

		m01 += rhs.m01;
		m11 += rhs.m11;
		m21 += rhs.m21;
		m31 += rhs.m31;

		m02 += rhs.m02;
		m12 += rhs.m12;
		m22 += rhs.m22;
		m32 += rhs.m32;

		m03 += rhs.m03;
		m13 += rhs.m13;
		m23 += rhs.m23;
		m33 += rhs.m33;

		return *this;
	}
	
	Matrix4& Matrix4::operator-=(const Matrix4& rhs)
	{
		m00 -= rhs.m00;
		m10 -= rhs.m10;
		m20 -= rhs.m20;
		m30 -= rhs.m30;

		m01 -= rhs.m01;
		m11 -= rhs.m11;
		m21 -= rhs.m21;
		m31 -= rhs.m31;

		m02 -= rhs.m02;
		m12 -= rhs.m12;
		m22 -= rhs.m22;
		m32 -= rhs.m32;

		m03 -= rhs.m03;
		m13 -= rhs.m13;
		m23 -= rhs.m23;
		m33 -= rhs.m33;

		return *this;
	}

	Matrix4& Matrix4::operator*=(const Matrix4& rhs)
	{
		float recM00 = m00;
		float recM01 = m01;
		float recM02 = m02;
		float recM10 = m10;
		float recM11 = m11;
		float recM12 = m12;
		float recM20 = m20;
		float recM21 = m21;
		float recM22 = m22;
		float recM30 = m30;
		float recM31 = m31;
		float recM32 = m32;

		m00 = recM00 * rhs.m00 + recM01 * rhs.m10 + recM02 * rhs.m20 + m03 * rhs.m30;
		m01 = recM00 * rhs.m01 + recM01 * rhs.m11 + recM02 * rhs.m21 + m03 * rhs.m31;
		m02 = recM00 * rhs.m02 + recM01 * rhs.m12 + recM02 * rhs.m22 + m03 * rhs.m32;
		m03 = recM00 * rhs.m03 + recM01 * rhs.m13 + recM02 * rhs.m23 + m03 * rhs.m33;

		m10 = recM10 * rhs.m00 + recM11 * rhs.m10 + recM12 * rhs.m20 + m13 * rhs.m30;
		m11 = recM10 * rhs.m01 + recM11 * rhs.m11 + recM12 * rhs.m21 + m13 * rhs.m31;
		m12 = recM10 * rhs.m02 + recM11 * rhs.m12 + recM12 * rhs.m22 + m13 * rhs.m32;
		m13 = recM10 * rhs.m03 + recM11 * rhs.m13 + recM12 * rhs.m23 + m13 * rhs.m33;

		m20 = recM20 * rhs.m00 + recM21 * rhs.m10 + recM22 * rhs.m20 + m23 * rhs.m30;
		m21 = recM20 * rhs.m01 + recM21 * rhs.m11 + recM22 * rhs.m21 + m23 * rhs.m31;
		m22 = recM20 * rhs.m02 + recM21 * rhs.m12 + recM22 * rhs.m22 + m23 * rhs.m32;
		m23 = recM20 * rhs.m03 + recM21 * rhs.m13 + recM22 * rhs.m23 + m23 * rhs.m33;

		m30 = recM30 * rhs.m00 + recM31 * rhs.m10 + recM32 * rhs.m20 + m33 * rhs.m30;
		m31 = recM30 * rhs.m01 + recM31 * rhs.m11 + recM32 * rhs.m21 + m33 * rhs.m31;
		m32 = recM30 * rhs.m02 + recM31 * rhs.m12 + recM32 * rhs.m22 + m33 * rhs.m32;
		m33 = recM30 * rhs.m03 + recM31 * rhs.m13 + recM32 * rhs.m23 + m33 * rhs.m33;

		return *this;
	}

	float& Matrix4::operator[](const uint8 index)
	{
	#if ROW_MAJOR
		if (index == 0u)
			return m00;
		if (index == 1u)
			return m01;
		if (index == 2u)
			return m02;
		if (index == 3u)
			return m03;
		if (index == 4u)
			return m10;
		if (index == 5u)
			return m11;
		if (index == 6u)
			return m12;
		if (index == 7u)
			return m13;
		if (index == 8u)
			return m20;
		if (index == 9u)
			return m21;
		if (index == 10)
			return m22;
		if (index == 11)
			return m23;
		if (index == 12)
			return m30;
		if (index == 13)
			return m31;
		if (index == 14)
			return m32;
		if (index == 15)
			return m33;
	#else
		if (index == 0u)
			return m00;
		if (index == 1u)
			return m10;
		if (index == 2u)
			return m20;
		if (index == 3u)
			return m30;
		if (index == 4u)
			return m01;
		if (index == 5u)
			return m11;
		if (index == 6u)
			return m21;
		if (index == 7u)
			return m31;
		if (index == 8u)
			return m02;
		if (index == 9u)
			return m12;
		if (index == 10)
			return m22;
		if (index == 11)
			return m32;
		if (index == 12)
			return m03;
		if (index == 13)
			return m13;
		if (index == 14)
			return m23;
		if (index == 15)
			return m33;
	#endif
			
		LOG_ERROR("float& Mat3[" + std::to_string(index) + "] Element out of Array")
			return m00;
	}
	
	float Matrix4::operator[](const uint8 index) const
	{
	#if ROW_MAJOR
		if (index == 0u)
			return m00;
		if (index == 1u)
			return m01;
		if (index == 2u)
			return m02;
		if (index == 3u)
			return m03;
		if (index == 4u)
			return m10;
		if (index == 5u)
			return m11;
		if (index == 6u)
			return m12;
		if (index == 7u)
			return m13;
		if (index == 8u)
			return m20;
		if (index == 9u)
			return m21;
		if (index == 10)
			return m22;
		if (index == 11)
			return m23;
		if (index == 12)
			return m30;
		if (index == 13)
			return m31;
		if (index == 14)
			return m32;
		if (index == 15)
			return m33;
	#else
		if (index == 0u)
			return m00;
		if (index == 1u)
			return m10;
		if (index == 2u)
			return m20;
		if (index == 3u)
			return m30;
		if (index == 4u)
			return m01;
		if (index == 5u)
			return m11;
		if (index == 6u)
			return m21;
		if (index == 7u)
			return m31;
		if (index == 8u)
			return m02;
		if (index == 9u)
			return m12;
		if (index == 10)
			return m22;
		if (index == 11)
			return m32;
		if (index == 12)
			return m03;
		if (index == 13)
			return m13;
		if (index == 14)
			return m23;
		if (index == 15)
			return m33;
	#endif

		LOG_ERROR("float Mat3[" + std::to_string(index) + "] Element out of Array")
			return 0.0f;
	}
}