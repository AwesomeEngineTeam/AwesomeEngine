#pragma once

#ifndef AWESOME_RENDERER_RENDER_TEXTURE_GUARD
#define AWESOME_RENDERER_RENDER_TEXTURE_GUARD

#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>

#include <Awesome/Core/Types.hpp>

namespace aw
{
	struct RenderTexture
	{
		TextureHandle textureHandle{};
		vk::Extent2D extent{};
	};
}

//#include <Awesome/Renderer/Framegraph/RenderTexture.inl>

#endif //GUARD