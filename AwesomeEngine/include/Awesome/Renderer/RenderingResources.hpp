#pragma once

#ifndef AWESOME_RENDERER_RENDERING_RESOURCES_GUARD
#define AWESOME_RENDERER_RENDERING_RESOURCES_GUARD

#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>

#include <Awesome/Core/Types.hpp>

namespace aw
{
	struct RenderingResources
	{
		vk::CommandBuffer commandBuffer{};

		uint32 imageIndex{ ~0u };
	};
}

//#include <Awesome/Renderer/Framegraph/Framegraph.inl>

#endif //GUARD