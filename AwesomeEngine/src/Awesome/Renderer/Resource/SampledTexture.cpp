#include <Awesome/Renderer/Resource/SampledTexture.hpp>
#include <Awesome/Renderer/Control/Context.hpp>

namespace aw
{
	SampledTexture::SampledTexture() : SampledTexture(vk::SamplerCreateInfo(vk::SamplerCreateFlags(), vk::Filter::eLinear, vk::Filter::eLinear))
	{
		// vk::SamplerCreateInfo createInfo(vk::SamplerCreateFlags(), vk::Filter::eLinear, vk::Filter::eLinear);
		//createInfo.magFilter

		//VkSamplerCreateInfo createInfo{
		//	VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,				// VkStructureType         sType;
		//	nullptr,											// const void*             pNext;
		//	0,													// VkSamplerCreateFlags    flags;
		//	static_cast<VkFilter>(magFilter),					// VkFilter                magFilter;
		//	static_cast<VkFilter>(minFilter),					// VkFilter                minFilter;
		//	static_cast<VkSamplerMipmapMode>(mipmapMode),		// VkSamplerMipmapMode     mipmapMode;
		//	static_cast<VkSamplerAddressMode>(addressModeU),	// VkSamplerAddressMode    addressModeU;
		//	static_cast<VkSamplerAddressMode>(addressModeV),	// VkSamplerAddressMode    addressModeV;
		//	static_cast<VkSamplerAddressMode>(addressModeW),	// VkSamplerAddressMode    addressModeW;
		//	mipLodBias,											// float                   mipLodBias;
		//	static_cast<VkBool32>(anisotropyEnable),			// VkBool32                anisotropyEnable;
		//	maxAnisotropy,										// float                   maxAnisotropy;
		//	static_cast<VkBool32>(compareEnable),				// VkBool32                compareEnable;
		//	static_cast<VkCompareOp>(compareOp),				// VkCompareOp             compareOp;
		//	minLod,												// float                   minLod;
		//	maxLod,												// float                   maxLod;
		//	static_cast<VkBorderColor>(borderColor),			// VkBorderColor           borderColor;
		//	static_cast<VkBool32>(unnormalizedCoordinates)		// VkBool32                unnormalizedCoordinates;
		//};

		// m_samplerHandle = Context::GetDevice().Get().createSamplerUnique(createInfo);
		//m_samplerHandle->Create(
		//	aw::Filter::Linear,
		//	aw::Filter::Linear,
		//	aw::SamplerMipmapMode::Nearest,
		//	aw::SamplerAddressMode::ClampToEdge,
		//	aw::SamplerAddressMode::ClampToEdge,
		//	aw::SamplerAddressMode::ClampToEdge,
		//	0.0f,
		//	false,
		//	1.0f,
		//	false,
		//	aw::CompareOp::Always,
		//	0.0f,
		//	0.0f,
		//	aw::BorderColor::FloatTransparentBlack,
		//	false
		//);
	}

	SampledTexture::SampledTexture(const vk::SamplerCreateInfo& createInfo) : Texture()
	{
		m_samplerHandle = Context::GetDevice().Get().createSamplerUnique(createInfo);
	}
}