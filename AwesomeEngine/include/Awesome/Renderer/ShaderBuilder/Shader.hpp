#pragma once

#ifndef AWESOME_RENDERER_SHADER_GUARD
#define AWESOME_RENDERER_SHADER_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>

namespace aw
{
	struct Shader
	{
		vk::UniquePipeline pipeline;
		vk::UniquePipelineLayout pipelineLayout;
		vk::UniquePipelineCache pipelineCache;

		Shader() = default;
		Shader(const Shader&) = default;
		Shader(Shader&&) = default;
		~Shader() = default;

		AWESOME_RENDERER_API void Create(class PipelineInfo&&, const vk::PipelineLayoutCreateInfo& layoutInfo);

		AWESOME_RENDERER_API void Bind(vk::CommandBuffer) const;

		Shader& operator=(const Shader&) = default;
		Shader& operator=(Shader&&) = default;
	};

	struct ShaderHandle
	{
		vk::Pipeline pipeline;
		vk::PipelineLayout pipelineLayout;

		ShaderHandle() = default;
		inline ShaderHandle(const Shader&);
		ShaderHandle(const ShaderHandle&) = default;
		ShaderHandle(ShaderHandle&&) = default;
		~ShaderHandle() = default;

		AWESOME_RENDERER_API void Bind(vk::CommandBuffer) const;

		ShaderHandle& operator=(const ShaderHandle&) = default;
		ShaderHandle& operator=(ShaderHandle&&) = default;
	};
}

#include <Awesome/Renderer/ShaderBuilder/Shader.inl>

#endif //GUARD