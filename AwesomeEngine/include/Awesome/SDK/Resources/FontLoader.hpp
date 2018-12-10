#pragma once

#ifndef AWESOME_SDK_FONT_LOADER_GUARD
#define AWESOME_SDK_FONT_LOADER_GUARD

#include <Awesome/SDK/Config.hpp>

#include <Awesome/Renderer/Resource/FontAtlas.hpp>

#include <Awesome/Core/Logger/Logger.hpp>

//#include <imgui/imgui.h>

namespace aw
{
	namespace sdk
	{
		class FontLoader
		{
		public:
			FontLoader() = delete;
			~FontLoader() = delete;

			AWESOME_SDK_API static FontAtlas Load(class ResourceManager&, const std::string& filename, float pixelSize);
		};
	}
}

#include <Awesome/SDK/Resources/FontLoader.inl>

#endif //GUARD