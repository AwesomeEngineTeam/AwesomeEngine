#pragma once

#ifndef AWESOME_RENDERER_MATERIAL_MANAGER_GUARD
#define AWESOME_RENDERER_MATERIAL_MANAGER_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Pipeline/Program.hpp>
#include <Awesome/Renderer/ShaderBuilder/Shader.hpp>
#include <Awesome/Renderer/Descriptor/DescriptorSetLayoutInfo.hpp>
#include <Awesome/Renderer/Resource/Material.hpp>

namespace aw
{
	struct Pass;
	class PipelineInfo;
	class Material;
	class VertexLayout;

	class MaterialManager
	{
		struct UniqueMaterial
		{
			DescriptorSetLayoutInfo descriptorSetLayoutInfo{};
			vk::UniqueDescriptorSetLayout descriptorSetLayout{};
		};

		struct UniqueDescriptorPoolSet
		{
			vk::UniqueDescriptorPool descriptorPool{};
			vk::UniqueDescriptorSet descriptorSet{};
		};

		std::vector<Shader> m_shaders{};
		std::vector<UniqueMaterial> m_miscUniqueMaterials{};
		std::vector<UniqueDescriptorPoolSet> m_uniqueDescriptorPoolSets{};

	public:
		MaterialManager() = default;
		MaterialManager(const MaterialManager&) = delete;
		MaterialManager(MaterialManager&&) = default;
		~MaterialManager() = default;

		AWESOME_RENDERER_API ShaderHandle CreateShader(Material&, Pass&, const VertexLayout&);
		
		AWESOME_RENDERER_API void CreateMaterial(Material&, const std::function<Program(MaterialInfo&, DescriptorSetLayoutInfo&)>& makeShaderProgram);
		
	private:
		AWESOME_RENDERER_API Shader CreateUniqueShader(PipelineInfo, const DescriptorSetLayoutInfo&);
		AWESOME_RENDERER_API Shader CreateUniqueScreenShader(PipelineInfo, const DescriptorSetLayoutInfo&);
		AWESOME_RENDERER_API UniqueMaterial CreateUniqueMaterial(DescriptorSetLayoutInfo);
		AWESOME_RENDERER_API UniqueMaterial CreateUniqueScreenMaterial(DescriptorSetLayoutInfo);

		AWESOME_RENDERER_API UniqueDescriptorPoolSet CreateUniqueDescriptorPoolSet(UniqueMaterial&);

	public:
		MaterialManager& operator=(const MaterialManager&) = delete;
		MaterialManager& operator=(MaterialManager&&) = default;
	};
}

#include <Awesome/Renderer/Resource/MaterialManager.inl>

#endif //GUARD