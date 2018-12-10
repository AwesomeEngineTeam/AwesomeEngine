#pragma once

#ifndef AWESOME_RENDERER_FONT_ATLAS_GUARD
#define AWESOME_RENDERER_FONT_ATLAS_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Resource/TexelBuffer.hpp>

#include <Awesome/Core/Types.hpp>

#include <stb_truetype.h>

#include <vector>

namespace aw
{
	struct FontAtlas
	{
		TexelBuffer atlas;
		std::vector<stbtt_bakedchar> bakedFont;
		std::vector<stbtt_packedchar> packedFont;

		stbtt_packedchar chardata[6][128];
		// uint64 width{ 0 };
		// uint64 height{ 0 };
		// std::vector<uint8> texels{};
		// uint64 channels{ 0 };

		FontAtlas() = default;
		FontAtlas(const FontAtlas&) = default;
		FontAtlas(FontAtlas&&) = default;
		~FontAtlas() = default;

		FontAtlas& operator=(const FontAtlas&) = default;
		FontAtlas& operator=(FontAtlas&&) = default;
	};
}

#include <Awesome/Renderer/Resource/FontAtlas.inl>

#endif //GUARD