namespace aw
{
	inline Quaternion::Quaternion(const float _w, const float _x, const float _y, const float _z) :
		w{ _w },
		x{ _x },
		y{ _y },
		z{ _z }
	{
	}

	inline Quaternion::Quaternion(const Quaternion& other) :
		w{ other.w.load() },
		x{ other.x.load() },
		y{ other.y.load() },
		z{ other.z.load() }
	{
	}

	inline Quaternion::Quaternion(Quaternion&& other) :
		w{ std::move(other.w.load()) },
		x{ std::move(other.x.load()) },
		y{ std::move(other.y.load()) },
		z{ std::move(other.z.load()) }
	{
	}

	inline Quaternion Quaternion::GetConjugated() const
	{
		return Quaternion(w, -x, -y, -z);
	}
	
	inline Quaternion Quaternion::GetInversed() const
	{
		float n = SquaredNorm();

		if (NearlyEqual(n, 1.0f))
			return GetConjugated();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(n))
		{
			LOG_ERROR("Quaternion::GetInversed - not inversible");
				return Quaternion::Zero();
		}
	#endif

		return Quaternion(w / n, -x / n, -y / n, -z / n);
	}
	
	inline Quaternion Quaternion::GetNormalized() const
	{
		float n = Norm();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(n))
		{
			LOG_ERROR("Quaternion::GetNormalized - norm null")
				return Quaternion::Zero();
		}
	#endif // AWESOME_MATHS_SAFE

		return Quaternion(w / n, x / n, y / n, z / n);
	}
	
	inline Quaternion Quaternion::GetClamped(const Quaternion& qMin, const Quaternion& qMax) const
	{
		return Quaternion(
			aw::GetClamped(w, qMin.w, qMax.w),
			aw::GetClamped(x, qMin.x, qMax.x),
			aw::GetClamped(y, qMin.y, qMax.y),
			aw::GetClamped(z, qMin.z, qMax.z)
		);
	}
	
	inline Quaternion& Quaternion::Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;

		return *this;
	}
	
	inline Quaternion& Quaternion::Inverse()
	{
		float n = SquaredNorm();

		if (NearlyEqual(n, 1.0f))
			return Conjugate();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(n))
		{
			LOG_ERROR("Quaternion::Inverse - not inversible");
				return Invalidate();
		}
	#endif

		w = w / n;
		x = x / -n;
		y = y / -n;
		z = z / -n;

		return *this;
	}
	
	inline Quaternion& Quaternion::Normalize()
	{
		float n = Norm();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(n))
		{
			w = 1.0f;
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;

			LOG_ERROR("Quaternion::Normalize - norm null")
				return *this;
		}
	#endif // AWESOME_MATHS_SAFE

		w = w / n;
		x = x / n;
		y = y / n;
		z = z / n;

		return *this;
	}
	
	inline Quaternion& Quaternion::Clamp(const Quaternion& qMin, const Quaternion& qMax)
	{
		w = aw::GetClamped(w, qMin.w, qMax.w);
		x = aw::GetClamped(x, qMin.x, qMax.x);
		y = aw::GetClamped(y, qMin.y, qMax.y);
		z = aw::GetClamped(z, qMin.z, qMax.z);

		return *this;
	}

#if AWESOME_MATHS_SAFE
	inline Quaternion& Quaternion::Invalidate()
	{
		w = 0.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;

		return *this;
	}
#endif

	inline float Quaternion::Norm() const
	{
		return sqrtf(SquaredNorm());
	}
	
	inline float Quaternion::SquaredNorm() const
	{
		return SumSquare(w, x, y, z);
	}
	
	inline float Quaternion::Dot(const Quaternion& other) const
	{
		return w * other.w + x * other.x + y * other.y + z * other.z;
	}
	
	inline Quaternion Quaternion::Cross(const Quaternion& other) const
	{
		// w = 1 and Cross Vec3
		return Quaternion(1.0f, y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
	
	inline std::string Quaternion::ToString() const
	{
		return "[Quaternion] w = " + std::to_string(w) + "\tx = " + std::to_string(x) + 
			"\ty = " + std::to_string(y) + "\tz = " + std::to_string(z);
	}

	inline Quaternion Quaternion::Lerp(const Quaternion& start, const Quaternion& end, float percent)
	{
		aw::Clamp(percent, 0.0f, 1.0f);
		return start * (1.0f - percent) + end * percent;
	}

	inline Quaternion Quaternion::NLerp(const Quaternion& start, const Quaternion& end, float percent)
	{
		aw::Clamp(percent, 0.0f, 1.0f);
		return (start * (1.0f - percent) + end * percent).Normalize();
	}

	inline const Quaternion& Quaternion::Zero()
	{
		static const Quaternion awQuaternionZero{};
		return awQuaternionZero;
	}

	inline const Quaternion& Quaternion::Identity()
	{
		static const Quaternion awQuaternionIdentity{ 1.0f, 0.0f, 0.0f, 0.0f };
		return awQuaternionIdentity;
	}

	inline Quaternion Quaternion::operator+() const
	{
		return *this;
	}

	inline Quaternion Quaternion::operator-() const
	{
		return Quaternion(-w, -x, -y, -z);
	}

	inline Quaternion Quaternion::operator*(const float rhs) const
	{
		return Quaternion(w * rhs, x * rhs, y * rhs, z * rhs);
	}

	inline Quaternion Quaternion::operator/(const float rhs) const
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
		{
			LOG_ERROR("Quaternion::operator/ - divisor null");
				return Identity();
		}
	#endif

		return Quaternion(w / rhs, x / rhs, y / rhs, z / rhs);
	}

	inline Quaternion Quaternion::operator+(const Quaternion& rhs) const
	{
		return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline Quaternion Quaternion::operator-(const Quaternion& rhs) const
	{
		return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
	}

    inline Quaternion Quaternion::operator^(const Quaternion& rhs) const
    {
        return Cross(rhs);
    }

    inline float Quaternion::operator|(const Quaternion& rhs) const
    {
        return Dot(rhs);
    }

	inline Quaternion& Quaternion::operator=(const Quaternion& rhs)
	{
		w = rhs.w.load();
		x = rhs.x.load();
		y = rhs.y.load();
		z = rhs.z.load();

		return *this;
	}

	inline Quaternion& Quaternion::operator=(Quaternion&& rhs)
	{
		w = std::move(rhs.w.load());
		x = std::move(rhs.x.load());
		y = std::move(rhs.y.load());
		z = std::move(rhs.z.load());

		return *this;
	}

	inline Quaternion& Quaternion::operator*=(const float rhs)
	{
		w = w * rhs;
		x = x * rhs;
		y = y * rhs;
		z = z * rhs;

		return *this;
	}

	inline Quaternion& Quaternion::operator/=(const float rhs)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
		{
			LOG_ERROR("Quaternion::operator/= - divisor null");
				return Invalidate();
		}
	#endif

		w = w / rhs;
		x = x / rhs;
		y = y / rhs;
		z = z / rhs;

		return *this;
	}

	inline Quaternion& Quaternion::operator+=(const Quaternion& rhs)
	{
		w = w + rhs.w;
		x = x + rhs.x;
		y = y + rhs.y;
		z = z + rhs.z;

		return *this;
	}

	inline Quaternion& Quaternion::operator-=(const Quaternion& rhs)
	{
		w = w - rhs.w;
		x = x - rhs.x;
		y = y - rhs.y;
		z = z - rhs.z;

		return *this;
	}

	inline bool Quaternion::operator==(const Quaternion& rhs) const
	{
		return NearlyEqual(w, rhs.w) && NearlyEqual(x, rhs.x) && NearlyEqual(y, rhs.y) && NearlyEqual(z, rhs.z);
	}

	inline bool Quaternion::operator!=(const Quaternion& rhs) const
	{
		return !(*this == rhs);
	}

	inline float Quaternion::operator()(const uint8 row, const uint8 column) const
	{
		if (row == 0)
		{
			if (column == 0)
				return 1.0f - 2.0f * (y * y + z * z);
			if (column == 1)
				return 2.0f * (x * y - z * w);
			if (column == 2)
				return 2.0f * (x * z + y * w);
		}
		if (row == 1)
		{
			if (column == 0)
				return 2.0f * (x * y + z * w);
			if (column == 1)
				return 1.0f - 2.0f * (x * x + z * z);
			if (column == 2)
				return 2.0f * (y * z - x * w);
		}
		if (row == 2)
		{
			if (column == 0)
				return 2.0f * (x * z - y * w);
			if (column == 1)
				return 2.0f * (y * z + x * w);
			if (column == 2)
				return 1.0f - 2.0f * (x * x + y * y);
		}

		LOG_ERROR("float Quaternion(" + std::to_string(row) + ',' + std::to_string(column) + ") no element corresponding")
			return 0;
	}

	inline Quaternion operator*(const float lhs, const Quaternion& rhs)
	{
		return rhs * lhs;
	}
}