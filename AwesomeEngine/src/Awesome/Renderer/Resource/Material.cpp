#include <Awesome/Renderer/Resource/Material.hpp>
//#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Resource/SampledTexture.hpp>

namespace aw
{
	void Material::SetViewUBO(const UniformBuffer& uview) const
	{
		const SubBuffer& view = uview.GetDestination();

		vk::DescriptorBufferInfo bufferInfo(
			view.buffer->Get(), view.offset, view.size
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_descriptorSet,
			0,
			0,
			1,
			vk::DescriptorType::eUniformBuffer,
			nullptr,
			&bufferInfo
		);

		Context::GetDevice().Get().updateDescriptorSets(descriptorWrite, nullptr);
	}

	void Material::SetLightUBO(const UniformBuffer& ulight) const
	{
		const SubBuffer& light = ulight.GetDestination();

		vk::DescriptorBufferInfo bufferInfo(
			light.buffer->Get(), light.offset, light.size
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_descriptorSet,
			1,
			0,
			1,
			vk::DescriptorType::eUniformBuffer,
			nullptr,
			&bufferInfo
		);

		Context::GetDevice().Get().updateDescriptorSets(descriptorWrite, nullptr);
	}

	void Material::SetSampledTexture(const SampledTextureHandle& tex, uint32 binding) const
	{
		vk::DescriptorImageInfo imageInfo(
			tex.GetSampler(),
			tex.GetImageView(),
			vk::ImageLayout::eShaderReadOnlyOptimal
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_descriptorSet,
			binding,
			0,
			1,
			vk::DescriptorType::eCombinedImageSampler,
			&imageInfo
		);

		Context::GetDevice().Get().updateDescriptorSets(descriptorWrite, nullptr);
	}

	void Material::SetInputAttachment(const SampledTextureHandle& tex, uint32 binding) const
	{
		vk::DescriptorImageInfo imageInfo(
			tex.GetSampler(),
			tex.GetImageView(),
			vk::ImageLayout::eShaderReadOnlyOptimal
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_descriptorSet,
			binding,
			0,
			1,
			vk::DescriptorType::eInputAttachment,
			&imageInfo
		);

		Context::GetDevice().Get().updateDescriptorSets(descriptorWrite, nullptr);
	}

	void Material::SetUBO(const UniformBuffer& ubo, uint32 binding) const
	{
		const SubBuffer& sub = ubo.GetDestination();

		vk::DescriptorBufferInfo bufferInfo(
			sub.buffer->Get(), sub.offset, sub.size
		);

		vk::WriteDescriptorSet descriptorWrite(
			m_descriptorSet,
			binding,
			0,
			1,
			vk::DescriptorType::eUniformBuffer,
			nullptr,
			&bufferInfo
		);

		Context::GetDevice().Get().updateDescriptorSets(descriptorWrite, nullptr);
	}

	//void Material::FlushTextures() const
	//{
	//	if (materialInfo.hasDiffuseMap)
	//		if (materialInfo.isSkybox)
	//			SetSampledTexture(m_diffuse, 0);
	//		else
	//			SetSampledTexture(m_diffuse, 2);
	//	if (materialInfo.hasHeightMap)
	//		SetSampledTexture(m_height, 5);
	//	if (materialInfo.hasNormalMap)
	//		SetSampledTexture(m_normal, 3);
	//	if (materialInfo.hasSpecularMap)
	//		SetSampledTexture(m_specular, 4);
	//	if (materialInfo.hasEnvironmentMap)
	//		SetSampledTexture(m_environment, 6);
	//	if (materialInfo.hasReflectionMap)
	//		SetSampledTexture(m_reflection, 7);
	//	if (materialInfo.hasRefractionMap)
	//		SetSampledTexture(m_refraction, 8);
	//	if (materialInfo.hasEmissiveMap)
	//		SetSampledTexture(m_emissive, 9);
	//	if (materialInfo.hasOcclusionMap)
	//		SetSampledTexture(m_occlusion, 10);
	//	if (materialInfo.hasMetalicRoughnessMap)
	//		SetSampledTexture(m_metalicRoughness, 11);
	//}
}