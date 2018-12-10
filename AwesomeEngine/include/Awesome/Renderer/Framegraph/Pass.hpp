#pragma once

#ifndef AWESOME_RENDERER_PASS_GUARD
#define AWESOME_RENDERER_PASS_GUARD

#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Types.hpp>

namespace aw
{
	struct Pass
	{
		vk::RenderPass renderPass{};
		vk::Framebuffer framebuffer{};
		std::vector<vk::PipelineColorBlendAttachmentState> colorBlendStates{};
		uint32 inputCount{ 0 };
		uint32 colorOutputCount{ 0 };
	};
}

#include <Awesome/Renderer/Framegraph/Pass.inl>

#endif //GUARD