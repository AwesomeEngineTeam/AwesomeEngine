namespace aw
{
	inline Variable::Variable(const char* _name) :
		name{ std::string(_name) },
		value{ nanf(0) },
		isValide{ true }
	{
	}

	inline Variable::Variable(const Variable& other) :
		name{ other.name },
		value{ other.value },
		isValide{ other.isValide }
	{
	}

	inline Variable::Variable(Variable&& other) :
		name{ std::move(other.name) },
		value{ std::move(other.value) },
		isValide{ std::move(other.isValide) }
	{
	}

	inline std::string Variable::GetName() const
	{
		return name;
	}

#if AWESOME_MATHS_SAFE
	inline void Variable::Invalidate()
	{
		value = nanf(0);
		isValide =false;
	}
#endif

	inline bool Variable::IsKnown() const
	{
		return !isnan(value);
	}

	inline std::string aw::Variable::ToString()
	{
		if (!isValide)
			return name + " is invalide";

		if (isnan(value))
			return name + " is unknown";

		return name + " = " + std::to_string(value);
	}

	inline Variable Variable::operator+() const
	{
		return *this;
	}

	inline Variable Variable::operator-() const
	{
		Variable opp = *this;
		opp.value = -opp.value;

		return opp;
	}

	inline float Variable::operator+(const float rhs) const
	{
		return value + rhs;
	}

	inline float Variable::operator-(const float rhs) const
	{
		return value - rhs;
	}

	inline float Variable::operator*(const float rhs) const
	{
		return value * rhs;
	}

	inline float Variable::operator/(const float rhs) const
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
			LOG_ERROR("Variable::operator/ - divisor null")
	#endif

		return value / rhs;
	}

	inline float Variable::operator+(const Variable& rhs) const
	{
		return value + rhs;
	}

	inline float Variable::operator-(const Variable& rhs) const
	{
		return value - rhs;
	}

	inline float Variable::operator*(const Variable& rhs) const
	{
		return value * rhs;
	}

	inline float Variable::operator/(const Variable& rhs) const
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
			LOG_ERROR("Variable::operator/ - divisor null")
		#endif

		return value / rhs;
	}

	inline Variable& Variable::operator=(const Variable& rhs)
	{
		name = rhs.name;
		value = rhs.value;
		isValide = rhs.isValide;

		return *this;
	}

	inline Variable& Variable::operator=(Variable&& rhs)
	{
		name = std::move(rhs.name);
		value = std::move(rhs.value);
		isValide = std::move(rhs.isValide);

		return *this;
	}

	inline Variable& Variable::operator=(const float rhs)
	{
		value = rhs;

		return *this;
	}

	inline Variable::operator float() const
	{
		return value;
	}

	float operator+(const float lhs, const Variable& rhs)
	{
		return rhs + lhs;
	}

	float operator-(const float lhs, const Variable& rhs)
	{
		return rhs - lhs;
	}

	float operator*(const float lhs, const Variable& rhs)
	{
		return rhs * lhs;
	}

	float operator/(const float lhs, const Variable& rhs)
	{
		return rhs / lhs;
	}
}