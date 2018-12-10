#pragma once

#ifndef AWESOME_RENDERER_DESCRIPTOR_SET_GUARD
#define AWESOME_RENDERER_DESCRIPTOR_SET_GUARD

#include <Awesome/Renderer/Prerequisites.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>

#include <Awesome/Core/Types.hpp>

namespace aw
{
	class DescriptorSetLayoutInfo;
	class Buffer;
	//class Sampler;
	//class ImageView;

	class DescriptorSet
	{
		vk::DescriptorSet m_handle{};

	public:
		DescriptorSet() = default;
		inline DescriptorSet(vk::DescriptorSet);
		DescriptorSet(const DescriptorSet&) = default;
		DescriptorSet(DescriptorSet&&) = default;
		~DescriptorSet() = default;

		AWESOME_RENDERER_API static void Update(uint32 descriptorWriteCount, const vk::WriteDescriptorSet* descriptorWrites, uint32 descriptorCopyCount = 0, const vk::CopyDescriptorSet* descriptorCopies = nullptr);
		AWESOME_RENDERER_API void SetBufferToBinding(uint32 index, vk::DescriptorType, const Buffer&, uint32 size, uint32 offset = 0, uint32 dstArrayElement = 0) const;
		AWESOME_RENDERER_API void SetImageToBinding(uint32 index, vk::DescriptorType, vk::Sampler, vk::ImageView, vk::ImageLayout) const;
		AWESOME_RENDERER_API void SetBufferToBinding(uint32 index, const DescriptorSetLayoutInfo& layoutInfo, const Buffer& buffer, uint32 size, uint32 offset = 0, uint32 dstArrayElement = 0) const;
		AWESOME_RENDERER_API void SetImageToBinding(uint32 index, const DescriptorSetLayoutInfo& layoutInfo, vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout) const;

		/**
		* DON'T use this overload if the binding type is not VK_DESCRIPTOR_TYPE_SAMPLER or VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER or if you haven't set any immutable sampler to it !
		*
		* @param A binding index.
		* @param B layout info.
		* @param C image view.
		* @param D image layout type.
		*/
		AWESOME_RENDERER_API void SetImageToBinding(uint32 index, const DescriptorSetLayoutInfo& layoutInfo, vk::ImageView imageView, vk::ImageLayout imageLayout);

		DescriptorSet& operator=(const DescriptorSet&) = default;
		DescriptorSet& operator=(DescriptorSet&&) = default;
	};

	static_assert(sizeof(DescriptorSet) == sizeof(VkDescriptorSet), "handle and wrapper have different size!");
}

#include <Awesome/Renderer/Descriptor/DescriptorSet.inl>

#endif //GUARD