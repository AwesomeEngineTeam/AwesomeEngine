#pragma once

#ifndef AWESOME_WINDOWING_ENUMS_GUARD
#define AWESOME_WINDOWING_ENUMS_GUARD

#include <Awesome/Core/Types.hpp>

namespace aw
{
	enum class WindowStyleBit : uint8
	{
		None = 0x0,
		Fullscreen = 0x1,
		Titlebar = 0x2,
		Resizable = 0x4,
		Closable = 0x8,
		Threaded = 0x10,
		Default = Titlebar | Resizable | Closable
	};

	constexpr WindowStyleBit operator|(WindowStyleBit lhs, WindowStyleBit rhs)
	{
		return WindowStyleBit(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
	}

	constexpr WindowStyleBit operator&(WindowStyleBit lhs, WindowStyleBit rhs)
	{
		return WindowStyleBit(static_cast<uint32>(lhs) & static_cast<uint32>(rhs));
	}
}

#endif