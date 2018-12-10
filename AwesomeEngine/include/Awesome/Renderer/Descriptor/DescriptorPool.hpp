#pragma once

#ifndef AWESOME_RENDERER_DESCRIPTOR_POOL_GUARD
#define AWESOME_RENDERER_DESCRIPTOR_POOL_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>

#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Types.hpp>

#include <Awesome/Renderer/Descriptor/DescriptorSet.hpp>

namespace aw
{
	class DescriptorPool
	{
		vk::UniqueDescriptorPool m_handle;

	public:
		DescriptorPool() = default;
		inline DescriptorPool(const vk::DescriptorPoolCreateInfo& createInfo);
		DescriptorPool(const DescriptorPool&) = default;
		DescriptorPool(DescriptorPool&&) = default;
		inline ~DescriptorPool();

		AWESOME_RENDERER_API void Create(const vk::DescriptorPoolCreateInfo& createInfo);
		AWESOME_RENDERER_API void Create(const std::vector<DescriptorSetLayoutInfo>& createInfos, uint32 maxSets = 0);
		AWESOME_RENDERER_API void Destroy();

		inline vk::DescriptorPool Get() const;

		DescriptorPool& operator=(const DescriptorPool&) = default;
		DescriptorPool& operator=(DescriptorPool&&) = default;
	};
}

#include <Awesome/Renderer/Descriptor/DescriptorPool.inl>

#endif //GUARD