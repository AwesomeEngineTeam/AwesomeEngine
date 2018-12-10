#pragma once

#ifndef AWESOME_CORE_TOOLS_GUARD
#define AWESOME_CORE_TOOLS_GUARD

#include <Awesome/Core/Types.hpp>

namespace aw
{
	template<typename T> constexpr T Sum(T v);
	template<typename T, typename... Args> constexpr T Sum(T v, Args... args);

	template<typename Out, typename In> constexpr Out Assemble(const In& lhs, const In& rhs);
	template<typename Out, typename In> constexpr Out DisassembleLHS(const In& assembled);
	template<typename Out, typename In> constexpr Out DisassembleRHS(const In& assembled);
	template<typename Out, typename In> constexpr void Disassemble(const In& assembled, Out* lhs, Out* rhs);

	template<typename T, T... args>
	constexpr bool PackContains(const T& i);

	template<typename T, T... args>
	constexpr T SumParamsUntilEquals(const T& i);
}

#include <Awesome/Core/Tools.inl>

#endif //GUARD