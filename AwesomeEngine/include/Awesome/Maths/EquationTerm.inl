namespace aw
{
	inline EquationTerm::EquationTerm(const float _coefficient, const char* varName) :
		coefficient { _coefficient },
		variable{ varName }
	{
	}

	inline EquationTerm::EquationTerm(const EquationTerm& other) :
		coefficient{ other.coefficient },
		variable{ other.variable }
	{
	}

	inline EquationTerm::EquationTerm(EquationTerm&& other) :
		coefficient{ std::move(other.coefficient) },
		variable{ std::move(other.variable) }
	{
	}

#if AWESOME_MATHS_SAFE
	inline void EquationTerm::Invalidate()
	{
		variable.Invalidate();
		coefficient = nanf(0);
	}
#endif

	inline EquationTerm EquationTerm::operator+()
	{
		return *this;
	}

	inline EquationTerm EquationTerm::operator-()
	{
		EquationTerm opp = *this;
		opp.coefficient = -opp.coefficient;

		return opp;
	}

	inline EquationTerm EquationTerm::operator*(const float& rhs)
	{
		EquationTerm res = *this;
		res.coefficient *= rhs;

		return res;
	}

	inline EquationTerm EquationTerm::operator/(const float& rhs)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(rhs))
			LOG_ERROR("EquationTerm::operator/ - divisor null");
	#endif

		EquationTerm res = *this;
		res.coefficient /= rhs;

		return res;
	}

	inline std::string EquationTerm::ToString()
	{
		if (!variable.IsKnown())
			return "Variable is unknown";

		return NearlyEqual(coefficient, 1.0f) ? variable.GetName() + " = " + std::to_string(variable) :
			std::to_string(coefficient) + " x " + variable.GetName() + " = " + std::to_string(variable);
	}

	inline EquationTerm& EquationTerm::operator=(const EquationTerm& rhs)
	{
		coefficient = rhs.coefficient;
		variable = rhs.variable;

		return *this;
	}

	inline EquationTerm& EquationTerm::operator=(EquationTerm&& rhs)
	{
		coefficient = std::move(rhs.coefficient);
		variable = std::move(rhs.variable);

		return *this;
	}

	inline EquationTerm& EquationTerm::operator=(const float rhs)
	{
	#if AWESOME_MATHS_SAFE
		if (NearlyZero(coefficient))
			LOG_ERROR("EquationTerm::operator= - coefficient null");
	#endif

		variable = rhs / coefficient;
		coefficient /= coefficient;

		return *this;
	}
}