#pragma once

#ifndef AWESOME_MATHS_LINEAREQUATION_GUARD
#define AWESOME_MATHS_LINEAREQUATION_GUARD

//#include <string>

#include <Awesome/Maths/EquationTerm.hpp>

namespace aw
{
	class LinearEquation
	{
	public:
		std::vector<EquationTerm> leftTerms;
		std::vector<EquationTerm> rightTerms;

		LinearEquation() = delete;
		inline LinearEquation(std::string str);
		inline LinearEquation(const LinearEquation& other) = delete;
		inline LinearEquation(LinearEquation&& other) = delete;
		~LinearEquation() = default;

		void Solve();

		std::string ToString();

		inline LinearEquation& operator=(const LinearEquation& rhs) = delete;
		inline LinearEquation& operator=(LinearEquation&& rhs) = delete;
	};
}

#include <Awesome/Maths/LinearEquation.inl>

#endif //GUARD