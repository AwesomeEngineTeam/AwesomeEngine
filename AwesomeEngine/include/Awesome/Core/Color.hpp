#pragma once

#ifndef AWESOME_CORE_COLOR_GUARD
#define AWESOME_CORE_COLOR_GUARD

#include <Awesome/Core/Config.hpp>
#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Reflexion/Reflect.hpp>

#include <array>

namespace aw
{
	struct Color3f;
	struct Color4f;

	struct Color3f
	{
		float red{ 0.0f };
		float green{ 0.0f };
		float blue{ 0.0f };

		Color3f() = default;
		inline Color3f(float r, float g, float b);
		inline Color3f(const std::array<float, 3>&);
		inline Color3f(const float[3]);
		Color3f(Color3f&&) = default;
		Color3f(const Color3f&) = default;
		AWESOME_CORE_API explicit Color3f(const Color4f&);
		~Color3f() = default;

		AWESOME_CORE_API static const Color3f& Black();
		AWESOME_CORE_API static const Color3f& White();

		AWESOME_CORE_API static const Color3f& Red();
		AWESOME_CORE_API static const Color3f& Green();
		AWESOME_CORE_API static const Color3f& Blue();

		AWESOME_CORE_API static const Color3f& Yellow();
		AWESOME_CORE_API static const Color3f& Magenta();
		AWESOME_CORE_API static const Color3f& Cyan();

		AWESOME_CORE_API static const Color3f& NiceYellow();
		AWESOME_CORE_API static const Color3f& DarkSublime();

		Color3f& operator=(Color3f&&) = default;
		Color3f& operator=(const Color3f&) = default;
		AWESOME_CORE_API Color3f& operator=(const Color4f&);

		REFLECT_BEGIN(Color3f)
			REFLECT_MEMBER(red)
			REFLECT_MEMBER(green)
			REFLECT_MEMBER(blue)
		REFLECT_END()
	};

	struct Color4f
	{
		float red{ 0.0f };
		float green{ 0.0f };
		float blue{ 0.0f };
		float alpha{ 0.0f };

		Color4f() = default;
		inline Color4f(float r, float g, float b, float a);
		inline Color4f(const std::array<float, 4>&);
		inline Color4f(const float[4]);
		Color4f(Color4f&&) = default;
		Color4f(const Color4f&) = default;
		AWESOME_CORE_API explicit Color4f(const Color3f&);
		~Color4f() = default;

		AWESOME_CORE_API static const Color4f& Black();
		AWESOME_CORE_API static const Color4f& BlackTransparent();
		AWESOME_CORE_API static const Color4f& White();
		AWESOME_CORE_API static const Color4f& WhiteTransparent();

		AWESOME_CORE_API static const Color4f& Red();
		AWESOME_CORE_API static const Color4f& Green();
		AWESOME_CORE_API static const Color4f& Blue();

		AWESOME_CORE_API static const Color4f& Yellow();
		AWESOME_CORE_API static const Color4f& Magenta();
		AWESOME_CORE_API static const Color4f& Cyan();

		AWESOME_CORE_API static const Color4f& NiceYellow();
		AWESOME_CORE_API static const Color4f& DarkSublime();

		inline operator const std::array<float, 4>&() const;

		Color4f& operator=(Color4f&&) = default;
		Color4f& operator=(const Color4f&) = default;
		AWESOME_CORE_API Color4f& operator=(const Color3f&);
	};
}

#include <Awesome/Core/Color.inl>

#endif //guard