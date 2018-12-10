#pragma once

#ifndef AWESOME_MATHS_EQUATIONTERM_GUARD
#define AWESOME_MATHS_EQUATIONTERM_GUARD

#include <Awesome/Maths/Variable.hpp>

namespace aw
{
	class EquationTerm
	{
	public:
		float coefficient;
		Variable variable;

		EquationTerm() = delete;
		inline EquationTerm(const float _coefficient, const char* varName);
		inline EquationTerm(const EquationTerm& other);
		inline EquationTerm(EquationTerm&& other);
		~EquationTerm() = default;

	#if AWESOME_MATHS_SAFE
		inline void Invalidate();
	#endif

		inline std::string ToString();

		inline EquationTerm operator+();
		inline EquationTerm operator-();
		inline EquationTerm operator*(const float& rhs);
		inline EquationTerm operator/(const float& rhs);

		inline EquationTerm& operator=(const EquationTerm& rhs);
		inline EquationTerm& operator=(EquationTerm&& rhs);
		inline EquationTerm& operator=(const float rhs);
	};

	inline EquationTerm operator*(const float& lhs, const EquationTerm& rhs);
}

#include <Awesome/Maths/EquationTerm.inl>

#endif //GUARD