#include <Awesome/Renderer/Descriptor/DescriptorSet.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Descriptor/DescriptorSetLayoutInfo.hpp>
#include <Awesome/Renderer/Memory/Buffer.hpp>

namespace aw
{
	void DescriptorSet::Update(uint32 descriptorWriteCount, const vk::WriteDescriptorSet* descriptorWrites, uint32 descriptorCopyCount, const vk::CopyDescriptorSet* descriptorCopies)
	{
		Context::GetDevice().Get().updateDescriptorSets(descriptorWriteCount, descriptorWrites, descriptorCopyCount, descriptorCopies);
	}

	void DescriptorSet::SetBufferToBinding(uint32 index, vk::DescriptorType descriptorType, const Buffer& buffer, uint32 size, uint32 offset, uint32 dstArrayElement) const
	{
		vk::DescriptorBufferInfo bufferInfo(
			buffer.Get(),
			offset,
			size
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_handle,
			index,
			dstArrayElement,
			1,
			descriptorType,
			nullptr,
			&bufferInfo,
			nullptr
		);

		Update(1, &descriptorWrite);
	}

	void DescriptorSet::SetImageToBinding(uint32 index, vk::DescriptorType descriptorType, vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout) const
	{
		vk::DescriptorImageInfo imageInfo(
			sampler,
			imageView,
			imageLayout
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_handle,
			index,
			0,
			1,
			descriptorType,
			&imageInfo,
			nullptr,
			nullptr
		);

		Update(1, &descriptorWrite);
	}

	void DescriptorSet::SetBufferToBinding(uint32 index, const DescriptorSetLayoutInfo& layoutInfo, const Buffer& buffer, uint32 size, uint32 offset, uint32 dstArrayElement) const
	{
		vk::DescriptorBufferInfo bufferInfo(
			buffer.Get(),
			offset,
			size
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_handle,
			index,
			dstArrayElement,
			1,
			layoutInfo.GetBindings()[index].descriptorType,
			nullptr,
			&bufferInfo,
			nullptr
		);

		Update(1, &descriptorWrite);
	}

	void DescriptorSet::SetImageToBinding(uint32 index, const DescriptorSetLayoutInfo& layoutInfo, vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout) const
	{
		vk::DescriptorImageInfo imageInfo(
			sampler,
			imageView,
			imageLayout
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_handle,
			index,
			0,
			1,
			layoutInfo.GetBindings()[index].descriptorType,
			&imageInfo,
			nullptr,
			nullptr
		);

		Update(1, &descriptorWrite);
	}

	void DescriptorSet::SetImageToBinding(uint32 index, const DescriptorSetLayoutInfo& layoutInfo, vk::ImageView imageView, vk::ImageLayout imageLayout)
	{
		vk::DescriptorImageInfo imageInfo(
			vk::Sampler(),
			imageView,
			imageLayout
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_handle,
			index,
			0,
			1,
			layoutInfo.GetBindings()[index].descriptorType,
			&imageInfo,
			nullptr,
			nullptr
		);

		Update(1, &descriptorWrite);
	}
}