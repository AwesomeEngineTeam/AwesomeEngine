namespace aw
{
	inline Vector2::Vector2(float scale) :
		x{ scale },
		y{ scale }
	{
	}

	inline Vector2::Vector2(float _x, float _y) :
		x{ _x },
		y{ _y }
	{
	}

	inline Vector2::Vector2(const Vector2& other) :
		x{ other.x },
		y{ other.y }
	{
	}

	inline Vector2::Vector2(Vector2&& other) noexcept :
		x{ std::move(other.x) },
		y{ std::move(other.y) }
	{
	}

	inline Vector2 Vector2::GetNormalized() const
	{
		float n = Norm();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(n))
		{
			LOG_ERROR("Vector2::GetNormalized - norm null");
			return Vector2();
		}
	#endif

		return Vector2(x / n, y / n);
	}

	inline Vector2 Vector2::GetClamped(const Vector2& vMin, const Vector2& vMax) const
	{
		return Vector2(
			aw::GetClamped(x, vMin.x, vMax.x), 
			aw::GetClamped(y, vMin.y, vMax.y)
		);
	}

	inline Vector2& Vector2::Normalize()
	{
		float n = Norm();

	#if AWESOME_MATHS_SAFE
		if (NearlyZero(n))
		{
			LOG_ERROR("Vector2::Normalize - norm null");
			return *this;
		}
	#endif

		x /= n;
		y /= n;

		return *this;
	}

	inline Vector2& Vector2::Clamp(const Vector2& vMin, const Vector2& vMax)
	{
		aw::Clamp(x, vMin.x, vMax.x);
		aw::Clamp(y, vMin.y, vMax.y);

		return *this;
	}

#if AWESOME_MATHS_SAFE
	inline Vector2& Vector2::Invalidate()
	{
		x = 0.0f;
		y = 0.0f;

		return *this;
	}
#endif

	inline float Vector2::Norm() const
	{
		return sqrtf(SquaredNorm());
	}

	inline float Vector2::SquaredNorm() const
	{
		return SumSquare(x, y);
	}

	inline float Vector2::Dot(const Vector2& other) const
	{
		return x * other.x + y * other.y;
	}

	inline std::string Vector2::ToString() const
	{
		return "[Vector2] x = " + std::to_string(x) + "\ty = " + std::to_string(y);
	}

	inline Vector2 Vector2::Lerp(const Vector2& start, const Vector2& end, float percent)
	{
		aw::Clamp(percent, 0.0f, 1.0f);
		return LerpUnclamped(start, end, percent);
	}

	inline Vector2 Vector2::LerpUnclamped(const Vector2& start, const Vector2& end, float percent)
	{
		return (end - start) * percent + start;
	}

	inline Vector2 Vector2::Nlerp(const Vector2& start, const Vector2& end, float percent)
	{
		aw::Clamp(percent, 0.0f, 1.0f);
		return NlerpUnclamped(start, end, percent);
	}

	inline Vector2 Vector2::NlerpUnclamped(const Vector2& start, const Vector2& end, float percent)
	{
		return LerpUnclamped(start, end, percent).Normalize();
	}

	inline Vector2 Vector2::Slerp(const Vector2& start, const Vector2& end, float percent)
	{
		aw::Clamp(percent, 0.0f, 1.0f);
		return SlerpUnclamped(start, end, percent);
	}

	inline Vector2 Vector2::SlerpUnclamped(const Vector2& start, const Vector2& end, float percent)
	{
		float angle = Vector2::AngleBetween(start, end) * percent;

		return Vector2(cosf(angle) * start.x - sinf(angle) * start.y, sinf(angle) * start.x + cosf(angle) * start.y).Normalize() *
			aw::Lerp(start.Norm(), end.Norm(), percent);
	}

	inline float Vector2::DistanceBetween(const Vector2& v1, const Vector2& v2)
	{
		return (v1 - v2).Norm();
	}

	inline float Vector2::AngleBetween(const Vector2& v1, const Vector2& v2)
	{
		return atan2f(v2.y, v2.x) - atan2f(v1.y, v1.x);
	}

	inline const Vector2& Vector2::Zero()
	{
		static const Vector2 awVector2Zero{};
		return awVector2Zero;
	}

	inline const Vector2& Vector2::One()
	{
		static const Vector2 awVector2One{ 1.0f, 1.0f };
		return awVector2One;
	}

	inline const Vector2& Vector2::Right()
	{
		static const Vector2 awVector2Right{ 1.0f, 0.0f };
		return awVector2Right;
	}

	inline const Vector2& Vector2::Left()
	{
		static const Vector2 awVector2Left{ -1.0f, 0.0f };
		return awVector2Left;
	}

	inline const Vector2& Vector2::Up()
	{
		static const Vector2 awVector2Up{ 0.0f, 1.0f };
		return awVector2Up;
	}

	inline const Vector2& Vector2::Down()
	{
		static const Vector2 awVector2Down{ 0.0f, -1.0f };
		return awVector2Down;
	}

	inline Vector2 Vector2::operator+() const
	{
		return *this;
	}

	inline Vector2 Vector2::operator-() const
	{
		return Vector2(-x, -y);
	}

	inline Vector2 Vector2::operator*(const float rhs) const
	{
		return Vector2(x * rhs, y * rhs);
	}

	inline Vector2 Vector2::operator/(const float rhs) const
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
		{
			LOG_ERROR("Vector2::operaotor/ - divisor null");
			return Zero();
		}
	#endif

		return Vector2(x / rhs, y / rhs);
	}

	inline Vector2 Vector2::operator+(const Vector2& rhs) const
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	inline Vector2 Vector2::operator-(const Vector2& rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	inline float Vector2::operator|(const Vector2& rhs) const
	{
		return Dot(rhs);
	}

	inline Vector2& Vector2::operator=(const Vector2& rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	inline Vector2& Vector2::operator=(Vector2&& rhs)
	{
		x = std::move(rhs.x);
		y = std::move(rhs.y);

		return *this;
	}

	inline Vector2& Vector2::operator*=(const float rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	inline Vector2& Vector2::operator/=(const float rhs)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
		{
			LOG_ERROR("Vector2::operaotor/= - divisor null");
			return Invalidate();
		}
	#endif

		x /= rhs;
		y /= rhs;

		return *this;
	}

	inline Vector2& Vector2::operator+=(const Vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	inline Vector2& Vector2::operator-=(const Vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	inline bool Vector2::operator==(const Vector2& rhs) const
	{
		return NearlyEqual(x, rhs.x) && NearlyEqual(y, rhs.y);
	}

	inline bool Vector2::operator!=(const Vector2& rhs) const
	{
		return !(this->operator==(rhs));
	}

	inline Vector2 operator*(const float lhs, const Vector2& rhs)
	{
		return rhs * lhs;
	}
}