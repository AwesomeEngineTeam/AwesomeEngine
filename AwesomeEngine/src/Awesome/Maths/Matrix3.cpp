#include <Awesome/Maths/Matrix4.hpp>
#include <Awesome/Maths/Matrix3.hpp>
#include <Awesome/Maths/Vector2.hpp>
#include <Awesome/Maths/Vector3.hpp>
#include <Awesome/Maths/Quaternion.hpp>

namespace aw
{
	Matrix3::Matrix3(const Matrix4& other) :
		m00{ other.m00 },
		m10{ other.m10 },
		m20{ other.m20 },
		m01{ other.m01 },
		m11{ other.m11 },
		m21{ other.m21 },
		m02{ other.m02 },
		m12{ other.m12 },
		m22{ other.m22 }
	{
	}

	Matrix3 Matrix3::GetInversed() const
	{
		if (IsOrthogonal())
			return GetTransposed();

		float det = Determinant();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(det))
		{
			LOG_ERROR("Matrix3::GetInversed - not inversible")
				return Zero();
		}
	#endif

		return (1.0f / det) * Matrix3{
			m11 * m22 - m21 * m12, m02 * m21 - m01 * m22, m01 * m12 - m11 * m02,
			m12 * m20 - m10 * m22, m00 * m22 - m20 * m02, m02 * m10 - m00 * m12,
			m10 * m21 - m20 * m11, m01 * m20 - m00 * m21, m00 * m11 - m10 * m01
		};
	}

	Matrix3 Matrix3::GetOrthogonalized() const
	{
		Vector3 e0 = Vector3(m00, m10, m20);
		Vector3 e1 = Vector3(m01, m11, m21);
		Vector3 e2 = Vector3(m02, m12, m22);

		Vector3::GramSchmidt(e0, e1, e2);

		return FromColumns(e0, e1, e2);
	}

	Vector3 Matrix3::GetRow(uint8 row) const
	{
		if (row == 0u)
			return Vector3(m00, m01, m02);
		if (row == 1u)
			return Vector3(m10, m11, m12);
		if (row == 2u)
			return Vector3(m20, m21, m22);

		LOG_ERROR("Matrix3::GetRow - row inaccessible : row " + std::to_string(row + 1))
			return Vector3::Zero();
	}

	Vector3 Matrix3::GetColumn(uint8 column) const
	{
		if (column == 0u)
			return Vector3(m00, m10, m20);
		if (column == 1u)
			return Vector3(m01, m11, m21);
		if (column == 2u)
			return Vector3(m02, m12, m22);

		LOG_ERROR("Matrix3::GetColumn - column inaccessible : column " + std::to_string(column + 1))
			return Vector3::Zero();
	}

	Matrix3& Matrix3::Inverse()
	{
		if (IsOrthogonal())
			return Transpose();

		float det = Determinant();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(det))
		{
			LOG_ERROR("Matrix3::GetInversed - not inversible")
				return Invalidate();
		}
	#endif

		float recM00 = m00;
		float recM10 = m10;
		float recM20 = m20;
		float recM01 = m01;
		float recM11 = m11;
		float recM02 = m02;

		m00 =  recM11 * m22		-	m21		* m12;
		m10 =  m12	* recM20	-	recM10	* m22;
		m20 =  recM10 * m21		-	recM20	* recM11;
		m01 =  recM02 * m21		-	recM01	* m22;
		m11 =  recM00 * m22		-	recM20	* recM02;
		m21 =  recM01 * recM20	-	recM00	* m21;
		m02 =  recM01 * m12		-	recM11	* recM02;
		m12 =  recM02 * recM10	-	recM00	* m12;
		m22 =  recM00 * recM11	-	recM10	* recM01;

		return this->operator*=(1.0f / det);
	}

	Matrix3& Matrix3::Orthogonalize()
	{
		Vector3 e0 = Vector3(m00, m10, m20);
		Vector3 e1 = Vector3(m01, m11, m21);
		Vector3 e2 = Vector3(m02, m12, m22);

		Vector3::GramSchmidt(e0, e1, e2);

		m00 = e0.x;
		m10 = e0.y;
		m20 = e0.z;

		m01 = e1.x;
		m11 = e1.y;
		m21 = e1.z;

		m02 = e2.x;
		m12 = e2.y;
		m22 = e2.z;

		return *this;
	}

	Vector3 Matrix3::ExtractScaling(float tolerance)
	{
		Vector3 scale;

		const float squareSum0 = (m00 * m00) + (m10 * m10) + (m20 * m20);
		const float squareSum1 = (m01 * m01) + (m11 * m11) + (m21 * m21);
		const float squareSum2 = (m02 * m02) + (m12 * m12) + (m22 * m22);

		if (squareSum0 > tolerance)
		{
			float scale0 = sqrtf(squareSum0);
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

		if (squareSum1 > tolerance)
		{
			float scale1 = sqrtf(squareSum1);
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

		if (squareSum2 > tolerance)
		{
			float scale2 = sqrtf(squareSum2);
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

	void Matrix3::QRDecompositionHR(Matrix3& Q, Matrix3& R) const
	{
		Vector3 m1 = Vector3{ m00, m10, m20 };
		Vector3 u = m1 - Vector3{ m1.Norm(), 0.0f, 0.0f };
		Vector3 v = u / u.Norm();

		Matrix3 Q1 = Matrix3{
			1.0f - 2.0f * v.x * v.x, -2.0f * v.x * v.y, -2.0f * v.x * v.z,
			-2.0f * v.y * v.x, 1.0f - 2.0f * v.y * v.y, -2.0f * v.y * v.z,
			-2.0f * v.z * v.x, -2.0f * v.z * v.y, 1.0f - 2.0f * v.z * v.z
		};

		Matrix3 Q1M = Q1 * *this;

		Vector2 mp1 = Vector2{ Q1M.m11, Q1M.m21 };
		Vector2 up = mp1 - Vector2{ mp1.Norm(), 0.0f };
		Vector2 vp = up / up.Norm();

		Matrix3 Q2 = Matrix3{
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f - 2.0f * vp.x * vp.x, -2.0f * vp.x * vp.y,
			0.0f, -2.0f * vp.y * vp.x, 1.0f - 2.0f * vp.y * vp.y
		};

		Q = Q1.Transpose() * Q2.Transpose();
		R = Q.GetTransposed() * *this;
	}

	void Matrix3::QRDecompositionGR(Matrix3& Q, Matrix3& R) const
	{
		float angle = atan2f(m20, m00);

		Matrix3 GR1 = Matrix3(
			cosf(angle),	0.0f,	sinf(angle),
			0.0f,			1.0f,	0.0f,
			-sinf(angle),	0.0f,	cosf(angle)
		);

		Matrix3 Ap = GR1 * *this;

		angle = atan2f(Ap.m10, Ap.m00);

		Matrix3 GR2 = Matrix3(
			cosf(angle), sinf(angle), 0.0f,
			-sinf(angle), cosf(angle), 0.0f,
			0.0f, 0.0f, 1.0f
		);

		Ap = GR2 * Ap;

		angle = atan2f(Ap.m21, Ap.m11);

		Matrix3 GR3 = Matrix3(
			1.0f,	0.0f,			0.0f,
			0.0f,	cosf(angle),	sinf(angle),
			0.0f,	-sinf(angle),	cosf(angle)
		);

		Q = GR3 * GR2 * GR1;
		R = Q * *this;

		Q.Transpose();
	}

	Quaternion Matrix3::ToQuaternion() const
	{
		Quaternion q;

		float trace = m00 + m11 + m22;
		if (trace > 0.0f)
		{
			float invS = InvSqrt(trace + 1.0f);
			q.w = 0.5f / invS;

			float s = 0.5f * invS;
			q.x = (m21 - m12) * s;
			q.y = (m02 - m20) * s;
			q.z = (m10 - m01) * s; 
		}
		else 
		{
			if ((m00 > m11) && (m00 > m22)) 
			{
				float invS = InvSqrt(1.0f + m00 - m11 - m22);
				q.x = 0.5f / invS;

				float s = 0.5f * invS;
				q.w = (m21 - m12) * s;
				q.y = (m10 + m01) * s;
				q.z = (m02 + m20) * s;
			}
			else if (m11 > m22) 
			{
				float invS = InvSqrt(1.0f - m00 + m11 - m22);
				q.y = 0.5f / invS;

				float s = 0.5f * invS;
				q.w = (m02 - m20) * s;
				q.x = (m10 + m01) * s;
				q.z = (m21 + m12) * s;
			}
			else 
			{
				float invS = InvSqrt(1.0f - m00 - m11 + m22);
				q.z = 0.5f / invS;

				float s = 0.5f * invS;
				q.w = (m10 - m01) * s;
				q.x = (m02 + m20) * s;
				q.y = (m12 + m21) * s;
			}
		}

		return q;
	}

	Matrix3 Matrix3::FromColumns(const Vector3& col1, const Vector3& col2, const Vector3& col3)
	{
		return Matrix3(
			col1.x, col2.x, col3.x,
			col1.y, col2.y, col3.y,
			col1.z, col2.z, col3.z);
	}

	Matrix3 Matrix3::FromRows(const Vector3& row1, const Vector3& row2, const Vector3& row3)
	{
		return Matrix3(
			row1.x, row1.y, row1.z,
			row2.x, row2.y, row2.z,
			row3.x, row3.y, row3.z);
	}

	Matrix3 Matrix3::MakeRotation(const Vector3& eulerAngles)
	{
		Matrix3 RX = Identity();
		RX.m11 = cosf(eulerAngles.x);
		RX.m12 = -sinf(eulerAngles.x);
		RX.m21 = sinf(eulerAngles.x);
		RX.m22 = cosf(eulerAngles.x);

		Matrix3 RY = Identity();
		RY.m00 = cosf(eulerAngles.y);
		RY.m02 = sinf(eulerAngles.y);
		RY.m20 = -sinf(eulerAngles.y);
		RY.m22 = cosf(eulerAngles.y);

		Matrix3 RZ = Identity();
		RZ.m00 = cosf(eulerAngles.z);
		RZ.m01 = -sinf(eulerAngles.z);
		RZ.m10 = sinf(eulerAngles.z);
		RZ.m11 = cosf(eulerAngles.z);

		//return RZ * RX * RY;
		//return RZ * RY * RX;
		return RY * RX * RZ;
		//return RY * RZ * RX;
		//return RY * RX * RZ;
		//return RX * RZ * RY;
	}

	Vector3 Matrix3::operator*(const Vector3& rhs) const
	{
		return Vector3{
			m00 * rhs.x + m01 * rhs.y + m02 * rhs.z,
			m10 * rhs.x + m11 * rhs.y + m12 * rhs.z,
			m20 * rhs.x + m21 * rhs.y + m22 * rhs.z
		};
	}

	Matrix3 Matrix3::operator*(const Matrix3& rhs) const
	{
		return Matrix3{
			m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20,
			m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21,
			m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22,

			m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20,
			m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21,
			m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22,

			m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20,
			m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21,
			m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22
		};
	}

	Matrix3& Matrix3::operator=(const Matrix3& rhs)
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

		return *this;
	}

	Matrix3& Matrix3::operator=(Matrix3&& rhs)
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

		return *this;
	}

	Matrix3& Matrix3::operator*=(const float rhs)
	{
		m00 *= rhs;
		m01 *= rhs;
		m02 *= rhs;
		m10 *= rhs;
		m11 *= rhs;
		m12 *= rhs;
		m20 *= rhs;
		m21 *= rhs;
		m22 *= rhs;

		return *this;
	}

	Matrix3& Matrix3::operator/=(const float rhs)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyEqual(rhs, 0.0f))
		{
			LOG_ERROR("Matrix3::operator/ - divisor null")
				return Invalidate();
		}
	#endif

		m00 /= rhs;
		m01 /= rhs;
		m02 /= rhs;

		m10 /= rhs;
		m11 /= rhs;
		m12 /= rhs;

		m20 /= rhs;
		m21 /= rhs;
		m22 /= rhs;

		return *this;
	}

	Matrix3& Matrix3::operator+=(const Matrix3& rhs)
	{
		m00 += rhs.m00;
		m01 += rhs.m01;
		m02 += rhs.m02;

		m10 += rhs.m10;
		m11 += rhs.m11;
		m12 += rhs.m12;

		m20 += rhs.m20;
		m21 += rhs.m21;
		m22 += rhs.m22;

		return *this;
	}

	Matrix3& Matrix3::operator-=(const Matrix3& rhs)
	{
		m00 -= rhs.m00;
		m01 -= rhs.m01;
		m02 -= rhs.m02;

		m10 -= rhs.m10;
		m11 -= rhs.m11;
		m12 -= rhs.m12;

		m20 -= rhs.m20;
		m21 -= rhs.m21;
		m22 -= rhs.m22;

		return *this;
	}

	Matrix3& Matrix3::operator*=(const Matrix3& rhs)
	{
		float recM00 = m00;
		float recM01 = m01;
		float recM10 = m10;
		float recM11 = m11;
		float recM20 = m20;
		float recM21 = m21;

		m00 = recM00 * rhs.m00 + recM01 * rhs.m10 + m02 * rhs.m20;
		m01 = recM00 * rhs.m01 + recM01 * rhs.m11 + m02 * rhs.m21;
		m02 = recM00 * rhs.m02 + recM01 * rhs.m12 + m02 * rhs.m22;

		m10 = recM10 * rhs.m00 + recM11 * rhs.m10 + m12 * rhs.m20;
		m11 = recM10 * rhs.m01 + recM11 * rhs.m11 + m12 * rhs.m21;
		m12 = recM10 * rhs.m02 + recM11 * rhs.m12 + m12 * rhs.m22;

		m20 = recM20 * rhs.m00 + recM21 * rhs.m10 + m22 * rhs.m20;
		m21 = recM20 * rhs.m01 + recM21 * rhs.m11 + m22 * rhs.m21;
		m22 = recM20 * rhs.m02 + recM21 * rhs.m12 + m22 * rhs.m22;

		return *this;
	}

	Matrix3::operator Matrix4() const
	{
		return Matrix4{
			m00, m01, m02, 0.0f,
			m10, m11, m12, 0.0f,
			m20, m21, m22, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	float& Matrix3::operator[](const uint8 index)
	{
	#if (ROW_MAJOR == 1)
		if (index == 0u)
			return m00;
		if (index == 1u)
			return m01;
		if (index == 2u)
			return m02;
		if (index == 3u)
			return m10;
		if (index == 4u)
			return m11;
		if (index == 5u)
			return m12;
		if (index == 6u)
			return m20;
		if (index == 7u)
			return m21;
		if (index == 8u)
			return m22;
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
	#endif

		LOG_ERROR("float& Matrix3[" + std::to_string(index) + "] Element out of Array")
			return m00;
	}

	float Matrix3::operator[](const uint8 index) const
	{
	#if (ROW_MAJOR == 1)
		if (index == 0u)
			return m00;
		if (index == 1u)
			return m01;
		if (index == 2u)
			return m02;
		if (index == 3u)
			return m10;
		if (index == 4u)
			return m11;
		if (index == 5u)
			return m12;
		if (index == 6u)
			return m20;
		if (index == 7u)
			return m21;
		if (index == 8u)
			return m22;
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
	#endif

		LOG_ERROR("float Matrix3[" + std::to_string(index) + "] Element out of Array")
			return 0.0f;
	}
}