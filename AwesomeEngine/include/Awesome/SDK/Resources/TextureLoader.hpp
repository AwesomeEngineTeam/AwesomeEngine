#pragma once

#ifndef AWESOME_SDK_TEXTURE_LOADER_GUARD
#define AWESOME_SDK_TEXTURE_LOADER_GUARD

#include <Awesome/SDK/Config.hpp>

#include <Awesome/Renderer/Resource/TexelBuffer.hpp>

#include <Awesome/Core/Logger/Logger.hpp>

namespace aw
{
	namespace sdk
	{
		enum class TexelBufferLoadParamsBit : uint32
		{
			None = 0,
			InvertR = 0x1,
			InvertG = 0x2,
			InvertB = 0x4,
			InvertRGB = 0x7,
			InvertA = 0x8,
			InvertRGBA = 0xf,
			MeshLoadParamsBit_MAX = uint32(InvertRGBA),
		};

		enum class TexelBufferLoadComponent : uint32
		{
			Grey = 1,
			GreyAlpha = 2,
			RGB = 3,
			RGBAlpha = 4
		};

		constexpr TexelBufferLoadParamsBit operator|(TexelBufferLoadParamsBit lhs, TexelBufferLoadParamsBit rhs)
		{
			return TexelBufferLoadParamsBit(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
		}

		constexpr TexelBufferLoadParamsBit operator&(TexelBufferLoadParamsBit lhs, TexelBufferLoadParamsBit rhs)
		{
			return TexelBufferLoadParamsBit(static_cast<uint32>(lhs) & static_cast<uint32>(rhs));
		}

		class TextureLoader
		{
		public:
			TextureLoader() = delete;
			~TextureLoader() = delete;

			AWESOME_SDK_API static TexelBuffer Load(const std::string& filename, TexelBufferLoadComponent loadComponent = TexelBufferLoadComponent::RGBAlpha, TexelBufferLoadParamsBit params = TexelBufferLoadParamsBit::None);
			AWESOME_SDK_API static TexelBuffer LoadSkybox(const std::string& skyboxDirectory, const std::string& fileExtention = ".jpg");
		};
	}
}

#include <Awesome/SDK/Resources/TextureLoader.inl>

#endif //GUARD