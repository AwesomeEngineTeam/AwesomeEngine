#pragma once

#ifndef AWESOME_RENDERER_RENDER_AREA_GUARD
#define AWESOME_RENDERER_RENDER_AREA_GUARD

#include <Awesome/Core/Reflexion/Reflect.hpp>

#include <Awesome/Renderer/Control/Vulkan.hpp>

namespace aw
{
	struct RenderArea
	{
		vk::Viewport viewport;
		vk::Rect2D scissor;

		REFLECT_BEGIN(RenderArea)
			REFLECT_MEMBER(viewport)
			REFLECT_MEMBER(scissor)
		REFLECT_END()
	};
}

#endif //GUARD