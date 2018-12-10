namespace aw
{
	inline Matrix3::Matrix3(
		const float _m00, const float _m01, const float _m02,
		const float _m10, const float _m11, const float _m12,
		const float _m20, const float _m21, const float _m22) :
		m00{ _m00 },
		m10{ _m10 },
		m20{ _m20 },
		m01{ _m01 },
		m11{ _m11 },
		m21{ _m21 },
		m02{ _m02 },
		m12{ _m12 },
		m22{ _m22 }
	{
	}

	inline Matrix3::Matrix3(const Matrix3& other) :
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

	inline Matrix3::Matrix3(Matrix3&& other) :
		m00{ std::move(other.m00) },
		m10{ std::move(other.m10) },
		m20{ std::move(other.m20) },
		m01{ std::move(other.m01) },
		m11{ std::move(other.m11) },
		m21{ std::move(other.m21) },
		m02{ std::move(other.m02) },
		m12{ std::move(other.m12) },
		m22{ std::move(other.m22) }
	{
	}

	inline Matrix3 Matrix3::GetTransposed() const
	{
		return Matrix3(
			m00, m10, m20,
			m01, m11, m21,
			m02, m12, m22
		);
	}

	inline Matrix3& Matrix3::Transpose()
	{
		std::swap(m10, m01);
		std::swap(m20, m02);
		std::swap(m21, m12);

		return *this;
	}

#if AWESOME_MATHS_SAFE
	inline Matrix3& Matrix3::Invalidate()
	{
		m00 = 0.0f;
		m01 = 0.0f;
		m02 = 0.0f;
		m10 = 0.0f;
		m11 = 0.0f;
		m12 = 0.0f;
		m20 = 0.0f;
		m21 = 0.0f;
		m22 = 0.0f;

		return *this;
	}
#endif

	inline float Matrix3::Trace() const
	{
		return m00 + m11 + m22;
	}

	inline float Matrix3::Determinant() const
	{
		return 
			m00 * m11 * m22 + 
			m01 * m12 * m20 + 
			m02 * m10 * m21 -
			m20 * m11 * m02 -
			m21 * m12 * m00 -
			m22 * m10 * m01;
	}

	inline Matrix3& Matrix3::MultiplyRow(uint8 row, float factor)
	{
		if (row == 0u)
		{
			m00 *= factor;
			m01 *= factor;
			m02 *= factor;
			return *this;
		}
		if (row == 1u)
		{
			m10 *= factor;
			m11 *= factor;
			m12 *= factor;
			return *this;
		}
		if (row == 2u)
		{
			m20 *= factor;
			m21 *= factor;
			m22 *= factor;
			return *this;
		}

		return *this;

		LOG_ERROR("Matrix3::MultiplyRow - row inaccessible : row " + std::to_string(row + 1));
	}

	inline Matrix3& Matrix3::MultiplyRows(float factor1, float factor2, float factor3)
	{
		MultiplyRow(0, factor1);
		MultiplyRow(1, factor2);
		MultiplyRow(2, factor3);

		return *this;
	}

	inline Matrix3& Matrix3::DivideRow(uint8 row, float divisor)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(divisor))
		{
			LOG_ERROR("Matrix3::DivideRow - divisor null")
				return Invalidate();
		}
	#endif

		if (row == 0u)
		{
			m00 /= divisor;
			m01 /= divisor;
			m02 /= divisor;
			return *this;
		}
		if (row == 1u)
		{
			m10 /= divisor;
			m11 /= divisor;
			m12 /= divisor;
			return *this;
		}
		if (row == 2u)
		{
			m20 /= divisor;
			m21 /= divisor;
			m22 /= divisor;
			return *this;
		}

		return *this;

		LOG_ERROR("Matrix3::DivideRow - row inaccessible : row " + std::to_string(row + 1))
	}

	inline Matrix3& Matrix3::DivideRows(float divisor1, float divisor2, float divisor3)
	{
		DivideRow(0, divisor1);
		DivideRow(1, divisor2);
		DivideRow(2, divisor3);

		return *this;
	}

	inline Matrix3& Matrix3::MultiplyColumn(uint8 column, float factor)
	{
		if (column == 0u)
		{
			m00 *= factor;
			m10 *= factor;
			m20 *= factor;
			return *this;
		}
		if (column == 1u)
		{
			m01 *= factor;
			m11 *= factor;
			m21 *= factor;
			return *this;
		}
		if (column == 2u)
		{
			m02 *= factor;
			m12 *= factor;
			m22 *= factor;
			return *this;
		}

		return *this;

		LOG_ERROR("Matrix3::MultiplyColumn - column inaccessible : column " + std::to_string(column + 1))
	}

	inline Matrix3& Matrix3::MultiplyColumns(float factor1, float factor2, float factor3)
	{
		MultiplyColumn(0, factor1);
		MultiplyColumn(1, factor2);
		MultiplyColumn(2, factor3);

		return *this;
	}

	inline Matrix3& Matrix3::DivideColumn(uint8 column, float divisor)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(divisor))
		{
			LOG_ERROR("Matrix3::DivideColumn - divisor null")
				return Invalidate();
		}
	#endif

		if (column == 0u)
		{
			m00 /= divisor;
			m10 /= divisor;
			m20 /= divisor;
			return *this;
		}
		if (column == 1u)
		{
			m01 /= divisor;
			m11 /= divisor;
			m21 /= divisor;
			return *this;
		}
		if (column == 2u)
		{
			m02 /= divisor;
			m12 /= divisor;
			m22 /= divisor;
			return *this;
		}

		return *this;

		LOG_ERROR("Matrix3::DivideColumn - column inaccessible : column " + std::to_string(column + 1));
	}

	inline Matrix3& Matrix3::DivideColumns(float divisor1, float divisor2, float divisor3)
	{
		DivideColumn(0, divisor1);
		DivideColumn(1, divisor2);
		DivideColumn(2, divisor3);

		return *this;
	}

	inline bool Matrix3::IsDiagonal() const
	{
		return 
			NearlyEqual(m01, 0.0f) &&
			NearlyEqual(m02, 0.0f) &&
			NearlyEqual(m10, 0.0f) &&
			NearlyEqual(m12, 0.0f) &&
			NearlyEqual(m20, 0.0f) &&
			NearlyEqual(m21, 0.0f);
	}

	inline bool Matrix3::IsOrthogonal() const
	{
		return 
			NearlyEqual(m00 * m01 +	m10 * m11 +	m20 * m21, 0.0f) &&
			NearlyEqual(m00 * m02 + m10 * m12 + m20 * m22, 0.0f) &&
			NearlyEqual(m01 * m02 + m11 * m12 + m21 * m22, 0.0f) &&
			NearlyEqual(m00 * m00 + m10 * m10 + m20 * m20, 1.0f) &&
			NearlyEqual(m01 * m01 + m11 * m11 + m21 * m21, 1.0f) &&
			NearlyEqual(m02 * m02 + m12 * m12 + m22 * m22, 1.0f);
	}

	inline std::string Matrix3::ToString() const
	{
		return "[Matrix3]\n|" +
			std::to_string(m00) + '\t' + std::to_string(m01) + '\t' + std::to_string(m02) + "|\n|" +
			std::to_string(m10) + '\t' + std::to_string(m11) + '\t' + std::to_string(m12) + "|\n|" + 
			std::to_string(m20) + '\t' + std::to_string(m21) + '\t' + std::to_string(m22) + '|';
	}

	inline const Matrix3& Matrix3::Zero()
	{
		static const Matrix3 awMatrix3Zero{};
		return awMatrix3Zero;
	}

	inline const Matrix3& Matrix3::Identity()
	{
		static const Matrix3 awMatrix3Identity{ 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		return awMatrix3Identity;
	}

	inline Matrix3 Matrix3::operator+() const
	{
		return *this;
	}

	inline Matrix3 Matrix3::operator-() const
	{
		return Matrix3{
			-m00, -m01, -m02,
			-m10, -m11, -m12,
			-m20, -m21, -m22
		};
	}

	inline Matrix3 Matrix3::operator*(const float rhs) const
	{
		return Matrix3{
			rhs * m00,	rhs * m01,	rhs * m02,
			rhs * m10,	rhs * m11,	rhs * m12,
			rhs * m20,	rhs * m21,	rhs * m22
		};
	}

	inline Matrix3 Matrix3::operator/(const float rhs) const
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
		{
			LOG_ERROR("Matrix3::operator/ - divisor null")
				return Zero();
		}
	#endif

		return Matrix3{
			m00 / rhs, m01 / rhs, m02 / rhs,
			m10 / rhs, m11 / rhs, m12 / rhs,
			m20 / rhs, m21 / rhs, m22 / rhs
		};
	}

	inline Matrix3 Matrix3::operator+(const Matrix3& rhs) const
	{
		return Matrix3{
			m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
			m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
			m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22
		};
	}

	inline Matrix3 Matrix3::operator-(const Matrix3& rhs) const
	{
		return Matrix3{
			m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
			m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
			m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22
		};
	}

	inline bool Matrix3::operator==(const Matrix3& rhs) const
	{
		return NearlyEqual(m00, rhs.m00) &&
			NearlyEqual(m10, rhs.m10) &&
			NearlyEqual(m20, rhs.m20) &&
			NearlyEqual(m01, rhs.m01) &&
			NearlyEqual(m11, rhs.m11) &&
			NearlyEqual(m21, rhs.m21) &&
			NearlyEqual(m02, rhs.m02) &&
			NearlyEqual(m12, rhs.m12) &&
			NearlyEqual(m22, rhs.m22);
	}

	inline bool Matrix3::operator!=(const Matrix3& rhs) const
	{
		return !(this->operator==(rhs));
	}

	inline float& Matrix3::operator()(const uint8 rowIndex, const uint8 columnIndex)
	{
	#if AWESOME_MATHS_SAFE
		if (rowIndex > 2 || columnIndex > 2)
		{
			LOG_ERROR("float& Matrix3(" + std::to_string(rowIndex) + ", " + std::to_string(columnIndex) + ") out of range")
				return m00;
		}
	#endif

	#if (ROW_MAJOR == 1)
		return this->operator[](rowIndex * 3 + columnIndex);
	#else
		return this->operator[](rowIndex + columnIndex * 3);
	#endif
	}

	inline float Matrix3::operator()(const uint8 rowIndex, const uint8 columnIndex) const
	{
	#if AWESOME_MATHS_SAFE
		if (rowIndex > 2 || columnIndex > 2)
		{
			LOG_ERROR("float& Matrix3(" + std::to_string(rowIndex) + ", " + std::to_string(columnIndex) + ") out of range")
				return 0.0f;
		}
	#endif

	#if (ROW_MAJOR == 1)
		return this->operator[](rowIndex * 3 + columnIndex);
	#else
		return this->operator[](rowIndex + columnIndex * 3);
	#endif
	}

	inline Matrix3 operator*(const float lhs, const Matrix3& rhs)
	{
		return rhs * lhs;
	}
}