#include <Awesome/SDK/Resources/TextureLoader.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace aw
{
	namespace sdk
	{
		TexelBuffer TextureLoader::Load(const std::string& filename, TexelBufferLoadComponent loadComponent, TexelBufferLoadParamsBit params)
		{
			int32 width, height, channels;
			//stbi_info(filename, &width, &height, &channels);
			// TODO use true channels
			uint8* rawTexels = stbi_load(filename.c_str(), &width, &height, &channels, static_cast<int>(loadComponent));

			channels = static_cast<int>(loadComponent); // TODO use true channels

			if (rawTexels == nullptr)
			{
				LOG_ERROR("Failed to load image %s", filename.c_str());

				TexelBuffer texelBuffer;
				texelBuffer.width = 1;
				texelBuffer.height = 1;
				texelBuffer.channels = 4;

				texelBuffer.texels.resize(4);
				texelBuffer.texels[0] = 0;
				texelBuffer.texels[1] = 0;
				texelBuffer.texels[2] = 0;
				texelBuffer.texels[3] = 0;

				return texelBuffer;
			}

			if (params != TexelBufferLoadParamsBit::None)
			{
				struct Texel
				{
					uint8 r, g, b, a;

					void Swizzle(TexelBufferLoadParamsBit params)
					{
						int invert;

						invert = (params & TexelBufferLoadParamsBit::InvertR) != TexelBufferLoadParamsBit::None;
						r = (invert * (~r)) + ((1 - invert) * r);

						invert = (params & TexelBufferLoadParamsBit::InvertG) != TexelBufferLoadParamsBit::None;
						g = (invert * (~g)) + ((1 - invert) * g);

						invert = (params & TexelBufferLoadParamsBit::InvertB) != TexelBufferLoadParamsBit::None;
						b = (invert * (~b)) + ((1 - invert) * b);

						invert = (params & TexelBufferLoadParamsBit::InvertA) != TexelBufferLoadParamsBit::None;
						a = (invert * (~a)) + ((1 - invert) * a);
					}
				};

				auto texels = reinterpret_cast<Texel*>(rawTexels);

				for (int32 i = 0; i < width * height; i++)
				{
					texels[i].Swizzle(params);
				}
			}

			TexelBuffer texelBuffer;
			texelBuffer.width = width;
			texelBuffer.height = height;
			texelBuffer.channels = channels;
			texelBuffer.texels.resize(texelBuffer.width * texelBuffer.height * texelBuffer.channels);

			memcpy(texelBuffer.texels.data(), rawTexels, texelBuffer.width * texelBuffer.height * texelBuffer.channels);

			STBI_FREE(rawTexels);

			return texelBuffer;
		}

		TexelBuffer TextureLoader::LoadSkybox(const std::string& skyboxDirectory, const std::string& fileExtention)
		{
			TexelBuffer texelBuffer;
			int32 width, height, channels;

			auto reset = [&]()
			{
				texelBuffer.width = 1;
				texelBuffer.height = 1;
				texelBuffer.channels = 4;
				texelBuffer.layerCount = 6;

				texelBuffer.texels.resize(texelBuffer.channels * texelBuffer.layerCount, 0);
			};

			if (stbi_info((skyboxDirectory + "/posx" + fileExtention).c_str(), &width, &height, &channels) == 0)
			{
				LOG_ERROR("Failed to get skybox infos");
				reset();
				return texelBuffer;
			}

			texelBuffer.width = width;
			texelBuffer.height = height;
			texelBuffer.channels = 4; // TODO use true channels
			texelBuffer.layerCount = 6;

			uint64 faceSize = texelBuffer.width * texelBuffer.height * texelBuffer.channels;

			texelBuffer.texels.resize(faceSize * texelBuffer.layerCount);

			auto load = [&](const std::string& face, int32* width, int32* height, int32* channels) -> uint8*
			{
				std::string filename = (skyboxDirectory + '/' + face + fileExtention);
				uint8* rawTexels = stbi_load(filename.c_str(), width, height, channels, 4);
				*channels = 4; // TODO use true channels

				return rawTexels;
			};

			auto checkFace = [&](int32 width, int32 height, int32 channels) -> bool
			{
				return width == texelBuffer.width &&
					height == texelBuffer.height &&
					channels == texelBuffer.channels;
			};

			auto copyTexels = [&](uint8* faceTexels, uint32 index)
			{
				memcpy(texelBuffer.texels.data() + faceSize * index, faceTexels, faceSize);
			};

			auto copyFaceChecked = [&](const std::string& faceName, uint32 index) -> bool
			{
				uint8* face = nullptr;
				face = load(faceName, &width, &height, &channels);

				if (face == nullptr)
				{
					LOG_ERROR("Failed to load image %s", (skyboxDirectory + '/' + faceName + fileExtention).c_str());
					return false;
				}

				if (checkFace(width, height, channels) == false)
				{
					LOG_ERROR("Failed to load image %s (incompatibles skybox faces)", (skyboxDirectory + '/' + faceName + fileExtention).c_str());
					LOG_ERROR("different sizes or channels (%d/%d %d/%d %d/%d)", width, texelBuffer.width, height, texelBuffer.height, channels, texelBuffer.channels);
					STBI_FREE(face);
					reset();
					return false;
				}
				copyTexels(face, index);
				STBI_FREE(face);
				return true;
			};

			if (!copyFaceChecked("posx", 0)) return texelBuffer;
			if (!copyFaceChecked("negx", 1)) return texelBuffer;
			if (!copyFaceChecked("posy", 2)) return texelBuffer;
			if (!copyFaceChecked("negy", 3)) return texelBuffer;
			if (!copyFaceChecked("posz", 4)) return texelBuffer;
			copyFaceChecked("negz", 5);

			return texelBuffer;
		}
	}
}