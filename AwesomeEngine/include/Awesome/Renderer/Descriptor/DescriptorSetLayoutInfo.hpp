#pragma once

#ifndef AWESOME_RENDERER_DESCRIPTOR_SET_LAYOUT_INFO_GUARD
#define AWESOME_RENDERER_DESCRIPTOR_SET_LAYOUT_INFO_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Config.hpp>

#include <Awesome/Core/Logger/Logger.hpp>

#include <vector>

namespace aw
{
	class DescriptorSetLayoutInfo
	{
		std::vector<vk::DescriptorSetLayoutBinding> m_descriptorSetLayoutBindings;
		mutable vk::DescriptorSetLayoutCreateInfo m_descriptorSetLayoutCreateInfo{};
		std::vector<vk::DescriptorPoolSize> m_descriptorPoolSizes;

	public:
		DescriptorSetLayoutInfo() = default;
		DescriptorSetLayoutInfo(const DescriptorSetLayoutInfo&) = default;
		DescriptorSetLayoutInfo(DescriptorSetLayoutInfo&&) = default;
		~DescriptorSetLayoutInfo() = default;

		inline const vk::DescriptorSetLayoutCreateInfo& GetCreateInfo() const;
		inline const std::vector<vk::DescriptorPoolSize>& GetDescriptorPoolSizes() const;
		inline const std::vector<vk::DescriptorSetLayoutBinding>& GetBindings() const;

		inline void AddBinding(vk::DescriptorType type, vk::ShaderStageFlags flags, uint32 descriptorCount = 1, const vk::Sampler* samplers = nullptr);

		inline void SetBindingCount(uint32 count);
		inline void SetBindingType(uint32 bindingIndex, vk::DescriptorType type);
		inline void SetBindingDescriptorCount(uint32 bindingIndex, uint32 count);
		inline void SetBindingStageFlags(uint32 bindingIndex, vk::ShaderStageFlags flags);
		inline void SetBindingImmutableSamplers(uint32 bindingIndex, const vk::Sampler* samplers);

		DescriptorSetLayoutInfo& operator=(const DescriptorSetLayoutInfo&) = default;
		DescriptorSetLayoutInfo& operator=(DescriptorSetLayoutInfo&&) = default;
	};
}

#include <Awesome/Renderer/Descriptor/DescriptorSetLayoutInfo.inl>

#endif //GUARD