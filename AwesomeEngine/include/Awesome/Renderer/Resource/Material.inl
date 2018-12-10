namespace aw
{
	inline MaterialInfo::MaterialInfo()
	{
		receiveLights = true;
		receiveShadows = true;
		castShadows = true;

		hasDiffuseMap = false;
		hasEmissiveMap = false;
		hasHeightMap = false;
		hasNormalMap = false;
		hasSpecularMap = false;
		hasReflectionMap = false;
		hasRefractionMap = false;
		hasEnvironmentMap = false;
		hasOcclusionMap = false;
		hasMetalicRoughnessMap = false;

		correctGamma = true;
		isTransparent = false;
		isScreenMaterial = false;
		isSkybox = false;

		hasViewUBO = false;
		hasLightUBO = false;
		//hasLightScatteringUBO = false;

		_padding = 0;
	}

	inline bool MaterialInfo::HasMap() const
	{
		return
			hasDiffuseMap ||
			hasEmissiveMap ||
			hasHeightMap ||
			hasNormalMap ||
			hasSpecularMap ||
			hasReflectionMap ||
			hasRefractionMap ||
			hasEnvironmentMap ||
			hasOcclusionMap ||
			hasMetalicRoughnessMap;
	}

	inline void MaterialInfo::SetReceiveLights(bool b)
	{
		receiveLights = b;
	}

	inline void MaterialInfo::SetCorrectGamma(bool b)
	{
		correctGamma = b;
	}

	inline void MaterialInfo::SetIsTransparent(bool b)
	{
		isTransparent = b;
	}

	inline bool MaterialInfo::GetReceiveLights() const
	{
		return receiveLights;
	}

	inline bool MaterialInfo::GetCorrectGamma() const
	{
		return correctGamma;
	}

	inline bool MaterialInfo::GetIsTransparent() const
	{
		return isTransparent;
	}

	inline MaterialPipelineInfo::MaterialPipelineInfo()
	{
		SetDepthCompareOp(vk::CompareOp::eLess);
		SetCullMode(vk::CullModeFlagBits::eNone);
		SetPolygonMode(vk::PolygonMode::eFill);
		SetFrontFace(vk::FrontFace::eCounterClockwise);
		SetDepthWriteEnabled(true);
		SetDepthTest(true);
		SetPrimitiveTopology(vk::PrimitiveTopology::eTriangleList);
	}

	inline void MaterialPipelineInfo::SetDepthCompareOp(vk::CompareOp op)
	{
		m_compareOp = op;
	}

	inline vk::CompareOp MaterialPipelineInfo::GetDepthCompareOp()
	{
		return m_compareOp;
	}

	inline void MaterialPipelineInfo::SetCullMode(vk::CullModeFlags flags)
	{
		m_cullMode = flags;
	}

	inline vk::CullModeFlags MaterialPipelineInfo::GetCullMode()
	{
		return m_cullMode;
	}

	inline void MaterialPipelineInfo::SetPolygonMode(vk::PolygonMode mode)
	{
		m_polygonMode = static_cast<int16>(mode);
	}

	inline vk::PolygonMode MaterialPipelineInfo::GetPolygonMode()
	{
		return static_cast<vk::PolygonMode>(m_polygonMode);
	}

	inline void MaterialPipelineInfo::SetFrontFace(vk::FrontFace face)
	{
		m_frontFace = static_cast<int16>(face);
	}

	inline vk::FrontFace MaterialPipelineInfo::GetFrontFace()
	{
		return static_cast<vk::FrontFace>(m_frontFace);
	}

	inline void MaterialPipelineInfo::SetDepthWriteEnabled(bool b)
	{
		m_depthWriteEnabled = b;
	}

	inline bool MaterialPipelineInfo::GetDepthWriteEnabled()
	{
		return m_depthWriteEnabled;
	}

	inline void MaterialPipelineInfo::SetDepthTest(bool enable)
	{
		m_depthTest = enable;
	}

	inline bool MaterialPipelineInfo::GetDepthTest()
	{
		return m_depthTest;
	}

	inline void MaterialPipelineInfo::SetPrimitiveTopology(vk::PrimitiveTopology topology)
	{
		m_primitiveTopology = static_cast<int16>(topology);
	}

	inline vk::PrimitiveTopology MaterialPipelineInfo::GetPrimitiveTopology()
	{
		return static_cast<vk::PrimitiveTopology>(m_primitiveTopology);
	}

	inline void MaterialPipelineInfo::SetAlphaCutoff(float cutoff)
	{
		m_alphaCutoff = cutoff;
	}

	inline float MaterialPipelineInfo::GetAlphaCutoff()
	{
		return m_alphaCutoff;
	}

	inline void Material::BindDescriptorSet(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, uint32 descriptorSetCount, const vk::DescriptorSet* descriptorSets, uint32 dynOffsetCount, const uint32* dynOffsets)
	{
		commandBuffer.bindDescriptorSets(
			vk::PipelineBindPoint::eGraphics,
			pipelineLayout,
			0,
			descriptorSetCount,
			descriptorSets,
			dynOffsetCount,
			dynOffsets
		);
	}

	inline void Material::BindDescriptorSet(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, vk::ArrayProxy<const vk::DescriptorSet> descriptorSets, vk::ArrayProxy<const uint32> dynOffsets)
	{
		commandBuffer.bindDescriptorSets(
			vk::PipelineBindPoint::eGraphics,
			pipelineLayout,
			0,
			descriptorSets,
			dynOffsets
		);
	}

	//inline void Material::PushConstants(vk::CommandBuffer commandBuffer) const
	//{
	//}

	inline vk::DescriptorSet Material::GetDescriptorSet() const
	{
		return m_descriptorSet;
	}

	inline bool Material::IsModified() const
	{
		return m_modified;
	}

	inline void Material::MarkClean()
	{
		m_modified = false;
	}

	inline void Material::SetDiffuse(uint32 diffuse)
	{
		m_diffuse = std::move(diffuse);
		materialInfo.hasDiffuseMap = true;
		m_modified = true;
//		SetSampledTexture(diffuse, 2);
	}

	inline void Material::SetHeight(uint32 height)
	{
		m_height = std::move(height);
		materialInfo.hasHeightMap = true;
		m_modified = true;
//		SetSampledTexture(displacement, 5);
	}

	inline void Material::SetNormal(uint32 normal)
	{
		m_normal = std::move(normal);
		materialInfo.hasNormalMap = true;
		m_modified = true;
//		SetSampledTexture(normal, 3);
	}

	inline void Material::SetSpecular(uint32 specular)
	{
		m_specular = std::move(specular);
		materialInfo.hasSpecularMap = true;
		m_modified = true;
//		SetSampledTexture(specular, 4);
	}

	inline void Material::SetEnvironment(uint32 environment)
	{
		m_environment = std::move(environment);
		materialInfo.hasEnvironmentMap = true;
		m_modified = true;
//		SetSampledTexture(environment, 6);
	}

	inline void Material::SetReflection(uint32 reflection)
	{
		m_reflection = std::move(reflection);
		materialInfo.hasReflectionMap = true;
		m_modified = true;
//		SetSampledTexture(reflection, 7);
	}

	inline void Material::SetRefraction(uint32 refraction)
	{
		m_refraction = std::move(refraction);
		materialInfo.hasRefractionMap = true;
		m_modified = true;
//		SetSampledTexture(refraction, 8);
	}

	inline void Material::SetEmissive(uint32 emissive)
	{
		m_emissive = std::move(emissive);
		materialInfo.hasEmissiveMap = true;
		m_modified = true;
		//		SetSampledTexture(refraction, 9);
	}

	inline void Material::SetOcclusion(uint32 occlusion)
	{
		m_occlusion = std::move(occlusion);
		materialInfo.hasOcclusionMap = true;
		m_modified = true;
		//		SetSampledTexture(refraction, 10);
	}

	inline void Material::SetMetalicRoughness(uint32 metalicRoughness)
	{
		m_metalicRoughness = std::move(metalicRoughness);
		materialInfo.hasMetalicRoughnessMap = true;
		m_modified = true;
		//		SetSampledTexture(refraction, 11);
	}

	inline uint32 Material::GetDiffuse()
	{
		return m_diffuse;
	}

	inline uint32 Material::GetHeight()
	{
		return m_height;
	}

	inline uint32 Material::GetNormal()
	{
		return m_normal;
	}

	inline uint32 Material::GetSpecular()
	{
		return m_specular;
	}

	inline uint32 Material::GetEnvironment()
	{
		return m_environment;
	}

	inline uint32 Material::GetReflection()
	{
		return m_reflection;
	}

	inline uint32 Material::GetRefraction()
	{
		return m_refraction;
	}

	inline uint32 Material::GetEmissive()
	{
		return m_emissive;
	}

	inline uint32 Material::GetOcclusion()
	{
		return m_occlusion;
	}

	inline uint32 Material::GetMetalicRoughness()
	{
		return m_metalicRoughness;
	}

	inline uint32 Material::GetDiffuseBinding()
	{
		if (materialInfo.isSkybox)
			return 0;
		else
			return 2;
	}

	inline uint32 Material::GetHeightBinding()
	{
		return 5;
	}

	inline uint32 Material::GetNormalBinding()
	{
		return 3;
	}

	inline uint32 Material::GetSpecularBinding()
	{
		return 4;
	}

	inline uint32 Material::GetEnvironmentBinding()
	{
		return 6;
	}

	inline uint32 Material::GetReflectionBinding()
	{
		return 7;
	}

	inline uint32 Material::GetRefractionBinding()
	{
		return 8;
	}

	inline uint32 Material::GetEmissiveBinding()
	{
		return 9;
	}

	inline uint32 Material::GetOcclusionBinding()
	{
		return 10;
	}

	inline uint32 Material::GetMetalicRoughnessBinding()
	{
		return 11;
	}

	inline Vector2 Material::GetScaleUV() const
	{
		return m_scaleUV;
	}

	inline Vector2 Material::GetOffsetUV() const
	{
		return m_offsetUV;
	}

	inline void Material::SetScaleUV(const Vector2& scale)
	{
		m_scaleUV = scale;
	}

	inline void Material::SetOffsetUV(const Vector2& offset)
	{
		m_offsetUV = offset;
	}

	inline MaterialHandle::MaterialHandle(const Material& material) :
		descriptorSet{ material.GetDescriptorSet() }
	{
	}

	//inline void MaterialHandle::BindDescriptorSet(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, uint32 descriptorSetCount, const vk::DescriptorSet* descriptorSets, uint32 dynOffsetCount, const uint32* dynOffsets) const
	//{
	//	commandBuffer.bindDescriptorSets(
	//		vk::PipelineBindPoint::eGraphics,
	//		pipelineLayout,
	//		0,
	//		descriptorSetCount,
	//		descriptorSets,
	//		dynOffsetCount,
	//		dynOffsets
	//	);
	//}

	//inline void MaterialHandle::BindDescriptorSet(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, vk::ArrayProxy<const vk::DescriptorSet> descriptorSets, vk::ArrayProxy<const uint32> dynOffsets) const
	//{
	//	commandBuffer.bindDescriptorSets(
	//		vk::PipelineBindPoint::eGraphics,
	//		pipelineLayout,
	//		0,
	//		descriptorSets,
	//		dynOffsets
	//	);
	//}
}
