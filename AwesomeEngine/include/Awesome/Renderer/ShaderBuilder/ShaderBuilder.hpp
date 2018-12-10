#pragma once

#ifndef AWESOME_RENDERER_SHADER_BUILDER_GUARD
#define AWESOME_RENDERER_SHADER_BUILDER_GUARD

#include <Awesome/Renderer/Config.hpp>
#include <Awesome/Renderer/Control/Vulkan.hpp>
#include <Awesome/Renderer/Pipeline/Program.hpp>
#include <Awesome/Renderer/Resource/Material.hpp>
#include <Awesome/Renderer/Memory/UniformBufferLayout.hpp>

#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Debug.hpp>

#include <memory>

#include <SPIRV/SpvBuilder.h>
#include <SPIRV/GLSL.std.450.h>
#include <SPIRV/spirv.hpp>

namespace aw
{
	class VertexLayout;
	class DescriptorSetLayoutInfo;

	namespace sb
	{
		struct IdWrapper;
		struct Type;
		struct Value;
		struct Variable;
		struct Function;

		class AWESOME_RENDERER_API ShaderBuilder
		{
			friend IdWrapper;
			friend Type;
			friend Value;
			friend Variable;
			friend Function;

			struct BuilderContent;

			static std::unique_ptr<BuilderContent> s_builderContent;

		public:
			ShaderBuilder() = delete;
			ShaderBuilder(const ShaderBuilder&) = delete;
			ShaderBuilder(ShaderBuilder&&) = delete;
			~ShaderBuilder() = delete;

			static void Begin();
			static void End();

			static spv::Builder& GetBuilder();

		private:
			static spv::Id GetGlsl450Import();

		public:
			static void WhileLoop(const std::function<Value()>& condition, const std::function<void()>& body);
			static void WhileLoopBegin(spv::Builder::LoopBlocks& loopBlocks, spv::Block& moreBlock, Value condition);
			static void WhileLoopEnd(spv::Builder::LoopBlocks& loopBlocks, spv::Block& moreBlock);
			static spv::Instruction* AddEntryPointDefault(spv::ExecutionModel, Function&, const char*);
			static void ProcessAttributesVertexDefault(const VertexLayout&, bool createOuts = true);
			static void ProcessAttributesFragmentDefault(const VertexLayout&);

			static void ProcessGl_PerVertexVertexDefault(Value& mvpMat, Value& homogenePosition, Variable& gl_PerVertex);
			static void ProcessPositionVertexDefault(Value& modelMat, Value& homogenePosition);
			static void ProcessUVVertexDefault();
			static void ProcessUVWVertexDefault();
			static void ProcessNormalVertexDefault(Value& modelMat);
			static void ProcessTangentVertexDefault(Value& modelMat);
			static void ProcessHandednessVertexDefault();
			static void ProcessColorVertexDefault();

		private:
			static void AddIdOperandIfExists(spv::Instruction* instruction, const std::string& name);
			static void ProcessUBL(const UniformBufferLayout&);

		public:
			static Function DeclareParallaxMappingFunc();
			static void GammaCorrect(Variable& color);
			static Value LightDiffuseIntensity(const Value& normal, const Value& lightDir);
			static Value LightSpecularIntensityBlinnPhong(const Value& normal, const Value& lightDir, const Value& viewDir, const Value& shininess);
			static Value LightSpecularIntensityPhong(const Value& normal, const Value& lightDir, const Value& viewDir, const Value& shininess);

			static Type Declaregl_PerVertex_t();
			static Type DeclareModelUBO_t();
			static Type DeclareViewUBO_t();
			static Type DeclarePointLight_t();
			static Type DeclareDirectionalLight_t();
			static Type DeclareLightUBO_t(const Type& PointLight_t, uint32 pointlightsCount, const Type& DirectionalLight_t, uint32 directionalLightsCount);

			static Program MakeShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);
			static Program MakeSkyboxShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);
			static Program MakeGUIShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);
			static Program MakeTextShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);
			static Program MakeRimShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);
			static Program MakeFirstLightScatteringObstacleShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);
			static Program MakeFirstLightScatteringWhiteLightShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);
			static Program MakeLastLightScatteringShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);
			static Program MakeSelectedOutlineShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);
			static Program MakeHorizontalBlurShaderProgram(MaterialInfo&, DescriptorSetLayoutInfo&);

			static void Disassemble(const std::vector<uint32>&);

			ShaderBuilder& operator=(const ShaderBuilder&) = delete;
			ShaderBuilder& operator=(ShaderBuilder&&) = delete;
		};
	}
}

#include <Awesome/Renderer/ShaderBuilder/ShaderBuilder.inl>

#endif //GUARD
