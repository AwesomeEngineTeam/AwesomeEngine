#include <Awesome/Maths/Matrix3x4.hpp>

#include <Awesome/Maths/Quaternion.hpp>
#include <Awesome/Maths/Matrix4.hpp>

namespace aw
{
	Matrix3x4 Matrix3x4::GetInversed() const
	{
		float det = Determinant();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(det))
		{
			LOG_ERROR("Matrix3x4::GetInversed - not inversible")
				return Zero();
		}
	#endif

		return (1.0f / det) * Matrix3x4 {
			m11 * m22 - m21 * m12,
			m21 * m02 - m01 * m22,
			m01 * m12 - m11 * m02,
			m01 * m13 * m22 - m01 * m12 * m23 + m11 * m02 * m23 - m11 * m03 * m22 - m21 * m02 * m13 + m21 * m03 * m12,

			m20 * m12 - m10 * m22,
			m00 * m22 - m20 * m02,
			m10 * m02 - m00 * m12,
			m00 * m12 * m23 - m00 * m13 * m22 - m10 * m02 * m23 + m10 * m03 * m22 + m20 * m02 * m13 - m20 * m03 * m12,

			m10 * m21 - m20 * m11,
			m20 * m01 - m00 * m21,
			m00 * m11 - m10 * m01,
			m00 * m13 * m21 - m00 * m11 * m23 + m10 * m01 * m23 - m10 * m03 * m21 - m20 * m01 * m13 + m20 * m03 * m11
		};
	}

	Vector3 Matrix3x4::GetRow(uint8 row) const
	{
		if (row == 0u)
			return Vector3(m00, m01, m02);
		if (row == 1u)
			return Vector3(m10, m11, m12);
		if (row == 2u)
			return Vector3(m20, m21, m22);

		LOG_ERROR("Matrix3x4::GetRow - row inaccessible : row " + std::to_string(row + 1))
			return Vector3::Zero();
	}

	Vector3 Matrix3x4::GetColumn(uint8 column) const
	{
		if (column == 0u)
			return Vector3(m00, m10, m20);
		if (column == 1u)
			return Vector3(m01, m11, m21);
		if (column == 2u)
			return Vector3(m02, m12, m22);
		if (column == 3u)
			return Vector3(m03, m13, m23);

		LOG_ERROR("Matrix3x4::GetColumn - column inaccessible : column " + std::to_string(column + 1))
			return Vector3::Zero();
	}

	Matrix3x4& Matrix3x4::Inverse()
	{
		float det = Determinant();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(det))
		{
			LOG_ERROR("Matrix3x4::GetInversed - not inversible")
				return Invalidate();
		}
	#endif

		float recM00 = m00;
		float recM01 = m01;
		float recM02 = m02;
		float recM03 = m03;
		float recM10 = m10;
		float recM11 = m11;
		float recM12 = m12;
		float recM13 = m13;
		float recM20 = m20;
		float recM21 = m21;

		m00 = recM11 * m22 - recM21 * recM12;
		m01 = recM21 * recM02 - recM01 * m22;
		m02 = recM01 * recM12 - recM11 * recM02;
		m03 = recM01 * recM13 * m22 - recM01 * recM12 * m23 + recM11 * recM02 * m23 - recM11 * recM03 * m22 - recM21 * recM02 * recM13 + recM21 * recM03 * recM12;
		m10 = recM20 * recM12 - recM10 * m22;
		m11 = recM00 * m22 - recM20 * recM02;
		m12 = recM10 * recM02 - recM00 * recM12;
		m13 = recM00 * recM12 * m23 - recM00 * recM13 * m22 - recM10 * recM02 * m23 + recM10 * recM03 * m22 + recM20 * recM02 * recM13 - recM20 * recM03 * recM12;
		m20 = recM10 * recM21 - recM20 * recM11;
		m21 = recM20 * recM01 - recM00 * recM21;
		m22 = recM00 * recM11 - recM10 * recM01;
		m23 = recM00 * recM13 * recM21 - recM00 * recM11 * m23 + recM10 * recM01 * m23 - recM10 * recM03 * recM21 - recM20 * recM01 * recM13 + recM20 * recM03 * recM11;

		return this->operator*=(1.0f / det);
	}

	Vector3 Matrix3x4::ExtractTranslation()
	{
		Vector3 translation{ m03, m13, m23 };

		m03 = m13 = m23 = 0.0f;

		return translation;
	}

	Quaternion Matrix3x4::ExtractRotation()
	{
		return Quaternion();
	}

	Vector3 Matrix3x4::ExtractScaling()
	{
		Vector3 scale;

		const float sumSquare0 = SumSquare(m00, m10, m20);
		const float sumSquare1 = SumSquare(m01, m11, m21);
		const float sumSquare2 = SumSquare(m02, m12, m22);

		if (sumSquare0 > FLT_EPSILON)
		{
			float scale0 = sqrtf(sumSquare0);
			scale.x = scale0;
			float invScale0 = 1.0f / scale0;
			m00 *= invScale0;
			m10 *= invScale0;
			m20 *= invScale0;
		}
		else
		{
			scale.x = 0.0f;
		}

		if (sumSquare1 > FLT_EPSILON)
		{
			float scale1 = sqrtf(sumSquare1);
			scale.y = scale1;
			float invScale1 = 1.0f / scale1;
			m01 *= invScale1;
			m11 *= invScale1;
			m21 *= invScale1;
		}
		else
		{
			scale.y = 0.0f;
		}

		if (sumSquare2 > FLT_EPSILON)
		{
			float scale2 = sqrtf(sumSquare2);
			scale.z = scale2;
			float invScale2 = 1.0f / scale2;
			m02 *= invScale2;
			m12 *= invScale2;
			m22 *= invScale2;
		}
		else
		{
			scale.z = 0.0f;
		}

		return scale;
	}

	//Quaternion Matrix3x4::ToQuaternion() const
	//{
	//	Quaternion q;

	//	float trace = m00 + m11 + m22;
	//	if (trace > 0.0f)
	//	{
	//		float invS = InvSqrt(trace + 1.0f);
	//		q.w = 0.5f / invS;

	//		float s = 0.5f * invS;
	//		q.x = (m21 - m12) * s;
	//		q.y = (m02 - m20) * s;
	//		q.z = (m10 - m01) * s; 
	//	}
	//	else 
	//	{
	//		if ((m00 > m11) && (m00 > m22)) 
	//		{
	//			float invS = InvSqrt(1.0f + m00 - m11 - m22);
	//			q.x = 0.5f / invS;

	//			float s = 0.5f * invS;
	//			q.w = (m21 - m12) * s;
	//			q.y = (m10 + m01) * s;
	//			q.z = (m02 + m20) * s;
	//		}
	//		else if (m11 > m22) 
	//		{
	//			float invS = InvSqrt(1.0f - m00 + m11 - m22);
	//			q.y = 0.5f / invS;

	//			float s = 0.5f * invS;
	//			q.w = (m02 - m20) * s;
	//			q.x = (m10 + m01) * s;
	//			q.z = (m21 + m12) * s;
	//		}
	//		else 
	//		{
	//			float invS = InvSqrt(1.0f - m00 - m11 + m22);
	//			q.z = 0.5f / invS;

	//			float s = 0.5f * invS;
	//			q.w = (m10 - m01) * s;
	//			q.x = (m02 + m20) * s;
	//			q.y = (m12 + m21) * s;
	//		}
	//	}

	//	return q;
	//}

	Matrix3x4 Matrix3x4::FromColumns(const Vector3& col1, const Vector3& col2, const Vector3& col3, const Vector3& col4)
	{
		return Matrix3x4{
			col1.x, col2.x, col3.x, col4.x,
			col1.y, col2.y, col3.y, col4.y,
			col1.z, col2.z, col3.z, col4.z
		};
	}

	Matrix3x4 Matrix3x4::FromRows(const Vector3& row1, const Vector3& row2, const Vector3& row3)
	{
		return Matrix3x4{
			row1.x, row1.y, row1.z, 0.0f,
			row2.x, row2.y, row2.z, 0.0f,
			row3.x, row3.y, row3.z, 0.0f
		};
	}

	//Matrix3x4 Matrix3x4::MakeRotation(const Vector3& eulerAngles)
	//{
	//	Matrix3x4 RX = Identity();
	//	RX.m11 = cosf(eulerAngles.x);
	//	RX.m12 = -sinf(eulerAngles.x);
	//	RX.m21 = sinf(eulerAngles.x);
	//	RX.m22 = cosf(eulerAngles.x);

	//	Matrix3x4 RY = Identity();
	//	RY.m00 = cosf(eulerAngles.y);
	//	RY.m02 = sinf(eulerAngles.y);
	//	RY.m20 = -sinf(eulerAngles.y);
	//	RY.m22 = cosf(eulerAngles.y);

	//	Matrix3x4 RZ = Identity();
	//	RZ.m00 = cosf(eulerAngles.z);
	//	RZ.m01 = -sinf(eulerAngles.z);
	//	RZ.m10 = sinf(eulerAngles.z);
	//	RZ.m11 = cosf(eulerAngles.z);

	//	return RY * RX * RZ;
	//}

	Vector3 Matrix3x4::operator*(const Vector3& rhs) const
	{
		return Vector3{
			m00 * rhs.x + m01 * rhs.y + m02 * rhs.z + m03,
			m10 * rhs.x + m11 * rhs.y + m12 * rhs.z + m13,
			m20 * rhs.x + m21 * rhs.y + m22 * rhs.z + m23
		};
	}

	Matrix3x4 Matrix3x4::operator*(const Matrix3x4& rhs) const
	{
		return Matrix3x4{
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
			m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23
		};
	}

	Matrix4 Matrix3x4::operator*(const Matrix4 & rhs) const
	{
		return Matrix4{
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

			rhs.m30,
			rhs.m31,
			rhs.m32,
			rhs.m33
		};
	}

	Matrix3x4& Matrix3x4::operator=(const Matrix3x4& rhs)
	{
		m00 = rhs.m00;
		m10 = rhs.m10;
		m20 = rhs.m20;
		m01 = rhs.m01;
		m11 = rhs.m11;
		m21 = rhs.m21;
		m02 = rhs.m02;
		m12 = rhs.m12;
		m22 = rhs.m22;
		m03 = rhs.m03;
		m13 = rhs.m13;
		m23 = rhs.m23;

		return *this;
	}

	Matrix3x4& Matrix3x4::operator=(Matrix3x4&& rhs)
	{
		m00 = std::move(rhs.m00);
		m10 = std::move(rhs.m10);
		m20 = std::move(rhs.m20);
		m01 = std::move(rhs.m01);
		m11 = std::move(rhs.m11);
		m21 = std::move(rhs.m21);
		m02 = std::move(rhs.m02);
		m12 = std::move(rhs.m12);
		m22 = std::move(rhs.m22);
		m03 = std::move(rhs.m03);
		m13 = std::move(rhs.m13);
		m23 = std::move(rhs.m23);

		return *this;
	}

	Matrix3x4& Matrix3x4::operator*=(const float rhs)
	{
		m00 *= rhs;
		m01 *= rhs;
		m02 *= rhs;
		m03 *= rhs;
		m10 *= rhs;
		m11 *= rhs;
		m12 *= rhs;
		m13 *= rhs;
		m20 *= rhs;
		m21 *= rhs;
		m22 *= rhs;
		m23 *= rhs;

		return *this;
	}

	Matrix3x4& Matrix3x4::operator/=(const float rhs)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyEqual(rhs, 0.0f))
		{
			LOG_ERROR("Matrix3x4::operator/ - divisor null")
				return Invalidate();
		}
	#endif

		m00 /= rhs;
		m01 /= rhs;
		m02 /= rhs;
		m03 /= rhs;
		m10 /= rhs;
		m11 /= rhs;
		m12 /= rhs;
		m13 /= rhs;
		m20 /= rhs;
		m21 /= rhs;
		m22 /= rhs;
		m23 /= rhs;

		return *this;
	}

	Matrix3x4& Matrix3x4::operator+=(const Matrix3x4& rhs)
	{
		m00 += rhs.m00;
		m01 += rhs.m01;
		m02 += rhs.m02;
		m03 += rhs.m03;
		m10 += rhs.m10;
		m11 += rhs.m11;
		m12 += rhs.m12;
		m13 += rhs.m13;
		m20 += rhs.m20;
		m21 += rhs.m21;
		m22 += rhs.m22;
		m23 += rhs.m23;

		return *this;
	}

	Matrix3x4& Matrix3x4::operator-=(const Matrix3x4& rhs)
	{
		m00 -= rhs.m00;
		m01 -= rhs.m01;
		m02 -= rhs.m02;
		m03 -= rhs.m03;

		m10 -= rhs.m10;
		m11 -= rhs.m11;
		m12 -= rhs.m12;
		m13 -= rhs.m13;

		m20 -= rhs.m20;
		m21 -= rhs.m21;
		m22 -= rhs.m22;
		m23 -= rhs.m23;

		return *this;
	}

	Matrix3x4& Matrix3x4::operator*=(const Matrix3x4& rhs)
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

		m00 = recM00 * rhs.m00 + recM01 * rhs.m10 + recM02 * rhs.m20;
		m01 = recM00 * rhs.m01 + recM01 * rhs.m11 + recM02 * rhs.m21;
		m02 = recM00 * rhs.m02 + recM01 * rhs.m12 + recM02 * rhs.m22;
		m03 = recM00 * rhs.m03 + recM01 * rhs.m13 + recM02 * rhs.m23 + m03;

		m10 = recM10 * rhs.m00 + recM11 * rhs.m10 + recM12 * rhs.m20;
		m11 = recM10 * rhs.m01 + recM11 * rhs.m11 + recM12 * rhs.m21;
		m12 = recM10 * rhs.m02 + recM11 * rhs.m12 + recM12 * rhs.m22;
		m13 = recM10 * rhs.m03 + recM11 * rhs.m13 + recM12 * rhs.m23 + m13;

		m20 = recM20 * rhs.m00 + recM21 * rhs.m10 + recM22 * rhs.m20;
		m21 = recM20 * rhs.m01 + recM21 * rhs.m11 + recM22 * rhs.m21;
		m22 = recM20 * rhs.m02 + recM21 * rhs.m12 + recM22 * rhs.m22;
		m23 = recM20 * rhs.m03 + recM21 * rhs.m13 + recM22 * rhs.m23 + m23;

		return *this;
	}

	Matrix3x4::operator Matrix4() const
	{
		return Matrix4{
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	float& Matrix3x4::operator[](const uint8 index)
	{
	#if (ROW_MAJOR == 1)
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
	#else
		if (index == 0u)
			return m00;
		if (index == 1u)
			return m10;
		if (index == 2u)
			return m20;
		if (index == 3u)
			return m01;
		if (index == 4u)
			return m11;
		if (index == 5u)
			return m21;
		if (index == 6u)
			return m02;
		if (index == 7u)
			return m12;
		if (index == 8u)
			return m22;
		if (index == 9u)
			return m03;
		if (index == 10)
			return m13;
		if (index == 11)
			return m23;
	#endif

		LOG_ERROR("float& Matrix3x4[" + std::to_string(index) + "] Element out of Array")
			return m00;
	}

	float Matrix3x4::operator[](const uint8 index) const
	{
	#if (ROW_MAJOR == 1)
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
	#else
		if (index == 0u)
			return m00;
		if (index == 1u)
			return m10;
		if (index == 2u)
			return m20;
		if (index == 3u)
			return m01;
		if (index == 4u)
			return m11;
		if (index == 5u)
			return m21;
		if (index == 6u)
			return m02;
		if (index == 7u)
			return m12;
		if (index == 8u)
			return m22;
		if (index == 9u)
			return m03;
		if (index == 10)
			return m13;
		if (index == 11)
			return m23;
	#endif

		LOG_ERROR("float Matrix3x4[" + std::to_string(index) + "] Element out of Array")
			return 0.0f;
	}
}