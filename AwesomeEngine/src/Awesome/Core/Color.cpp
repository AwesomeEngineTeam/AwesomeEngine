#include <Awesome/Core/Color.hpp>

namespace aw
{
	Color3f::Color3f(const Color4f& other) :
		red{ other.red },
		green{ other.green },
		blue{ other.blue }
	{
	}

	const Color3f& Color3f::Black()
	{
		static Color3f color{ 0.0f, 0.0f, 0.0f };
		return color;
	}

	const Color3f& Color3f::White()
	{
		static Color3f color{ 1.0f, 1.0f, 1.0f };
		return color;
	}

	const Color3f& Color3f::Red()
	{
		static Color3f color{ 1.0f, 0.0f, 0.0f };
		return color;
	}

	const Color3f& Color3f::Green()
	{
		static Color3f color{ 0.0f, 1.0f, 0.0f };
		return color;
	}

	const Color3f& Color3f::Blue()
	{
		static Color3f color{ 0.0f, 0.0f, 1.0f };
		return color;
	}

	const Color3f& Color3f::Yellow()
	{
		static Color3f color{ 1.0f, 1.0f, 0.0f };
		return color;
	}

	const Color3f& Color3f::Magenta()
	{
		static Color3f color{ 1.0f, 0.0f, 1.0f };
		return color;
	}

	const Color3f& Color3f::Cyan()
	{
		static Color3f color{ 0.0f, 1.0f, 1.0f };
		return color;
	}

	const Color3f& Color3f::NiceYellow()
	{
		static Color3f color{ 1.0f, 0.92f, 0.016f };
		return color;
	}

	const Color3f& Color3f::DarkSublime()
	{
		static Color3f color{ 0.153f, 0.157f, 0.133f };
		return color;
	}

	Color3f& Color3f::operator=(const Color4f& color)
	{
		red = color.red;
		green = color.green;
		blue = color.blue;

		return *this;
	}




	Color4f::Color4f(const Color3f& other) :
		red{ other.red },
		green{ other.green },
		blue{ other.blue },
		alpha{ 1.0f }
	{
	}

	const Color4f& Color4f::Black()
	{
		static Color4f color{ 0.0f, 0.0f, 0.0f, 1.0f };
		return color;
	}

	const Color4f& Color4f::BlackTransparent()
	{
		static Color4f color{ 0.0f, 0.0f, 0.0f, 0.0f };
		return color;
	}

	const Color4f& Color4f::White()
	{
		static Color4f color{ 1.0f, 1.0f, 1.0f, 1.0f };
		return color;
	}

	const Color4f& Color4f::WhiteTransparent()
	{
		static Color4f color{ 1.0f, 1.0f, 1.0f, 0.0f };
		return color;
	}

	const Color4f& Color4f::Red()
	{
		static Color4f color{ 1.0f, 0.0f, 0.0f, 1.0f };
		return color;
	}

	const Color4f& Color4f::Green()
	{
		static Color4f color{ 0.0f, 1.0f, 0.0f, 1.0f };
		return color;
	}

	const Color4f& Color4f::Blue()
	{
		static Color4f color{ 0.0f, 0.0f, 1.0f, 1.0f };
		return color;
	}

	const Color4f& Color4f::Yellow()
	{
		static Color4f color{ 1.0f, 1.0f, 0.0f, 1.0f };
		return color;
	}

	const Color4f& Color4f::Magenta()
	{
		static Color4f color{ 1.0f, 0.0f, 1.0f, 1.0f };
		return color;
	}

	const Color4f& Color4f::Cyan()
	{
		static Color4f color{ 0.0f, 1.0f, 1.0f, 1.0f };
		return color;
	}

	const Color4f& Color4f::NiceYellow()
	{
		static Color4f color{ 1.0f, 0.92f, 0.016f, 1.0f };
		return color;
	}

	const Color4f& Color4f::DarkSublime()
	{
		static Color4f color{ 0.153f, 0.157f, 0.133f, 1.0f };
		return color;
	}

	Color4f& Color4f::operator=(const Color3f& color)
	{
		red = color.red;
		green = color.green;
		blue = color.blue;

		return *this;
	}
}