namespace aw
{
	inline Vector3::Vector3(const float scale) :
		x{ scale },
		y{ scale },
		z{ scale }
	{
	}

	inline Vector3::Vector3(const float _x, const float _y, const float _z) :
		x{ _x },
		y{ _y },
		z{ _z }
	{
	}

	inline Vector3::Vector3(const Vector3& other) :
		x{ other.x.load() },
		y{ other.y.load() },
		z{ other.z.load() }
	{
	}

	inline Vector3::Vector3(Vector3&& other) :
		x{ std::move(other.x.load()) },
		y{ std::move(other.y.load()) },
		z{ std::move(other.z.load()) }
	{
	}

	inline Vector3 Vector3::GetNormalized() const
	{
		float n = Norm();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(n))
		{
			LOG_ERROR("Vector3::GetNormalized - norm null")
				return Zero();
		}
	#endif

		return Vector3(x / n, y / n, z / n);
	}

	inline Vector3 Vector3::GetClamped(const Vector3& vMin, const Vector3& vMax) const
	{
		return Vector3(
			aw::GetClamped(x, vMin.x, vMax.x),
			aw::GetClamped(y, vMin.y, vMax.y),
			aw::GetClamped(z, vMin.z, vMax.z)
		);
	}

	inline Vector3& Vector3::Normalize()
	{
		float n = Norm();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(n))
		{
			LOG_ERROR("Vector3::Normalize - norm null")
				return Invalidate();
		}
	#endif

		x = x / n;
		y = y / n;
		z = z / n;

		return *this;
	}

	inline Vector3& Vector3::Clamp(const Vector3& vMin, const Vector3& vMax)
	{
		x = aw::GetClamped(x, vMin.x, vMax.x);
		y = aw::GetClamped(y, vMin.y, vMax.y);
		z = aw::GetClamped(z, vMin.z, vMax.z);

		return *this;
	}

#if AWESOME_MATHS_SAFE
	inline Vector3& Vector3::Invalidate()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;

		return *this;
	}
#endif

	inline float Vector3::Norm() const
	{
		return sqrtf(SquaredNorm());
	}

	inline float Vector3::SquaredNorm() const
	{
		return SumSquare(x, y, z);
	}

	inline float Vector3::Dot(const Vector3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	inline float Vector3::PerpDot(const Vector3& other) const
	{
		return -y * other.x + x * other.y;
	}

	inline Vector3 Vector3::Cross(const Vector3& other) const
	{
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}

	inline Vector3 Vector3::DoubleCross(const Vector3& other) const
	{
		return Cross(other).Cross(*this);
	}

	inline std::string Vector3::ToString() const
	{
		return "[Vector3] x = " + std::to_string(x) + "\ty = " + std::to_string(y) + "\tz = " + std::to_string(z);
	}

	inline Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float percent)
	{
		aw::Clamp(percent, 0.0f, 1.0f);
		return LerpUnclamped(start, end, percent);
	}

	inline Vector3 Vector3::LerpUnclamped(const Vector3& start, const Vector3& end, float percent)
	{
		return (1.0f - percent) * start + percent * end;
	}

	inline Vector3 Vector3::Nlerp(const Vector3& start, const Vector3& end, float percent)
	{
		aw::Clamp(percent, 0.0f, 1.0f);
		return NlerpUnclamped(start, end, percent);
	}

	inline Vector3 Vector3::NlerpUnclamped(const Vector3& start, const Vector3& end, float percent)
	{
		return ((1.0f - percent) * start + percent * end).Normalize();
	}

	inline Vector3 Vector3::FromTo(const Vector3& from, const Vector3& to)
	{
		return to - from;
	}

	inline float Vector3::DistanceBetween(const Vector3& v1, const Vector3& v2)
	{
		return (v1 - v2).Norm();
	}

	inline const Vector3& Vector3::Zero()
	{
		static const Vector3 awVector3Zero;
		return awVector3Zero;
	}

	inline const Vector3& Vector3::One()
	{
		static const Vector3 awVector3One{ 1.0f, 1.0f, 1.0f };
		return awVector3One;
	}

	inline const Vector3& Vector3::Right()
	{
		static const Vector3 awVector3Right{ 1.0f, 0.0f, 0.0f };
		return awVector3Right;
	}

	inline const Vector3& Vector3::Left()
	{
		static const Vector3 awVector3Left{ -1.0f, 0.0f, 0.0f };
		return awVector3Left;
	}

	inline const Vector3& Vector3::Up()
	{
		static const Vector3 awVector3Up{ 0.0f, 1.0f, 0.0f };
		return awVector3Up;
	}

	inline const Vector3& Vector3::Down()
	{
		static const Vector3 awVector3Down{ 0.0f, -1.0f, 0.0f };
		return awVector3Down;
	}

	inline const Vector3& Vector3::Forward()
	{
		static const Vector3 awVector3Forward{ 0.0f, 0.0f, 1.0f };
		return awVector3Forward;
	}

	inline const Vector3& Vector3::Backward()
	{
		static const Vector3 awVector3Backward{ 0.0f, 0.0f, -1.0f };
		return awVector3Backward;
	}

	inline Vector3 Vector3::operator+() const
	{
		return *this;
	}

	inline Vector3 Vector3::operator-() const
	{
		return Vector3(-x, -y, -z);
	}

	inline Vector3 Vector3::operator*(const float rhs) const
	{
		return Vector3(x * rhs, y * rhs, z * rhs);
	}

	inline Vector3 Vector3::operator/(const float rhs) const
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
		{
			LOG_ERROR("Vector3::operator/ - divisor null");
				return Zero();
		}
	#endif

		return Vector3(x / rhs, y / rhs, z / rhs);
	}

	inline Vector3 Vector3::operator+(const Vector3& rhs) const
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline Vector3 Vector3::operator-(const Vector3& rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	inline float Vector3::operator|(const Vector3& rhs) const
	{
		return Dot(rhs);
	}

	inline Vector3 Vector3::operator^(const Vector3& rhs) const
	{
		return Cross(rhs);
	}

	inline Vector3& Vector3::operator=(const Vector3& rhs)
	{
		x = rhs.x.load();
		y = rhs.y.load();
		z = rhs.z.load();

		return *this;
	}

	inline Vector3& Vector3::operator=(Vector3&& rhs)
	{
		x = std::move(rhs.x.load());
		y = std::move(rhs.y.load());
		z = std::move(rhs.z.load());

		return *this;
	}

	inline Vector3& Vector3::operator*=(const float rhs)
	{
		x = x * rhs;
		y = y * rhs;
		z = z * rhs;

		return *this;
	}

	inline Vector3& Vector3::operator/=(const float rhs)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
		{
			LOG_ERROR("Vector3::operator/= - divisor null");
				return Invalidate();
		}
	#endif

		x = x / rhs;
		y = y / rhs;
		z = z / rhs;

		return *this;
	}

	inline Vector3& Vector3::operator+=(const Vector3& rhs)
	{
		x = x + rhs.x;
		y = y + rhs.y;
		z = z + rhs.z;

		return *this;
	}

	inline Vector3& Vector3::operator-=(const Vector3& rhs)
	{
		x = x - rhs.x;
		y = y - rhs.y;
		z = z - rhs.z;

		return *this;
	}

	inline bool Vector3::operator==(const Vector3& rhs) const
	{
		return NearlyEqual(x, rhs.x) && NearlyEqual(y, rhs.y) && NearlyEqual(z, rhs.z);
	}

	inline bool Vector3::operator!=(const Vector3& rhs) const
	{
		return !(*this == rhs);
	}

	inline Vector3 operator*(const float lhs, const Vector3& rhs)
	{
		return rhs * lhs;
	}
}