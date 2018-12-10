#include <Awesome/SDK/Resources/FontLoader.hpp>
#include <Awesome/SDK/Resources/ResourceManager.hpp>
#include <Awesome/Renderer/Resource/TexelBuffer.hpp>

#include <Awesome/Core/Types.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#include "stb_truetype.h"

namespace aw
{
	namespace sdk
	{
		static uint8 ttf_buffer[1<<25];

		FontAtlas FontLoader::Load(ResourceManager& resourceManager, const std::string& filename, float pixelSize)
		{
			std::memset(ttf_buffer, 0, sizeof ttf_buffer);
			auto file = fopen(filename.c_str(), "rb");
			if (file == nullptr)
			{
				LOG_ERROR("Font not found");

				file = fopen("c:/windows/fonts/arial.ttf", "rb");
			}
			fread(ttf_buffer, 1, 1<<25, file);

			// stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer, 0));
			// bitmap = stbtt_GetCodepointBitmap(&font, 0, stbtt_ScaleForPixelHeight(&font, s), c, &w, &h, 0,0);

			FontAtlas fontAtlas;

			fontAtlas.bakedFont.resize(96);
			fontAtlas.packedFont.resize(6 * 128);
			//fontAtlas.packedFont.resize(96);

			stbtt_pack_context pc{};

			TexelBuffer& texels = fontAtlas.atlas;
			texels.width = 512;
			texels.height = 512;
			texels.channels = 4;
			texels.texels.resize(texels.width * texels.height * texels.channels, 0);

			std::vector<uint8> pixels(texels.width * texels.height);

			float scale[2] = { 24.0f, 14.0f };

			//stbtt_packedchar chardata[6][128];

			stbtt_PackBegin(&pc, pixels.data(), texels.width, texels.height, 0, 1, NULL);
			stbtt_PackSetOversampling(&pc, 1, 1);
			stbtt_PackFontRange(&pc, ttf_buffer, 0, pixelSize, 32, 95, fontAtlas.chardata[0] + 32);
			//stbtt_PackFontRange(&pc, ttf_buffer, 0, pixelSize, 32, fontAtlas.packedFont.size(), fontAtlas.packedFont.data());
			//for (uint8 i{ 0 }; i < 2; i++)
			//{
				//stbtt_PackSetOversampling(&pc, 1, 1);
				//stbtt_PackFontRange(&pc, ttf_buffer, 0, scale[i], 32, 95, &fontAtlas.packedFont[(i * 3 + 0) * 128] + 32);
				//stbtt_PackSetOversampling(&pc, 2, 2);
				//stbtt_PackFontRange(&pc, ttf_buffer, 0, scale[i], 32, 95, &fontAtlas.packedFont[(i * 3 + 1) * 128] + 32);
				//stbtt_PackSetOversampling(&pc, 3, 1);
				//stbtt_PackFontRange(&pc, ttf_buffer, 0, scale[i], 32, 95, &fontAtlas.packedFont[(i * 3 + 2) * 128] + 32);
			//	stbtt_PackSetOversampling(&pc, 1, 1);
			//	stbtt_PackFontRange(&pc, ttf_buffer, 0, scale[i], 32, 95, fontAtlas.chardata[i * 3 + 0] + 32);
			//	stbtt_PackSetOversampling(&pc, 2, 2);
			//	stbtt_PackFontRange(&pc, ttf_buffer, 0, scale[i], 32, 95, fontAtlas.chardata[i * 3 + 1] + 32);
			//	stbtt_PackSetOversampling(&pc, 3, 1);
			//	stbtt_PackFontRange(&pc, ttf_buffer, 0, scale[i], 32, 95, fontAtlas.chardata[i * 3 + 2] + 32);
			//}
			stbtt_PackEnd(&pc);

			for (uint64 i{ 0 }; i < texels.width * texels.height; i++)
			{
				texels.texels[i * 4 + 0] = 255;
				texels.texels[i * 4 + 1] = 255;
				texels.texels[i * 4 + 2] = 255;
				texels.texels[i * 4 + 3] = pixels[i];
			}

			//int res = stbtt_BakeFontBitmap(
			//	ttf_buffer,
			//	0,
			//	pixelSize,
			//	texels.texels.data(),
			//	texels.width,
			//	texels.height,
			//	32,
			//	fontAtlas.bakedFont.size(),
			//	fontAtlas.bakedFont.data()
			//);

			//float xpos = 0;
			//float ypos = 0;

			//stbtt_aligned_quad q{};

			//stbtt_GetBakedQuad(
			//	cdata,
			//	texels.width,
			//	texels.height,
			//	'a' - 'a',
			//	&xpos,
			//	&ypos,
			//	&q,      // output: quad to draw
			//	1
			//);

			/*ImFontAtlas fontAtlas{};

			fontAtlas.AddFontFromFileTTF(filename.c_str(), pixelSize);

			uint8* texels;

			TexelBuffer texelBuffer;
			fontAtlas.GetTexDataAsRGBA32(&texels, reinterpret_cast<int*>(&texelBuffer.width), reinterpret_cast<int*>(&texelBuffer.height));
			texelBuffer.channels = 4;

			texelBuffer.texels.resize(texelBuffer.width * texelBuffer.height * texelBuffer.channels);
			memcpy(texelBuffer.texels.data(), texels, texelBuffer.width * texelBuffer.height * texelBuffer.channels);

			vk::CommandBufferAllocateInfo cmdAllocInfo(
				Context::GetCommandPool(),
				vk::CommandBufferLevel::ePrimary,
				1
			);

			SampledTexture font{};

			vk::UniqueCommandBuffer copyCmdBuffer = std::move(Context::GetDevice().Get().allocateCommandBuffersUnique(cmdAllocInfo).back());
			font.Create(vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm, texelBuffer, copyCmdBuffer.get(), vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst, vk::ImageAspectFlagBits::eColor);

			SampledTextureHandle fontHandle = resourceManager.Register(filename + ":" + std::to_string(pixelSize), std::move(font));

			auto fontImage = fontHandle.GetImage();
			//std::memcpy(fontAtlas.TexID, fontImage, sizeof(void*));
	//		fontAtlas.TexID = u.voidPtr;
			fontAtlas.TexID = reinterpret_cast<void*&>(fontImage);

			return fontAtlas;*/

			return fontAtlas;
		}
	}
}