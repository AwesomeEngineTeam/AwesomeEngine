#pragma once

#ifndef AWESOME_RENDERER_FONT_GUARD
#define AWESOME_RENDERER_FONT_GUARD

#include <Awesome/Renderer/Config.hpp>

#include <Awesome/Core/Types.hpp>

#include <stb_truetype.h>

namespace aw
{
	class Font
	{
		stbtt_fontinfo m_info;

		// uint64 width{ 0 };
		// uint64 height{ 0 };
		// std::vector<uint8> texels{};
		// uint64 channels{ 0 };

	public:
		Font() = default;
		Font(const Font&) = default;
		inline Font(Font&&) noexcept;
		~Font() = default;

		AWESOME_RENDERER_API void Clear();

		AWESOME_RENDERER_API Font& operator+=(const Font& other);

		inline bool IsCompatible(const Font& other) const;

		Font& operator=(const Font&) = default;
		inline Font& operator=(Font&&) noexcept;
	};
}

#include <Awesome/Renderer/Resource/Font.inl>

#endif //GUARD