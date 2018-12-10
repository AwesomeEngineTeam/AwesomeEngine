#pragma once

#ifndef AWESOME_RENDERER_PROGRAM_GUARD
#define AWESOME_RENDERER_PROGRAM_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Config.hpp>

#include <vector>
#include <list>

namespace aw
{
	class Program
	{
		std::list<vk::UniqueShaderModule> m_modules{};
		std::vector<vk::PipelineShaderStageCreateInfo> m_stages{};

	public:
		Program() = default;
		Program(const Program&) = delete;
		Program(Program&&) = default;
		~Program() = default;

		inline void AddShaderModule(vk::ShaderModule shader, vk::ShaderStageFlagBits stage, const char* name = "main");
		inline void AddShaderModule(vk::UniqueShaderModule&& shader, vk::ShaderStageFlagBits stage, const char* name = "main");
		inline const vk::PipelineShaderStageCreateInfo* GetStages() const;
		inline uint32 GetStageCount() const;

		Program& operator=(const Program&) = delete;
		Program& operator=(Program&&) = default;
	};
}

#include <Awesome/Renderer/Pipeline/Program.inl>

#endif //GUARD