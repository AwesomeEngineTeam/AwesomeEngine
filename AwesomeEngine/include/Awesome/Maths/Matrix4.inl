namespace aw
{
	inline Matrix4::Matrix4(
		const float _m00, const float _m01, const float _m02, const float _m03,
		const float _m10, const float _m11, const float _m12, const float _m13,
		const float _m20, const float _m21, const float _m22, const float _m23,
		const float _m30, const float _m31, const float _m32, const float _m33) :
		m00{ _m00 },
		m10{ _m10 },
		m20{ _m20 },
		m30{ _m30 },
		m01{ _m01 },
		m11{ _m11 },
		m21{ _m21 },
		m31{ _m31 },
		m02{ _m02 },
		m12{ _m12 },
		m22{ _m22 },
		m32{ _m32 },
		m03{ _m03 },
		m13{ _m13 },
		m23{ _m23 },
		m33{ _m33 }
	{
	}

	inline Matrix4::Matrix4(const float arr[16]) :
		Matrix4(
			#if (ROW_MAJOR == 1)
			arr[0], arr[4], arr[8], arr[12],
			arr[1], arr[5], arr[9], arr[13],
			arr[2], arr[6], arr[10], arr[14],
			arr[3], arr[7], arr[11], arr[15]
			#else
			arr[0], arr[1], arr[2], arr[3],
			arr[4], arr[5], arr[6], arr[7],
			arr[8], arr[9], arr[10], arr[11],
			arr[12], arr[13], arr[14], arr[15]
			#endif
		)
	{
	}

	inline Matrix4::Matrix4(const Matrix4& other) :
		m00{ other.m00 },
		m10{ other.m10 },
		m20{ other.m20 },
		m30{ other.m30 },
		m01{ other.m01 },
		m11{ other.m11 },
		m21{ other.m21 },
		m31{ other.m31 },
		m02{ other.m02 },
		m12{ other.m12 },
		m22{ other.m22 },
		m32{ other.m32 },
		m03{ other.m03 },
		m13{ other.m13 },
		m23{ other.m23 },
		m33{ other.m33 }
	{
	}

	inline Matrix4::Matrix4(Matrix4&& other) :
		m00{ std::move(other.m00) },
		m10{ std::move(other.m10) },
		m20{ std::move(other.m20) },
		m30{ std::move(other.m30) },
		m01{ std::move(other.m01) },
		m11{ std::move(other.m11) },
		m21{ std::move(other.m21) },
		m31{ std::move(other.m31) },
		m02{ std::move(other.m02) },
		m12{ std::move(other.m12) },
		m22{ std::move(other.m22) },
		m32{ std::move(other.m32) },
		m03{ std::move(other.m03) },
		m13{ std::move(other.m13) },
		m23{ std::move(other.m23) },
		m33{ std::move(other.m33) }
	{
	}
	
	inline Matrix4 Matrix4::GetTransposed() const
	{
		return Matrix4{
			m00, m10, m20, m30,
			m01, m11, m21, m31,
			m02, m12, m22, m32,
			m03, m13, m23, m33
		};
	}
	
	inline Matrix4& Matrix4::Transpose()
	{
		std::swap(m10, m01);
		std::swap(m20, m02);
		std::swap(m30, m03);
		std::swap(m21, m12);
		std::swap(m31, m13);
		std::swap(m32, m23);

		return *this;
	}

	inline Matrix4& Matrix4::DiscardTranslation()
	{
		m30 = 0.0f;
		m31 = 0.0f;
		m32 = 0.0f;

		return *this;
	}

#if AWESOME_MATHS_SAFE
	inline Matrix4& Matrix4::Invalidate()
	{
		m00 = m01 = m02 = m03 = m10 = m11 = m12 = m13 = m20 = m21 = m22 = m23 = m30 = m31 = m32 = m33 = 0.0f;

		return *this;
	}
#endif

	inline float Matrix4::Trace() const
	{
		return m00 + m11 + m22 + m33;
	}

	inline bool Matrix4::IsHomogenous() const
	{
		return NearlyEqual(m03, 0.0f) && NearlyEqual(m13, 0.0f) && NearlyEqual(m23, 0.0f) &&
			NearlyEqual(m30, 0.0f) && NearlyEqual(m31, 0.0f) && NearlyEqual(m32, 0.0f) &&
			NearlyEqual(m33, 1.0f);
	}

	inline std::string Matrix4::ToString() const
	{
		return "[Matrix4]\n|" + 
			std::to_string(m00) + '\t' + std::to_string(m01) + '\t' + std::to_string(m02) + '\t' + std::to_string(m03) + "|\n|" +
			std::to_string(m10) + '\t' + std::to_string(m11) + '\t' + std::to_string(m12) + '\t' + std::to_string(m13) + "|\n|" +
			std::to_string(m20) + '\t' + std::to_string(m21) + '\t' + std::to_string(m22) + '\t' + std::to_string(m23) + "|\n|" +
			std::to_string(m30) + '\t' + std::to_string(m31) + '\t' + std::to_string(m32) + '\t' + std::to_string(m33) + '|';
	}

	inline const Matrix4& Matrix4::Zero()
	{
		static const Matrix4 awMatrix4Zero{};
		return awMatrix4Zero;
	}

	inline const Matrix4& Matrix4::Identity()
	{
		static const Matrix4 awMatrix4Identity{
			1.0f, 0.0f, 0.0f, 0.0f, 
			0.0f, 1.0f, 0.0f, 0.0f, 
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
		return awMatrix4Identity;
	}

	inline Matrix4 Matrix4::operator+() const
	{
		return *this;
	}
	
	inline Matrix4 Matrix4::operator-() const
	{
		return Matrix4{
			-m00, -m01, -m02, -m03,
			-m10, -m11, -m12, -m13,
			-m20, -m21, -m22, -m23,
			-m30, -m31, -m32, -m33 
		};
	}
	
	inline Matrix4 Matrix4::operator*(const float rhs) const
	{
		return Matrix4{
			rhs * m00, rhs * m01, rhs * m02, rhs * m03,
			rhs * m10, rhs * m11, rhs * m12, rhs * m13,
			rhs * m20, rhs * m21, rhs * m22, rhs * m23,
			rhs * m30, rhs * m31, rhs * m32, rhs * m33 
		};
	}

	inline Matrix4 Matrix4::operator/(const float rhs) const
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
		{
			LOG_ERROR("Matrix4::operator/ - divisor null")
				return Zero();
		}
	#endif

		return Matrix4{
			m00 / rhs, m01 / rhs, m02 / rhs, m03 / rhs,
			m10 / rhs, m11 / rhs, m12 / rhs, m13 / rhs,
			m20 / rhs, m21 / rhs, m22 / rhs, m23 / rhs,
			m30 / rhs, m31 / rhs, m32 / rhs, m33 / rhs
		};
	}
	
	inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const
	{
		return Matrix4{
			m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02, m03 + rhs.m03,
			m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12, m13 + rhs.m13,
			m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22, m23 + rhs.m23,
			m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32, m33 + rhs.m33
		};
	}
	
	inline Matrix4 Matrix4::operator-(const Matrix4& rhs) const
	{
		return Matrix4{
			m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02, m03 - rhs.m03,
			m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12, m13 - rhs.m13,
			m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22, m23 - rhs.m23,
			m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32, m33 - rhs.m33
		};
	}
		
	inline bool Matrix4::operator==(const Matrix4& rhs) const
	{
		return
			NearlyEqual(m00, rhs.m00) &&
			NearlyEqual(m10, rhs.m10) &&
			NearlyEqual(m20, rhs.m20) &&
			NearlyEqual(m30, rhs.m30) &&
			NearlyEqual(m01, rhs.m01) &&
			NearlyEqual(m11, rhs.m11) &&
			NearlyEqual(m21, rhs.m21) &&
			NearlyEqual(m31, rhs.m31) &&
			NearlyEqual(m02, rhs.m02) &&
			NearlyEqual(m12, rhs.m12) &&
			NearlyEqual(m22, rhs.m22) &&
			NearlyEqual(m32, rhs.m32) &&
			NearlyEqual(m03, rhs.m03) &&
			NearlyEqual(m13, rhs.m13) &&
			NearlyEqual(m23, rhs.m23) &&
			NearlyEqual(m33, rhs.m33);
	}
	
	inline bool Matrix4::operator!=(const Matrix4& rhs) const
	{
		return !(this->operator==(rhs));
	}

	inline float& Matrix4::operator()(const uint8 rowIndex, const uint8 columnIndex)
	{
	#if AWESOME_MATHS_SAFE
		if (rowIndex > 3 || columnIndex > 3)
		{
			LOG_ERROR("float& Matrix4(" + std::to_string(rowIndex) + ", " + std::to_string(columnIndex) + ") out of range");
			return m00;
		}
	#endif

	#if (ROW_MAJOR == 1)
		return this->operator[](rowIndex * 4 + columnIndex);
	#else
		return this->operator[](rowIndex + columnIndex * 4);
	#endif
	}
	
	inline float Matrix4::operator()(const uint8 rowIndex, const uint8 columnIndex) const
	{
	#if AWESOME_MATHS_SAFE
		if (rowIndex > 3 || columnIndex > 3)
		{
			LOG_ERROR("float Matrix4(" + std::to_string(rowIndex) + ", " + std::to_string(columnIndex) + ") out of range");
			return m00;
		}
	#endif

	#if (ROW_MAJOR == 1)
		return this->operator[](rowIndex * 4 + columnIndex);
	#else
		return this->operator[](rowIndex + columnIndex * 4);
	#endif
	}

	inline Matrix4 operator*(const float lhs, const Matrix4& rhs)
	{
		return rhs * lhs;
	}
}