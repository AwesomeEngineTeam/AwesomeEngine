#pragma once

#ifndef AWESOME_RENDERER_MATERIAL_GUARD
#define AWESOME_RENDERER_MATERIAL_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/ShaderBuilder/Shader.hpp>
#include <Awesome/Renderer/Pipeline/Program.hpp>
#include <Awesome/Renderer/Memory/UniformBuffer.hpp>
#include <Awesome/Renderer/Resource/SampledTexture.hpp>

#include <Awesome/Core/Color.hpp>

#include <Awesome/Maths/Vector2.hpp>

#include <memory>

namespace aw
{
	struct MaterialInfo
	{
		int32 receiveLights : 1;
		int32 receiveShadows : 1;
		int32 castShadows : 1;

		int32 hasDiffuseMap : 1;
		int32 hasEmissiveMap : 1;
		int32 hasHeightMap : 1;
		int32 hasNormalMap : 1;
		int32 hasSpecularMap : 1;
		int32 hasReflectionMap : 1;
		int32 hasRefractionMap : 1;
		int32 hasEnvironmentMap : 1;
		int32 hasOcclusionMap : 1;
		int32 hasMetalicRoughnessMap : 1;

		int32 correctGamma : 1;

		int32 isTransparent : 1;

		int32 isScreenMaterial : 1;
		int32 isSkybox : 1;

		int32 hasViewUBO : 1;
		int32 hasLightUBO : 1;
		//int32 hasLightScatteringUBO : 1;

		//int32 _padding : 12;
		int32 _padding : 13;

		inline MaterialInfo();

		inline bool HasMap() const;

		inline void SetReceiveLights(bool);
		inline void SetCorrectGamma(bool);
		inline void SetIsTransparent(bool);

		inline bool GetReceiveLights() const;
		inline bool GetCorrectGamma() const;
		inline bool GetIsTransparent() const;

		inline operator int32&() { return reinterpret_cast<int32&>(*this); };
	};

	class MaterialPipelineInfo
	{
		vk::CompareOp m_compareOp{ vk::CompareOp::eLess };
		vk::CullModeFlags m_cullMode{ vk::CullModeFlagBits::eNone };
		int16 m_polygonMode : 2;
		int16 m_frontFace : 1;
		int16 m_depthWriteEnabled : 1;
		int16 m_depthTest : 1;
		int16 m_primitiveTopology : 4;

		float m_alphaCutoff{ 0.0f };
		//pipelineInfo.SetDepthWriteEnable(false);
		//pipelineInfo.SetDepthCompareOp(vk::CompareOp::eLessOrEqual);

	public:
		inline MaterialPipelineInfo();

		inline void SetDepthCompareOp(vk::CompareOp);
		inline vk::CompareOp GetDepthCompareOp();
		inline void SetCullMode(vk::CullModeFlags);
		inline vk::CullModeFlags GetCullMode();
		inline void SetPolygonMode(vk::PolygonMode);
		inline vk::PolygonMode GetPolygonMode();
		inline void SetFrontFace(vk::FrontFace);
		inline vk::FrontFace GetFrontFace();
		inline void SetDepthWriteEnabled(bool);
		inline bool GetDepthWriteEnabled();
		inline void SetDepthTest(bool);
		inline bool GetDepthTest();
		inline void SetPrimitiveTopology(vk::PrimitiveTopology);
		inline vk::PrimitiveTopology GetPrimitiveTopology();
		inline void SetAlphaCutoff(float);
		inline float GetAlphaCutoff();
	};

	class Material
	{
		friend class MaterialManager;

		Program m_shaderProgram;
		vk::DescriptorSet m_descriptorSet;
		DescriptorSetLayoutInfo m_descLayoutInfo{};

		uint32 m_diffuse{ ~0 };
		uint32 m_emissive{ ~0 };
		uint32 m_height{ ~0 };
		uint32 m_normal{ ~0 };
		uint32 m_specular{ ~0 };
		uint32 m_reflection{ ~0 };
		uint32 m_refraction{ ~0 };
		uint32 m_environment{ ~0 };
		uint32 m_occlusion{ ~0 };
		uint32 m_metalicRoughness{ ~0 };

		bool m_modified{ true };

		Color3f m_emissiveFactor{ Color3f::Black() };
		Color4f m_baseColorFactor{ Color4f::White() };
		float m_metallicFactor{ 1.0f };
		float m_roughnessFactor{ 1.0f };

		Vector2 m_scaleUV{ 1.0f, 1.0f };
		Vector2 m_offsetUV{ 0.0f, 0.0f };

	public:
		MaterialInfo materialInfo{};
		MaterialPipelineInfo materialPipelineInfo{};

		Material() = default;
		Material(const Material&) = default;
		Material(Material&&) = default;
		~Material() = default;

		inline static void BindDescriptorSet(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, uint32 descriptorSetCount, const vk::DescriptorSet* descriptorSets, uint32 dynOffsetCount, const uint32* dynOffsets);
		inline static void BindDescriptorSet(vk::CommandBuffer commandBuffer, vk::PipelineLayout pipelineLayout, vk::ArrayProxy<const vk::DescriptorSet> descriptorSets, vk::ArrayProxy<const uint32> dynOffsets);
		//inline void PushConstants(vk::CommandBuffer commandBuffer) const;

		inline vk::DescriptorSet GetDescriptorSet() const;

		AWESOME_RENDERER_API void SetViewUBO(const UniformBuffer&) const;
		AWESOME_RENDERER_API void SetLightUBO(const UniformBuffer&) const;

		AWESOME_RENDERER_API void SetSampledTexture(const SampledTextureHandle&, uint32 binding) const;
		AWESOME_RENDERER_API void SetInputAttachment(const SampledTextureHandle&, uint32 binding) const;
		AWESOME_RENDERER_API void SetUBO(const UniformBuffer& ubo, uint32 binding) const;

		//AWESOME_RENDERER_API void FlushTextures() const;

		inline bool IsModified() const;
		inline void MarkClean();

		inline void SetDiffuse(uint32);
		inline void SetHeight(uint32);
		inline void SetNormal(uint32);
		inline void SetSpecular(uint32);
		inline void SetEnvironment(uint32);
		inline void SetReflection(uint32);
		inline void SetRefraction(uint32);
		inline void SetEmissive(uint32);
		inline void SetOcclusion(uint32);
		inline void SetMetalicRoughness(uint32);

		inline uint32 GetDiffuse();
		inline uint32 GetHeight();
		inline uint32 GetNormal();
		inline uint32 GetSpecular();
		inline uint32 GetEnvironment();
		inline uint32 GetReflection();
		inline uint32 GetRefraction();
		inline uint32 GetEmissive();
		inline uint32 GetOcclusion();
		inline uint32 GetMetalicRoughness();

		inline uint32 GetDiffuseBinding();
		inline uint32 GetHeightBinding();
		inline uint32 GetNormalBinding();
		inline uint32 GetSpecularBinding();
		inline uint32 GetEnvironmentBinding();
		inline uint32 GetReflectionBinding();
		inline uint32 GetRefractionBinding();
		inline uint32 GetEmissiveBinding();
		inline uint32 GetOcclusionBinding();
		inline uint32 GetMetalicRoughnessBinding();

		inline void SetEmissiveFactor(const Color3f& f) { m_emissiveFactor = f; };
		inline void SetBaseColorFactor(const Color4f& f) { m_baseColorFactor = f; };
		inline void SetMetallicFactor(float f) { m_metallicFactor = f; };
		inline void SetRoughnessFactor(float f) { m_roughnessFactor = f; };

		inline const Color3f& GetEmissiveFactor() { return m_emissiveFactor; };
		inline const Color4f& GetBaseColorFactor() { return m_baseColorFactor; };
		inline float GetMetallicFactor() { return m_metallicFactor; };
		inline float GetRoughnessFactor() { return m_roughnessFactor; };

		inline Vector2 GetScaleUV() const;
		inline Vector2 GetOffsetUV() const;

		inline void SetScaleUV(const Vector2&);
		inline void SetOffsetUV(const Vector2&);

	public:
		Material& operator=(const Material&) = default;
		Material& operator=(Material&&) = default;
	};

	struct MaterialHandle
	{
		vk::DescriptorSet descriptorSet;

		MaterialHandle() = default;
		inline MaterialHandle(const Material&);
		inline MaterialHandle(const MaterialHandle& o) = default;
		inline MaterialHandle(MaterialHandle&& o)  = default;
		~MaterialHandle() = default;

		MaterialHandle& operator=(const MaterialHandle&) = default;
		MaterialHandle& operator=(MaterialHandle&&) = default;
	};
}

#include <Awesome/Renderer/Resource/Material.inl>

#endif //GUARD