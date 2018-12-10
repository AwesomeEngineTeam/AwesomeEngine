#include <Awesome/Renderer/Resource/MaterialManager.hpp>
#include <Awesome/Renderer/Pipeline/PipelineInfo.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertex.hpp>
#include <Awesome/Renderer/Framegraph/Pass.hpp>
#include <Awesome/Renderer/Resource/Material.hpp>
//#include <Awesome/Renderer/ShaderBuilder/ShaderBuilder.hpp>
#include <Awesome/Renderer/Control/Context.hpp>

namespace aw
{
	ShaderHandle MaterialManager::CreateShader(Material& material, Pass& pass, const VertexLayout& vertexLayout)
	{
		PipelineInfo pipelineInfo;
		pipelineInfo.SetRenderPass(pass.renderPass);
		pipelineInfo.SetColorBlendAttachments(pass.colorBlendStates);
		pipelineInfo.AddViewport(vk::Viewport(0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f));
		pipelineInfo.AddScissor(vk::Rect2D({}, { 1, 1 }));
		pipelineInfo.AddVertexLayout(vertexLayout);
	
		pipelineInfo.SetCullMode(material.materialPipelineInfo.GetCullMode());
		pipelineInfo.SetFrontFace(material.materialPipelineInfo.GetFrontFace());
		pipelineInfo.SetPolygonMode(material.materialPipelineInfo.GetPolygonMode());
		pipelineInfo.SetDepthWriteEnable(material.materialPipelineInfo.GetDepthWriteEnabled());
		pipelineInfo.SetDepthTestEnable(material.materialPipelineInfo.GetDepthTest());
		pipelineInfo.SetDepthCompareOp(material.materialPipelineInfo.GetDepthCompareOp());
		pipelineInfo.SetTopology(material.materialPipelineInfo.GetPrimitiveTopology());

		//DescriptorSetLayoutInfo descLayoutInfo;
		//pipelineInfo.SetProgram(sb::ShaderBuilder::MakeShaderProgram(material.materialInfo, descLayoutInfo));
		pipelineInfo.SetProgram(&material.m_shaderProgram);
	
		//Shader shader;
		//shader.Create(std::move(pipelineInfo), );
	
		//Shader shader = CreateUniqueShader(pipelineInfo, std::move(descLayoutInfo));
	
		//material.m_shader = ShaderHandle(uniqueMaterial.shader);
		//material.m_descriptorPool = uniqueDescriptorPoolSet.descriptorPool.get();
		//material.m_descriptorSet = uniqueDescriptorPoolSet.descriptorSet.get();
	
		//m_uniqueMaterials[material.materialInfo] = std::move(uniqueMaterial);
		if (material.materialInfo.isScreenMaterial)
			m_shaders.emplace_back(CreateUniqueScreenShader(std::move(pipelineInfo), material.m_descLayoutInfo));
		else
			m_shaders.emplace_back(CreateUniqueShader(std::move(pipelineInfo), material.m_descLayoutInfo));

		return m_shaders.back();
	}

	void MaterialManager::CreateMaterial(Material& material, const std::function<Program(MaterialInfo&, DescriptorSetLayoutInfo&)>& makeShaderProgram)
	{
		DescriptorSetLayoutInfo descLayoutInfo;

		material.m_shaderProgram = makeShaderProgram(material.materialInfo, descLayoutInfo);

		UniqueMaterial uniqueMaterial =
			material.materialInfo.isScreenMaterial ? 
			CreateUniqueScreenMaterial(std::move(descLayoutInfo)) :
			CreateUniqueMaterial(std::move(descLayoutInfo));

		UniqueDescriptorPoolSet uniqueDescriptorPoolSet = CreateUniqueDescriptorPoolSet(uniqueMaterial);

		material.m_descriptorSet = uniqueDescriptorPoolSet.descriptorSet.get();
		material.m_descLayoutInfo = uniqueMaterial.descriptorSetLayoutInfo;

		m_miscUniqueMaterials.emplace_back(std::move(uniqueMaterial));
		m_uniqueDescriptorPoolSets.emplace_back(std::move(uniqueDescriptorPoolSet));
	}

	Shader MaterialManager::CreateUniqueShader(PipelineInfo pipelineInfo, const DescriptorSetLayoutInfo& descLayoutInfo)
	{
		DescriptorSetLayoutInfo geometryDescLayoutInfo;
		geometryDescLayoutInfo.AddBinding(vk::DescriptorType::eUniformBufferDynamic, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);

		auto geometryDescLayout = Context::GetDevice().Get().createDescriptorSetLayoutUnique(geometryDescLayoutInfo.GetCreateInfo());
		auto descLayout = Context::GetDevice().Get().createDescriptorSetLayoutUnique(descLayoutInfo.GetCreateInfo());

		std::array<vk::DescriptorSetLayout, 2> layouts{ { geometryDescLayout.get(), descLayout.get() } };
		vk::PipelineLayoutCreateInfo layoutInfo(
			vk::PipelineLayoutCreateFlags(),
			uint32(layouts.size()),
			layouts.data()
		);

		Shader shader;
		shader.Create(std::move(pipelineInfo), layoutInfo);

		return shader;
	}

	Shader MaterialManager::CreateUniqueScreenShader(PipelineInfo pipelineInfo, const DescriptorSetLayoutInfo& descLayoutInfo)
	{
		auto descLayout = Context::GetDevice().Get().createDescriptorSetLayoutUnique(descLayoutInfo.GetCreateInfo());

		std::array<vk::DescriptorSetLayout, 1> layouts{ { descLayout.get() } };
		vk::PipelineLayoutCreateInfo layoutInfo(
			vk::PipelineLayoutCreateFlags(),
			uint32(layouts.size()),
			layouts.data()
		);

		Shader shader;
		shader.Create(std::move(pipelineInfo), layoutInfo);

		return shader;
	}

	MaterialManager::UniqueMaterial MaterialManager::CreateUniqueMaterial(DescriptorSetLayoutInfo descLayoutInfo)
	{
		DescriptorSetLayoutInfo geometryDescLayoutInfo;
		geometryDescLayoutInfo.AddBinding(vk::DescriptorType::eUniformBufferDynamic, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);

		auto geometryDescLayout =	Context::GetDevice().Get().createDescriptorSetLayoutUnique(geometryDescLayoutInfo.GetCreateInfo());
		auto descLayout =			Context::GetDevice().Get().createDescriptorSetLayoutUnique(descLayoutInfo.GetCreateInfo());

		std::array<vk::DescriptorSetLayout, 2> layouts{ { geometryDescLayout.get(), descLayout.get() } };
		vk::PipelineLayoutCreateInfo layoutInfo(
			vk::PipelineLayoutCreateFlags(),
			uint32(layouts.size()),
			layouts.data()
		);

		UniqueMaterial uniqueMaterial;
		uniqueMaterial.descriptorSetLayoutInfo = std::move(descLayoutInfo);
		uniqueMaterial.descriptorSetLayout.swap(descLayout);

		return uniqueMaterial;
	}

	MaterialManager::UniqueMaterial MaterialManager::CreateUniqueScreenMaterial(DescriptorSetLayoutInfo descLayoutInfo)
	{
		auto descLayout = Context::GetDevice().Get().createDescriptorSetLayoutUnique(descLayoutInfo.GetCreateInfo());

		std::array<vk::DescriptorSetLayout, 1> layouts{ { descLayout.get() } };
		vk::PipelineLayoutCreateInfo layoutInfo(
			vk::PipelineLayoutCreateFlags(),
			uint32(layouts.size()),
			layouts.data()
		);

		UniqueMaterial uniqueMaterial;
		uniqueMaterial.descriptorSetLayoutInfo = std::move(descLayoutInfo);
		uniqueMaterial.descriptorSetLayout.swap(descLayout);

		return uniqueMaterial;
	}

	MaterialManager::UniqueDescriptorPoolSet MaterialManager::CreateUniqueDescriptorPoolSet(UniqueMaterial& uniqueMaterial)
	{
		UniqueDescriptorPoolSet uniqueDescritptorPoolSet;

		const auto& poolSizes = uniqueMaterial.descriptorSetLayoutInfo.GetDescriptorPoolSizes();

		if (poolSizes.empty() == true)
			return uniqueDescritptorPoolSet;

		vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo(
			vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
			1,
			poolSizes.size(),
			poolSizes.data()
		);

		uniqueDescritptorPoolSet.descriptorPool = Context::GetDevice().Get().createDescriptorPoolUnique(descriptorPoolCreateInfo);

		vk::DescriptorSetAllocateInfo descriptorSetAllocateInfo(
			uniqueDescritptorPoolSet.descriptorPool.get(),
			1,
			&uniqueMaterial.descriptorSetLayout.get()
		);

		auto uniqueDescriptorSets = Context::GetDevice().Get().allocateDescriptorSetsUnique(descriptorSetAllocateInfo);
		uniqueDescritptorPoolSet.descriptorSet.swap(uniqueDescriptorSets[0]);

		return uniqueDescritptorPoolSet;
	}
}