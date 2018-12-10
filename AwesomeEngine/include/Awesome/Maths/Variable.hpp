#pragma once

#ifndef AWESOME_MATHS_VARIABLE_GUARD
#define AWESOME_MATHS_VARIABLE_GUARD

#include <Awesome/Core/Debug.hpp>

#include <Awesome/Maths/Config.hpp>
#include <Awesome/Maths/Algorithm.hpp>

namespace aw
{
	class Variable
	{
		std::string name;
		float value;
		bool isValide;

	public:
		Variable() = delete;
		inline Variable(const char* _name);
		inline Variable(const Variable& other);
		inline Variable(Variable&& other);
		inline ~Variable() = default;

		inline std::string GetName() const;

	#if AWESOME_MATHS_SAFE
		inline void Invalidate();
	#endif

		inline bool IsKnown() const;

		inline std::string ToString();

		inline Variable operator+() const;
		inline Variable operator-() const;
		inline float operator+(const float rhs) const;
		inline float operator-(const float rhs) const;
		inline float operator*(const float rhs) const;
		inline float operator/(const float rhs) const;
		inline float operator+(const Variable& rhs) const;
		inline float operator-(const Variable& rhs) const;
		inline float operator*(const Variable& rhs) const;
		inline float operator/(const Variable& rhs) const;

		inline Variable& operator=(const Variable& rhs);
		inline Variable& operator=(Variable&& rhs);
		inline Variable& operator=(const float rhs);

		inline operator float() const;
	};

	inline float operator+(const float lhs, const Variable& rhs);
	inline float operator-(const float lhs, const Variable& rhs);
	inline float operator*(const float lhs, const Variable& rhs);
	inline float operator/(const float lhs, const Variable& rhs);

#define VARIABLE(x) Variable x(#x)
}

#include <Awesome/Maths/Variable.inl>

#endif //GUARD