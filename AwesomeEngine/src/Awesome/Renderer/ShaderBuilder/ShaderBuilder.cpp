#include <Awesome/Renderer/ShaderBuilder/ShaderBuilder.hpp>
#include <Awesome/Renderer/ShaderBuilder/Function.hpp>
#include <Awesome/Renderer/Control/Context.hpp>
#include <Awesome/Renderer/Pipeline/VertexLayout.hpp>
#include <Awesome/Renderer/Descriptor/DescriptorSetLayoutInfo.hpp>
#include <Awesome/Renderer/Pipeline/StaticVertexAttribute.hpp>
#include <Awesome/Renderer/Memory/UniformBufferLayout.hpp>
#include <Awesome/Core/Color.hpp>

#include <SPIRV/disassemble.h>

namespace aw
{
	namespace sb
	{
		struct ShaderBuilder::BuilderContent
		{
			//spv::Builder builder{ SPV_VERSION, 0, nullptr };
			spv::Builder builder{ 0x10000, 0, nullptr };
			spv::Id glsl450Import{ 0 };

			std::unordered_map<std::string, Variable> variables;
			std::unordered_map<std::string, Type> types;
		};

		std::unique_ptr<ShaderBuilder::BuilderContent> ShaderBuilder::s_builderContent;

		void ShaderBuilder::Begin()
		{
			ASSERT(s_builderContent, "ShaderBuilder::Begin already called");

			s_builderContent = std::make_unique<ShaderBuilder::BuilderContent>();
			s_builderContent->builder.addCapability(spv::CapabilityShader);
			//s_builderContent->builder.addSourceExtension("GL_ARB_separate_shader_objects");
		}

		void ShaderBuilder::End()
		{
			s_builderContent.reset();
		}

		spv::Builder& ShaderBuilder::GetBuilder()
		{
			ASSERT(!s_builderContent, "ShaderBuilder::Begin has not been called");

			return s_builderContent->builder;
		}

		spv::Id ShaderBuilder::GetGlsl450Import()
		{
			ASSERT(!s_builderContent, "ShaderBuilder::Begin has not been called");

			if (s_builderContent->glsl450Import == 0)
				s_builderContent->glsl450Import = s_builderContent->builder.import("GLSL.std.450");
			return s_builderContent->glsl450Import;
		}

		void ShaderBuilder::WhileLoop(const std::function<Value()>& condition, const std::function<void()>& body)
		{
			using namespace spv;

			Builder::LoopBlocks &loopBlocks = GetBuilder().makeNewLoop(); // TODO implement loops
			Block &moreBlock = GetBuilder().makeNewBlock();

			GetBuilder().createBranch(&loopBlocks.head);

			GetBuilder().setBuildPoint(&loopBlocks.head);
			GetBuilder().createLoopMerge(&loopBlocks.merge, &moreBlock, LoopControlMaskNone, LoopControlDependencyInfiniteMask);
			GetBuilder().createBranch(&loopBlocks.continue_target);

			GetBuilder().setBuildPoint(&loopBlocks.continue_target);
			GetBuilder().createConditionalBranch(
				condition(), &loopBlocks.body, &loopBlocks.merge
			);

			GetBuilder().setBuildPoint(&loopBlocks.body);

			if (body)
				body();

			GetBuilder().createBranch(&moreBlock);

			GetBuilder().setBuildPoint(&moreBlock);
			GetBuilder().createBranch(&loopBlocks.head);

			GetBuilder().setBuildPoint(&loopBlocks.merge);
		}

		void ShaderBuilder::WhileLoopBegin(spv::Builder::LoopBlocks& loopBlocks, spv::Block& moreBlock, Value condition)
		{
			using namespace spv;

			//Builder::LoopBlocks &loopBlocks = GetBuilder().makeNewLoop(); // TODO implement loops
			//Block &moreBlock = GetBuilder().makeNewBlock();

			//loopBlocks = GetBuilder().makeNewLoop(); // TODO implement loops
			//moreBlock = GetBuilder().makeNewBlock();

			GetBuilder().createBranch(&loopBlocks.head);

			GetBuilder().setBuildPoint(&loopBlocks.head);
			GetBuilder().createLoopMerge(&loopBlocks.merge, &moreBlock, LoopControlMaskNone, LoopControlDependencyInfiniteMask);
			GetBuilder().createBranch(&loopBlocks.continue_target);

			GetBuilder().setBuildPoint(&loopBlocks.continue_target);
			//GetBuilder().createConditionalBranch(
			//	condition, &loopBlocks.body, &loopBlocks.merge
			//);

			//GetBuilder().setBuildPoint(&loopBlocks.body);

			//if (body)
			//	body();

			//GetBuilder().createBranch(&moreBlock);

			//GetBuilder().setBuildPoint(&moreBlock);
			//GetBuilder().createBranch(&loopBlocks.head);

			//GetBuilder().setBuildPoint(&loopBlocks.merge);
		}

		void ShaderBuilder::WhileLoopEnd(spv::Builder::LoopBlocks& loopBlocks, spv::Block& moreBlock)
		{
			using namespace spv;

			//Builder::LoopBlocks& loopBlocks = GetBuilder().makeNewLoop(); // TODO implement loops
			//Block& moreBlock = GetBuilder().makeNewBlock();

			//GetBuilder().createBranch(&loopBlocks.head);

			//GetBuilder().setBuildPoint(&loopBlocks.head);
			//GetBuilder().createLoopMerge(&loopBlocks.merge, &moreBlock, LoopControlMaskNone, LoopControlDependencyInfiniteMask);
			//GetBuilder().createBranch(&loopBlocks.continue_target);

			//GetBuilder().setBuildPoint(&loopBlocks.continue_target);
			//GetBuilder().createConditionalBranch(
			//	condition, &loopBlocks.body, &loopBlocks.merge
			//);

			//GetBuilder().setBuildPoint(&loopBlocks.body);

			//if (body)
			//	body();

			GetBuilder().createBranch(&moreBlock);

			GetBuilder().setBuildPoint(&moreBlock);
			GetBuilder().createBranch(&loopBlocks.head);

			GetBuilder().setBuildPoint(&loopBlocks.merge);
		}

		spv::Instruction* ShaderBuilder::AddEntryPointDefault(spv::ExecutionModel executionModel, Function& func, const char* name)
		{
			spv::Instruction* entryPoint = GetBuilder().addEntryPoint(executionModel, func.func, name);

			AddIdOperandIfExists(entryPoint, "inPosition");
			AddIdOperandIfExists(entryPoint, "inUV");
			AddIdOperandIfExists(entryPoint, "inUVW");
			AddIdOperandIfExists(entryPoint, "inNormal");
			AddIdOperandIfExists(entryPoint, "inTangent");
			AddIdOperandIfExists(entryPoint, "inHandedness");
			AddIdOperandIfExists(entryPoint, "inColor");

			AddIdOperandIfExists(entryPoint, "outPosition");
			AddIdOperandIfExists(entryPoint, "outUV");
			AddIdOperandIfExists(entryPoint, "outUVW");
			AddIdOperandIfExists(entryPoint, "outNormal");
			AddIdOperandIfExists(entryPoint, "outTangent");
			AddIdOperandIfExists(entryPoint, "outHandedness");
			AddIdOperandIfExists(entryPoint, "outColor");

			//if (s_builderContent->position >= 0)
			//{
			//	entryPoint->addIdOperand(s_builderContent->inputs[s_builderContent->position]);
			//	entryPoint->addIdOperand(s_builderContent->outputs[s_builderContent->position]);
			//}
			//if (s_builderContent->UV >= 0)
			//{
			//	entryPoint->addIdOperand(s_builderContent->inputs[s_builderContent->UV]);
			//	entryPoint->addIdOperand(s_builderContent->outputs[s_builderContent->UV]);
			//}
			//if (s_builderContent->normal >= 0)
			//{
			//	entryPoint->addIdOperand(s_builderContent->inputs[s_builderContent->normal]);
			//	entryPoint->addIdOperand(s_builderContent->outputs[s_builderContent->normal]);
			//}
			//if (s_builderContent->tangent >= 0)
			//{
			//	entryPoint->addIdOperand(s_builderContent->inputs[s_builderContent->tangent]);
			//	entryPoint->addIdOperand(s_builderContent->outputs[s_builderContent->tangent]);
			//}
			//if (s_builderContent->handedness >= 0)
			//{
			//	entryPoint->addIdOperand(s_builderContent->inputs[s_builderContent->handedness]);
			//	entryPoint->addIdOperand(s_builderContent->outputs[s_builderContent->handedness]);
			//}
			//if (s_builderContent->color >= 0)
			//{
			//	entryPoint->addIdOperand(s_builderContent->inputs[s_builderContent->color]);
			//	entryPoint->addIdOperand(s_builderContent->outputs[s_builderContent->color]);
			//}

			return entryPoint;
		}

		void ShaderBuilder::ProcessAttributesVertexDefault(const VertexLayout& vertexLayout, bool createOuts)
		{
			using namespace spv;

			for (uint32 i = 0; i < vertexLayout.GetAttributeCount(); i++)
			{
				if (vertexLayout.GetAttributeAt(i) == VertexAttribute(PositionXYZ))
				{
					s_builderContent->variables["inPosition"] = Variable(StorageClassInput, Type::MakeVector3f(), "inPosition");
					s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outPosition"] = Variable(
							StorageClassOutput, Type::MakeVector3f(), "outPosition"
						);
						s_builderContent->variables["outPosition"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(PositionXY))
				{
					s_builderContent->variables["inPosition"] = Variable(StorageClassInput, Type::MakeVector2f(), "inPosition");
					s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outPosition"] = Variable(
							StorageClassOutput, Type::MakeVector2f(), "outPosition"
						);
						s_builderContent->variables["outPosition"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(NormalXYZ))
				{
					s_builderContent->variables["inNormal"] = Variable(StorageClassInput, Type::MakeVector3f(), "inNormal");
					s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outNormal"] = Variable(
							StorageClassOutput, Type::MakeVector3f(), "outNormal"
						);
						s_builderContent->variables["outNormal"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(NormalXY))
				{
					s_builderContent->variables["inNormal"] = Variable(StorageClassInput, Type::MakeVector2f(), "inNormal");
					s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outNormal"] = Variable(
							StorageClassOutput, Type::MakeVector2f(), "outNormal"
						);
						s_builderContent->variables["outNormal"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TangentXYZ))
				{
					s_builderContent->variables["inTangent"] = Variable(StorageClassInput, Type::MakeVector3f(), "inTangent");
					s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outTangent"] = Variable(
							StorageClassOutput, Type::MakeVector3f(), "outTangent"
						);
						s_builderContent->variables["outTangent"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TangentXYZHandedness))
				{
					s_builderContent->variables["inTangent"] = Variable(StorageClassInput, Type::MakeVector4f(), "inTangent");
					s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outTangent"] = Variable(
							StorageClassOutput, Type::MakeVector4f(), "outTangent"
						);
						s_builderContent->variables["outTangent"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TangentXY))
				{
					s_builderContent->variables["inTangent"] = Variable(StorageClassInput, Type::MakeVector2f(), "inTangent");
					s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outTangent"] = Variable(
							StorageClassOutput, Type::MakeVector2f(), "outTangent"
						);
						s_builderContent->variables["outTangent"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TextureUVW))
				{
					s_builderContent->variables["inUVW"] = Variable(StorageClassInput, Type::MakeVector3f(), "inUVW");
					s_builderContent->variables["inUVW"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outUVW"] = Variable(
							StorageClassOutput, Type::MakeVector3f(), "outUVW"
						);
						s_builderContent->variables["outUVW"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TextureUV))
				{
					s_builderContent->variables["inUV"] = Variable(StorageClassInput, Type::MakeVector2f(), "inUV");
					s_builderContent->variables["inUV"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outUV"] = Variable(
							StorageClassOutput, Type::MakeVector2f(), "outUV"
						);
						s_builderContent->variables["outUV"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(ColorRGBA))
				{
					Type ui = Type::MakeUint();

					s_builderContent->variables["inColor"] = Variable(StorageClassInput, Type::MakeVector(ui, 4), "inColor");
					s_builderContent->variables["inColor"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outColor"] = Variable(
							StorageClassOutput, Type::MakeVector(ui, 4), "outColor"
						);
						s_builderContent->variables["outColor"].AddDecoration(DecorationLocation, i);
					}
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(Handedness))
				{
					s_builderContent->variables["inHandedness"] = Variable(StorageClassInput, Type::MakeFloat(), "inHandedness");
					s_builderContent->variables["inHandedness"].AddDecoration(DecorationLocation, i);

					if (createOuts)
					{
						s_builderContent->variables["outHandedness"] = Variable(
							StorageClassOutput, Type::MakeFloat(), "outHandedness"
						);
						s_builderContent->variables["outHandedness"].AddDecoration(DecorationLocation, i);
					}
				}
				else
				{
					THROW("Vertex attribute format not implemented yet");
				}
			}
		}

		void ShaderBuilder::ProcessAttributesFragmentDefault(const VertexLayout& vertexLayout)
		{
			using namespace spv;

			for (uint32 i = 0; i < vertexLayout.GetAttributeCount(); i++)
			{
				if (vertexLayout.GetAttributeAt(i) == VertexAttribute(PositionXYZ))
				{
					s_builderContent->variables["inPosition"] = Variable(StorageClassInput, Type::MakeVector3f(), "inPosition");
					s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(PositionXY))
				{
					s_builderContent->variables["inPosition"] = Variable(StorageClassInput, Type::MakeVector2f(), "inPosition");
					s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(NormalXYZ))
				{
					s_builderContent->variables["inNormal"] = Variable(StorageClassInput, Type::MakeVector3f(), "inNormal");
					s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(NormalXY))
				{
					s_builderContent->variables["inNormal"] = Variable(StorageClassInput, Type::MakeVector2f(), "inNormal");
					s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TangentXYZ))
				{
					s_builderContent->variables["inTangent"] = Variable(StorageClassInput, Type::MakeVector3f(), "inTangent");
					s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TangentXYZHandedness))
				{
					s_builderContent->variables["inTangent"] = Variable(StorageClassInput, Type::MakeVector4f(), "inTangent");
					s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TangentXY))
				{
					s_builderContent->variables["inTangent"] = Variable(StorageClassInput, Type::MakeVector2f(), "inTangent");
					s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TextureUVW))
				{
					s_builderContent->variables["inUVW"] = Variable(StorageClassInput, Type::MakeVector3f(), "inUVW");
					s_builderContent->variables["inUVW"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(TextureUV))
				{
					s_builderContent->variables["inUV"] = Variable(StorageClassInput, Type::MakeVector2f(), "inUV");
					s_builderContent->variables["inUV"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(ColorRGBA))
				{
					Type ui = Type::MakeUint();

					s_builderContent->variables["inColor"] = Variable(StorageClassInput, Type::MakeVector(ui, 4), "inColor");
					s_builderContent->variables["inColor"].AddDecoration(DecorationLocation, i);
				}
				else if (vertexLayout.GetAttributeAt(i) == VertexAttribute(Handedness))
				{
					s_builderContent->variables["inHandedness"] = Variable(StorageClassInput, Type::MakeFloat(), "inHandedness");
					s_builderContent->variables["inHandedness"].AddDecoration(DecorationLocation, i);
				}
				else
				{
					THROW("Vertex attribute format not implemented yet");
				}
			}
		}

		void ShaderBuilder::ProcessGl_PerVertexVertexDefault(Value& mvpMat, Value& homogenePosition, Variable& gl_PerVertex)
		{
			Variable dynmvpTimesTmpPosAccessChain = gl_PerVertex.MakeAccessChain({ 0_c });

			dynmvpTimesTmpPosAccessChain = mvpMat * homogenePosition;
		}

		void ShaderBuilder::ProcessPositionVertexDefault(Value& modelMat, Value& homogenePosition)
		{
			auto it = s_builderContent->variables.find("outPosition");
			if (it != s_builderContent->variables.end())
			{
				Variable& outPosition = it->second;

				outPosition = (modelMat * homogenePosition).XYZ();
			}
		}

		void ShaderBuilder::ProcessUVVertexDefault()
		{
			auto it1 = s_builderContent->variables.find("inUV");
			auto it2 = s_builderContent->variables.find("outUV");
			if (it1 != s_builderContent->variables.end() && it2 != s_builderContent->variables.end())
			{
				Variable& inUV = it1->second;
				Variable& outUV = it2->second;

				outUV = inUV;
			}
		}

		void ShaderBuilder::ProcessUVWVertexDefault()
		{
			auto it1 = s_builderContent->variables.find("inUVW");
			auto it2 = s_builderContent->variables.find("outUVW");
			if (it1 != s_builderContent->variables.end() && it2 != s_builderContent->variables.end())
			{
				Variable& inUVW = it1->second;
				Variable& outUVW = it2->second;

				outUVW = inUVW;
			}
		}

		void ShaderBuilder::ProcessNormalVertexDefault(Value& modelMat)
		{
			auto it1 = s_builderContent->variables.find("inNormal");
			auto it2 = s_builderContent->variables.find("outNormal");
			if (it1 != s_builderContent->variables.end() && it2 != s_builderContent->variables.end())
			{
				Variable& inNormal = it1->second;
				Variable& outNormal = it2->second;

				//*
				Type vec4f = Type::MakeVector4f();
				Value tmp = modelMat.Inverse().Transpose();
				Value vec3_1 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 0)).XYZ();
				Value vec3_2 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 1)).XYZ();
				Value vec3_3 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 2)).XYZ();
				outNormal = (Value::MakeMatrix3f(vec3_1, vec3_2, vec3_3) * inNormal).Normalize();
				//*/

				//outNormal = inNormal.Normalize();
			}
		}

		void ShaderBuilder::ProcessTangentVertexDefault(Value& modelMat)
		{
			auto it1 = s_builderContent->variables.find("inTangent");
			auto it2 = s_builderContent->variables.find("outTangent");
			if (it1 != s_builderContent->variables.end() && it2 != s_builderContent->variables.end())
			{
				Variable& inTangent = it1->second;
				Variable& outTangent = it2->second;

				Type vec4f = Type::MakeVector4f();
				Value tmp = modelMat.Inverse().Transpose();
				Value vec3_1 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 0)).XYZ();
				Value vec3_2 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 1)).XYZ();
				Value vec3_3 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 2)).XYZ();
				outTangent = (Value::MakeMatrix3f(vec3_1, vec3_2, vec3_3) * inTangent).Normalize();

				//outTangent = inTangent;
			}
		}

		void ShaderBuilder::ProcessHandednessVertexDefault()
		{
			auto it1 = s_builderContent->variables.find("inHandedness");
			auto it2 = s_builderContent->variables.find("outHandedness");
			if (it1 != s_builderContent->variables.end() && it2 != s_builderContent->variables.end())
			{
				Variable& inHandedness = it1->second;
				Variable& outHandedness = it2->second;

				outHandedness = inHandedness;
			}
		}

		void ShaderBuilder::ProcessColorVertexDefault()
		{
			auto it1 = s_builderContent->variables.find("inColor");
			auto it2 = s_builderContent->variables.find("outColor");
			if (it1 != s_builderContent->variables.end() && it2 != s_builderContent->variables.end())
			{
				Variable& inColor = it1->second;
				Variable& outColor = it2->second;

				outColor = inColor;
			}
		}

		void ShaderBuilder::AddIdOperandIfExists(spv::Instruction* instruction, const std::string& name)
		{
			auto it = s_builderContent->variables.find(name);
			if (it != s_builderContent->variables.end())
				instruction->addIdOperand(it->second);
		}

		void ShaderBuilder::ProcessUBL(const UniformBufferLayout& ubl)
		{
			uint32 size = ubl.GetSize();
			std::vector<Type> types(size);

			for (uint32 i{ 0 }; i < size; i++)
			{
				UniformBufferLayout::Member member = ubl.GetAttributeAt(i);

				switch (member.attribute)
				{
					case UniformBufferAttribute::Vec3f:
					{
						types[i] = Type::MakeVector3f();
						break;
					}

					case UniformBufferAttribute::Vec4f:
					{
						types[i] = Type::MakeVector4f();
						break;
					}

					case UniformBufferAttribute::Mat4f:
					{
						types[i] = Type::MakeMatrix4f();
						break;
					}

					case UniformBufferAttribute::Invalid:
					default:
					{
						THROW("Invalid UniformBufferAttribute");
						break;
					}
				}
			}

			Type resultType = Type::MakeStruct(types, ubl.GetName().c_str());

			/*if (ubl.IsArray())
			{
				resultType = Type::MakeArray(resultType, , );
			}*/

			s_builderContent->types[ubl.GetName()] = resultType;

			uint32 currentOffset{ 0 };

			for (uint32 i{ 0 }; i < size; i++)
			{
				UniformBufferLayout::Member member = ubl.GetAttributeAt(i);

				if (member.name == "builtInPosition" && ubl.GetName() == "gl_PerVertex_t")
				{
					resultType.AddMemberDecoration(i, spv::DecorationBuiltIn, spv::BuiltInPosition);
					continue;
				}

				switch (member.attribute)
				{
					case UniformBufferAttribute::Vec3f:
					{
						resultType.AddMemberDecoration(i, spv::DecorationOffset, currentOffset);
						currentOffset += 3 * sizeof(float);
						break;
					}

					case UniformBufferAttribute::Vec4f:
					{
						resultType.AddMemberDecoration(i, spv::DecorationOffset, currentOffset);
						currentOffset += 4 * sizeof(float);
						break;
					}

					case UniformBufferAttribute::Mat4f:
					{
						resultType.AddMemberDecoration(i, spv::DecorationColMajor);
						resultType.AddMemberDecoration(i, spv::DecorationOffset, currentOffset);
						resultType.AddMemberDecoration(i, spv::DecorationMatrixStride, 16);
						currentOffset += 16 * sizeof(float);

						break;
					}

					case UniformBufferAttribute::Invalid:
					default:
					{
						break;
					}
				}
			}
			resultType.AddDecoration(spv::DecorationBlock);
			// resultType.AddDecoration(spv::DecorationOffset, currentLayoutOffset);
		}

		Function ShaderBuilder::DeclareParallaxMappingFunc()
		{
			using namespace spv;

			Type f = Type::MakeFloat();
			Type vec2f = Type::MakeVector(f, 2);
			Type vec3f = Type::MakeVector(f, 3);
			Type image2dSampled = Type::MakeSampledImage();

			Function parallaxMappingFunc(vec2f, { vec2f, vec3f, image2dSampled, f });

			Variable texCoords = parallaxMappingFunc.GetParam(0);
			Variable viewDir = parallaxMappingFunc.GetParam(1);
			Variable displacementMap_parallax = parallaxMappingFunc.GetParam(2);
			Variable heightScale = parallaxMappingFunc.GetParam(3);
			Variable currentLayerDepth = parallaxMappingFunc.CreateVariable(f, "currentLayerDepth");
			Variable currentTexCoords = parallaxMappingFunc.CreateVariable(vec2f, "currentTexCoords");
			Variable currentDepthMapValue = parallaxMappingFunc.CreateVariable(f, "currentDepthMapValue");

			Value numLayers = Value::Mix(32.0_c, 8.0_c, Value::MakeConstant(Vector3(0.0f, 0.0f, 1.0f)).Dot(viewDir).Abs());
			Value layerDepth = 1.0_c / numLayers;
			currentLayerDepth = 0.0_c;
			Value P = viewDir.XY() * heightScale;
			Value deltaTexCoords = P / Value::MakeVector2f(numLayers);

			currentTexCoords = texCoords;
			currentDepthMapValue = 1.0_c - displacementMap_parallax.ImageSampleImplicitLod(currentTexCoords).X();

			Builder::LoopBlocks& loopBlocks = GetBuilder().makeNewLoop(); // TODO implement loops
			Block& moreBlock = GetBuilder().makeNewBlock();

			GetBuilder().createBranch(&loopBlocks.head);

			GetBuilder().setBuildPoint(&loopBlocks.head);
			GetBuilder().createLoopMerge(&loopBlocks.merge, &moreBlock, LoopControlMaskNone, LoopControlDependencyInfiniteMask);
			GetBuilder().createBranch(&loopBlocks.continue_target);

			GetBuilder().setBuildPoint(&loopBlocks.continue_target);
			GetBuilder().createConditionalBranch(currentLayerDepth.Load() < currentDepthMapValue, &loopBlocks.body, &loopBlocks.merge);

			GetBuilder().setBuildPoint(&loopBlocks.body);
			currentTexCoords -= deltaTexCoords;
			currentDepthMapValue = 1.0_c - displacementMap_parallax.ImageSampleImplicitLod(currentTexCoords).X(); // f stands for texture(...).r ie. the first component of fetched sample
			currentLayerDepth += layerDepth;
			GetBuilder().createBranch(&moreBlock);

			GetBuilder().setBuildPoint(&moreBlock);
			GetBuilder().createBranch(&loopBlocks.head);

			GetBuilder().setBuildPoint(&loopBlocks.merge);
			parallaxMappingFunc.Return(currentTexCoords.Load());
			parallaxMappingFunc.Leave();

			return parallaxMappingFunc;
		}

		void ShaderBuilder::GammaCorrect(Variable& color)
		{
			color = Value::Pow(color, Value::MakeComposite(color.GetType(), 0.4545454545_c));
		}

		Value ShaderBuilder::LightDiffuseIntensity(const Value& normal, const Value& lightDir)
		{
			return Value::Max(normal.Dot(lightDir), 0.0_c);
		}

		Value ShaderBuilder::LightSpecularIntensityBlinnPhong(const Value& normal, const Value& lightDir, const Value& viewDir, const Value& shininess)
		{
			Value halfwayDir = (lightDir + viewDir).Normalize();
			return Value::Pow(Value::Max(normal.Dot(halfwayDir), 0.0_c), shininess);
		}

		Value ShaderBuilder::LightSpecularIntensityPhong(const Value& normal, const Value& lightDir, const Value& viewDir, const Value& shininess)
		{
			Value R = Value::Reflect(-lightDir, normal);
			return Value::Pow(Value::Max(viewDir.Dot(R), 0.0_c), shininess);
		}

		Type ShaderBuilder::Declaregl_PerVertex_t()
		{
			Type vec4f = Type::MakeVector4f();

			Type gl_PerVertex_t = Type::MakeStruct({ vec4f }, "gl_PerVertex_t");

			gl_PerVertex_t.AddDecoration(spv::DecorationBlock);
			gl_PerVertex_t.AddMemberDecoration(0, spv::DecorationBuiltIn, spv::BuiltInPosition);
			gl_PerVertex_t.AddMemberDecoration(0, spv::DecorationOffset, 0);

			return gl_PerVertex_t;
		}

		Type ShaderBuilder::DeclareModelUBO_t()
		{
			Type f = Type::MakeFloat();
			Type vec2f = Type::MakeVector(f, 2);
			Type vec4f = Type::MakeVector(f, 4);
			Type mat4f = Type::MakeMatrix(f, 4, 4);
			Type ui = Type::MakeUint();

			Type ModelUBO_t = Type::MakeStruct({ mat4f, mat4f, ui, ui, vec4f, vec4f, f, f, vec2f, vec2f }, "ModelUBO_t");

			ModelUBO_t.AddMemberDecoration(0, spv::DecorationColMajor);
			ModelUBO_t.AddMemberDecoration(0, spv::DecorationOffset, 0);
			ModelUBO_t.AddMemberDecoration(0, spv::DecorationMatrixStride, 16);
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 0, "mvp");

			ModelUBO_t.AddMemberDecoration(1, spv::DecorationColMajor);
			ModelUBO_t.AddMemberDecoration(1, spv::DecorationOffset, 64);
			ModelUBO_t.AddMemberDecoration(1, spv::DecorationMatrixStride, 16);
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 1, "model");

			ModelUBO_t.AddMemberDecoration(2, spv::DecorationOffset, 128);
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 2, "componentId");

			ModelUBO_t.AddMemberDecoration(3, spv::DecorationOffset, 132);
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 3, "actorId");

			ModelUBO_t.AddMemberDecoration(4, spv::DecorationOffset, 136);
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 4, "baseColorFactor");

			ModelUBO_t.AddMemberDecoration(5, spv::DecorationOffset, 136 + 4 * sizeof(float));
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 5, "emissiveFactor");

			ModelUBO_t.AddMemberDecoration(6, spv::DecorationOffset, 136 + 4 * sizeof(float) * 2);
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 6, "metallicFactor");

			ModelUBO_t.AddMemberDecoration(7, spv::DecorationOffset, 136 + 4 * sizeof(float) * 2 + sizeof(float));
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 7, "roughnessFactor");

			ModelUBO_t.AddMemberDecoration(8, spv::DecorationOffset, 136 + 4 * sizeof(float) * 2 + sizeof(float) * 2);
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 8, "scaleUV");

			ModelUBO_t.AddMemberDecoration(9, spv::DecorationOffset, 136 + 4 * sizeof(float) * 2 + sizeof(float) * 2 + sizeof(float) * 2);
			ShaderBuilder::GetBuilder().addMemberName(ModelUBO_t, 9, "offsetUV");

			ModelUBO_t.AddDecoration(spv::DecorationBlock);

			return ModelUBO_t;
		}

		Type ShaderBuilder::DeclareViewUBO_t()
		{
			Type f = Type::MakeFloat();
			Type vec3f = Type::MakeVector(f, 3);
			Type mat4f = Type::MakeMatrix(f, 4, 4);

			Type ViewUBO_t = Type::MakeStruct({ mat4f, vec3f }, "ViewUBO_t");

			ViewUBO_t.AddDecoration(spv::DecorationBlock);

			ViewUBO_t.AddMemberDecoration(0, spv::DecorationColMajor);
			ViewUBO_t.AddMemberDecoration(0, spv::DecorationOffset, 0);
			ViewUBO_t.AddMemberDecoration(0, spv::DecorationMatrixStride, 16);

			ViewUBO_t.AddMemberDecoration(1, spv::DecorationOffset, 16 * sizeof(float));

			return ViewUBO_t;
		}

		Type ShaderBuilder::DeclarePointLight_t()
		{
			Type vec4f = Type::MakeVector4f();

			Type PointLight_t = Type::MakeStruct({ vec4f, vec4f }, "PointLight_t");

			PointLight_t.AddMemberDecoration(0, spv::DecorationOffset, 0);
			ShaderBuilder::GetBuilder().addMemberName(PointLight_t, 0, "color");

			PointLight_t.AddMemberDecoration(1, spv::DecorationOffset, 4 * sizeof(float));
			ShaderBuilder::GetBuilder().addMemberName(PointLight_t, 1, "positionIntensity");

			return PointLight_t;
		}

		Type ShaderBuilder::DeclareDirectionalLight_t()
		{
			Type vec4f = Type::MakeVector4f();

			Type DirectionalLight_t = Type::MakeStruct({ vec4f, vec4f }, "DirectionalLight_t");

			DirectionalLight_t.AddMemberDecoration(0, spv::DecorationOffset, 0);
			ShaderBuilder::GetBuilder().addMemberName(DirectionalLight_t, 0, "color");

			DirectionalLight_t.AddMemberDecoration(1, spv::DecorationOffset, 4 * sizeof(float));
			ShaderBuilder::GetBuilder().addMemberName(DirectionalLight_t, 1, "directionIntensity");

			return DirectionalLight_t;
		}

		Type ShaderBuilder::DeclareLightUBO_t(const Type& PointLight_t, uint32 pointlightsCount, const Type& DirectionalLight_t, uint32 directionalLightsCount)
		{
			Type ui = Type::MakeUint();
			Type pointLightArr = Type::MakeArray(PointLight_t, Value::MakeConstant(pointlightsCount), 2 * 4 * sizeof(float));
			Type directionalLightArr = Type::MakeArray(DirectionalLight_t, Value::MakeConstant(directionalLightsCount), 2 * 4 * sizeof(float));

			Type LightUBO_t = Type::MakeStruct({ pointLightArr, ui, directionalLightArr, ui }, "LightUBO_t");

			LightUBO_t.AddMemberDecoration(0, spv::DecorationOffset, 0);
			LightUBO_t.AddMemberDecoration(0, spv::DecorationArrayStride, 2 * 4 * sizeof(float));
			ShaderBuilder::GetBuilder().addMemberName(LightUBO_t, 0, "pointLights");

			LightUBO_t.AddMemberDecoration(1, spv::DecorationOffset, pointlightsCount * 2 * 4 * sizeof(float));
			ShaderBuilder::GetBuilder().addMemberName(LightUBO_t, 1, "pointLightCount");

			LightUBO_t.AddMemberDecoration(2, spv::DecorationOffset, pointlightsCount * 2 * 4 * sizeof(float) + sizeof(pointlightsCount));
			LightUBO_t.AddMemberDecoration(2, spv::DecorationArrayStride, 2 * 4 * sizeof(float));
			ShaderBuilder::GetBuilder().addMemberName(LightUBO_t, 2, "directionalLights");

			LightUBO_t.AddMemberDecoration(3, spv::DecorationOffset, (pointlightsCount * 2 * 4 * sizeof(float)) + sizeof(pointlightsCount) + (directionalLightsCount * 2 * 4 * sizeof(float)));
			ShaderBuilder::GetBuilder().addMemberName(LightUBO_t, 3, "directionalLightCount");

			LightUBO_t.AddDecoration(spv::DecorationBlock);

			return LightUBO_t;
		}

		Program ShaderBuilder::MakeShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			bool usesPosition = false;
			bool usesNormal = false;
			bool usesTangent = false;
			bool usesHandedness = false;
			bool usesUV = false;
			bool usesColor = false;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type ui = Type::MakeUint();
				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec2ui = Type::MakeVector(ui, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);
				Type ViewUBO_t = DeclareViewUBO_t();
				Type PointLight_t = DeclarePointLight_t();
				Type DirectionalLight_t = DeclareDirectionalLight_t();
				Type LightUBO_t = DeclareLightUBO_t(PointLight_t, 10, DirectionalLight_t, 10);

				Type image2dSampled = Type::MakeSampledImage();
				Type imageCubeMap = Type::MakeImage(f, spv::DimCube);
				Type imageCubeMapSampled = Type::MakeSampledImage(imageCubeMap);

				sb::Function parallaxFunc;
				if (materialInfo.hasHeightMap)
					parallaxFunc = DeclareParallaxMappingFunc();

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Variable outCompId(StorageClassOutput, vec2ui, "outCompId");
				outCompId.AddDecoration(DecorationLocation, 1);

				dsetInfo.AddBinding(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment); // view
				dsetInfo.AddBinding(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment); // lights

				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // diffuseMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // normalMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // specularMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // heightMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // environmentMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // reflectionMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // refrectionMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // emissive
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // occlusion
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // metalicRoughness

				Type ModelUBO_t = DeclareModelUBO_t();
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				Variable view = Variable::CreateUniform(ViewUBO_t, 0, 1, "view");
				materialInfo.hasViewUBO = true;
				Variable lights = Variable::CreateUniform(LightUBO_t, 1, 1, "lights");
				materialInfo.hasLightUBO = true;

				Variable diffuseMap = Variable::CreateUniformConstant(image2dSampled, 2, 1, "diffuseMap");
				Variable normalMap = Variable::CreateUniformConstant(image2dSampled, 3, 1, "normalMap");
				Variable specularMap = Variable::CreateUniformConstant(image2dSampled, 4, 1, "specularMap");
				Variable displacementMap = Variable::CreateUniformConstant(image2dSampled, 5, 1, "displacementMap");
				Variable environmentMap = Variable::CreateUniformConstant(imageCubeMapSampled, 6, 1, "environmentMap");
				Variable reflectionMap = Variable::CreateUniformConstant(image2dSampled, 7, 1, "reflectionMap");
				Variable refractionMap = Variable::CreateUniformConstant(image2dSampled, 8, 1, "refractionMap");
				Variable emissiveMap = Variable::CreateUniformConstant(image2dSampled, 9, 1, "emissive");
				Variable occlusionMap = Variable::CreateUniformConstant(image2dSampled, 10, 1, "occlusion");
				Variable metalicRoughnessMap = Variable::CreateUniformConstant(image2dSampled, 11, 1, "metalicRoughness");

				Instruction *entryPoint = GetBuilder().addEntryPoint(ExecutionModelFragment, mainFunc.func, "main");
				entryPoint->addIdOperand(outColor);
				entryPoint->addIdOperand(outCompId);

				Variable UBOComponentId = dyn.MakeAccessChain({ 2_c });
				Variable UBOActorId = dyn.MakeAccessChain({ 3_c });
				outCompId.MakeAccessChain({ 0_c }) = UBOComponentId;
				outCompId.MakeAccessChain({ 1_c }) = UBOActorId;

				Variable UBOBaseColorFactor = dyn.MakeAccessChain({ 4_c });

				Value N;
				if (materialInfo.hasNormalMap || materialInfo.receiveLights || materialInfo.hasReflectionMap ||
					materialInfo.hasRefractionMap || materialInfo.hasMetalicRoughnessMap)
				{
					Variable inNormal(StorageClassInput, vec3f, "inNormal");
					inNormal.AddDecoration(DecorationLocation, 2);
					entryPoint->addIdOperand(inNormal);
					usesNormal = true;

					//Value tmpN = inNormal;
					//Value tmpN = Value::MakeVector3f(inNormal.X(), -inNormal.Y(), inNormal.Z());
					//N = tmpN.Normalize();

					N = inNormal.Normalize();
				}

				Value V;
				Value inPosition;
				if (materialInfo.hasHeightMap || materialInfo.receiveLights || materialInfo.hasReflectionMap ||
					materialInfo.hasRefractionMap || materialInfo.hasMetalicRoughnessMap)
				{
					Variable varInPosition(StorageClassInput, vec3f, "inPosition");
					varInPosition.AddDecoration(DecorationLocation, 0);
					entryPoint->addIdOperand(varInPosition);
					usesPosition = true;
					inPosition = varInPosition;

					V = (view.MakeAccessChain({ 1_c }) - inPosition).Normalize();
				}

				Value UV;
				if (materialInfo.hasHeightMap)
				{
					usesUV = true;
					Variable inUV(StorageClassInput, vec2f, "inUV");
					inUV.AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(inUV);

					UV = inUV;
					UV = UV + dyn.MakeAccessChain({ 9_c });
					UV = UV * dyn.MakeAccessChain({ 8_c });

					UV = parallaxFunc({ UV, V, displacementMap, 0.01_c });
				}
				else if (materialInfo.HasMap())
				{
					usesUV = true;

					Variable inUV(StorageClassInput, vec2f, "inUV");
					inUV.AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(inUV);

					UV = inUV;
					UV = UV + dyn.MakeAccessChain({ 9_c });
					UV = UV * dyn.MakeAccessChain({ 8_c });
				}

				Value TBN;
				Value T;
				if (materialInfo.hasNormalMap)
				{
					Variable inTangent(StorageClassInput, vec3f, "inTangent");
					inTangent.AddDecoration(DecorationLocation, 3);
					entryPoint->addIdOperand(inTangent);
					usesTangent = true;
					Variable inHandedness(StorageClassInput, f, "inHandedness");
					inHandedness.AddDecoration(DecorationLocation, 4);
					entryPoint->addIdOperand(inHandedness);
					usesHandedness = true;

					//Value scaledT = inTangent * 2.0_c - Value::MakeConstant(Vector3(1, 1, 1));
					//T = scaledT.Normalize();

					T = inTangent.Normalize();
					Value B = (inHandedness * N.Cross(T)).Normalize();
					TBN = Value::MakeMatrix3f(T, B, N);

					N = TBN * (normalMap.ImageSampleImplicitLod(UV).XYZ() * 2.0_c - Value::MakeConstant(Vector3(1.0f, 1.0f, 1.0f))).Normalize();
				}

				Value diffuseSample;
				if (materialInfo.hasDiffuseMap)
				{
					usesUV = true;
					diffuseSample = diffuseMap.ImageSampleImplicitLod(UV);

					if (materialInfo.isTransparent)
					{
						Block& discard = GetBuilder().makeNewBlock();
						Block& noDiscard = GetBuilder().makeNewBlock();

						GetBuilder().createConditionalBranch(diffuseSample.W() <= 0.0_c, &discard, &noDiscard);

						GetBuilder().setBuildPoint(&discard);
						GetBuilder().makeDiscard();
						GetBuilder().setBuildPoint(&noDiscard);
					}
				}
				else
					diffuseSample = UBOBaseColorFactor;

				Value specularSample;
				if (materialInfo.hasSpecularMap)
				{
					specularSample = specularMap.ImageSampleImplicitLod(UV).X();
				}
				//else
				//	specularSample = 1.0_c;

				Value Ka = 0.05_c;
				Value Kd = 0.2_c;
				Value Ks = 0.75_c;

				if (false && materialInfo.hasMetalicRoughnessMap && materialInfo.receiveLights)
				// if (materialInfo.receiveLights)
				{
					Value dielectricSpecular = Value::MakeConstant(Vector3(0.04f, 0.04f, 0.04f));
					Value black = Value::MakeConstant(Vector3(0.0f, 0.0f, 0.0f));
					Value oneVec3 = Value::MakeConstant(Vector3(1.0f, 1.0f, 1.0f));

					Value diffuseSampleRGB = diffuseSample.XYZ();
					Value metallic = dyn.MakeAccessChain({ 6_c });
					Value roughness = dyn.MakeAccessChain({ 7_c });
					if (materialInfo.hasMetalicRoughnessMap)
					{
						Value metallicRoughnessSampled = metalicRoughnessMap.ImageSampleImplicitLod(UV);
						metallic = metallic * metallicRoughnessSampled.Z();
						roughness = roughness * metallicRoughnessSampled.Y();
					}
					Value metallicVec3 = Value::MakeVector3f(metallic);

					//Value Cdiff = Value::Mix(diffuseSampleRGB * 0.96_c, black, metallicVec3);
					//Value Cdiff = diffuseSampleRGB;
					Value F0 = Value::Mix(dielectricSpecular, diffuseSampleRGB, metallicVec3);

					Variable light(StorageClassFunction, PointLight_t, "light");
					Variable directionalLight(StorageClassFunction, DirectionalLight_t, "directionalLight");
					Variable lightPosIntensity(StorageClassFunction, vec4f, "lightPosIntensity");
					Variable lightPos(StorageClassFunction, vec3f, "lightPos");
					Variable lightIntensity(StorageClassFunction, f, "lightIntensity");
					Variable lightColor(StorageClassFunction, vec4f, "lightColor");
					Variable L(StorageClassFunction, vec3f, "L");
					Variable H(StorageClassFunction, vec3f, "H");
					Variable radiance(StorageClassFunction, vec3f, "radiance");
					Variable distance(StorageClassFunction, f, "distance");
					Variable attenuation(StorageClassFunction, f, "attenuation");
					Value pointLightCount = lights.MakeAccessChain({ 1_c });
					Value direcionalLightCount = lights.MakeAccessChain({ 3_c });

					Variable NdotL(StorageClassFunction, f, "NdotL");
					Variable NdotV(StorageClassFunction, f, "NdotV");
					Variable NdotH(StorageClassFunction, f, "NdotH");
					Variable VdotH(StorageClassFunction, f, "VdotH");
					Variable LdotH(StorageClassFunction, f, "LdotH");
					Variable LdotV(StorageClassFunction, f, "LdotV");

					Variable NdotH2(StorageClassFunction, f, "NdotH2");
					Variable Ddenom(StorageClassFunction, f, "Ddenom");

					Variable loopCounter(StorageClassFunction, ui, "loopCounter");
					loopCounter = Value::MakeConstant(0u);

					Variable kD(StorageClassFunction, vec3f, "kD");
					Variable kS(StorageClassFunction, vec3f, "kS");

					Variable specular(StorageClassFunction, vec3f, "specular");
					Variable Lo(StorageClassFunction, vec3f, "Lo");
					Variable F(StorageClassFunction, vec3f, "F");
					Variable D(StorageClassFunction, f, "D");
					Variable G(StorageClassFunction, f, "G");

					Lo = Value::MakeVector3f(0.0_c);

					auto GeometrySchlickGGX = [](Value NdotV, Value k) -> Value
					{
						return NdotV / (NdotV * (1.0_c - k) + k);
					};

					Value aplus1 = roughness + 1.0_c;
					Value k = (aplus1 * aplus1) / 8.0_c;
					Value a = roughness * roughness;
					Value a2 = a * a;

					WhileLoop([&]()
					{
						return loopCounter.Load() < pointLightCount;
					}, [&]()
					{
						light = lights.MakeAccessChain({ 0_c, loopCounter.Load() });
						lightPosIntensity = light.MakeAccessChain({ 1_c });
						lightPos = lightPosIntensity.XYZ();
						lightIntensity = lightPosIntensity.W();
						lightColor = light.MakeAccessChain({ 0_c });
						L = (lightPos.Load() - inPosition).Normalize();
						H = (L.Load() + V).Normalize();

						NdotL = Value::Max(N.Dot(L.Load()), 0.0_c);
						NdotV = Value::Max(N.Dot(V), 0.0_c); // TODO optimize
						NdotH = Value::Max(N.Dot(H.Load()), 0.0_c);
						VdotH = Value::Max(V.Dot(H.Load()), 0.0_c);
						LdotH = Value::Max(L.Load().Dot(H.Load()), 0.0_c);
						LdotV = Value::Max(L.Load().Dot(V), 0.0_c);

						distance = (lightPos.Load() - inPosition).Length();
						radiance = lightColor.Load().XYZ() / (distance.Load() * distance.Load());

						// Fresnel
						F = F0 + (1.0_c - F0) * Value::Pow(1.0_c - NdotV.Load(), 5.0_c);
						//Value F = F0 + (1.0_c - F0) * Value::Pow(1.0_c - VdotH, 5.0_c);
						// ====================

						// Geometry Smith Schlick-GGX
						G = GeometrySchlickGGX(NdotV.Load(), k) * GeometrySchlickGGX(NdotL.Load(), k);
						// ====================

						// NDF
						NdotH2 = NdotH.Load() * NdotH.Load();
						Ddenom = NdotH2.Load() * (a2 - 1.0_c) + 1.0_c;
						Ddenom = 3.14159265358979_c * Ddenom.Load() * Ddenom.Load();
						D = a2 / Value::Max(Ddenom.Load(), 0.001_c);
						// ====================

						kS = F.Load();
						kD = (1.0_c - kS.Load()) * (1.0_c - metallic);

						specular = ((D.Load() * G.Load()) * F.Load()) / (Value::Max(4.0_c * NdotV.Load() * NdotL.Load(), 0.001_c));

						Lo = Lo.Load() + (kD.Load() * diffuseSampleRGB / 3.14159265358979_c + specular.Load()) * radiance.Load() * NdotL.Load();
						//Lo = Lo.Load() + (kD.Load() * diffuseSampleRGB / 3.14159265358979_c + specular.Load()) * radiance.Load();
						//Lo = Lo.Load() + (kD.Load() + specular.Load()) * radiance.Load();

						loopCounter += Value::MakeConstant(1u);
					});

					loopCounter = Value::MakeConstant(0u);

					WhileLoop([&]()
					{
						return loopCounter.Load() < direcionalLightCount;
					}, [&]()
					{
						// directionalLight = lights.MakeAccessChain({ 2_c, loopCounter.Load() });
						// lightPosIntensity = directionalLight.MakeAccessChain({ 1_c });
						// lightPos = lightPosIntensity.XYZ();
						// lightIntensity = lightPosIntensity.W();
						// lightColor = directionalLight.MakeAccessChain({ 0_c });
						// Ld += (lightColor.Load()) * LightDiffuseIntensity(N, L.Load());
						// Ls += lightColor.Load() * LightSpecularIntensityBlinnPhong(N, L, V, lightIntensity.Load());
						// loopCounter += Value::MakeConstant(1u);

						directionalLight = lights.MakeAccessChain({ 2_c, loopCounter.Load() });
						lightPosIntensity = directionalLight.MakeAccessChain({ 1_c });
						lightPos = lightPosIntensity.XYZ();
						lightIntensity = lightPosIntensity.W();
						lightColor = directionalLight.MakeAccessChain({ 0_c });
						L = lightPos.Normalize();
						H = (L.Load() + V).Normalize();

						NdotL = Value::Max(N.Dot(L.Load()), 0.0_c);
						NdotV = Value::Max(N.Dot(V), 0.0_c); // TODO optimize
						NdotH = Value::Max(N.Dot(H.Load()), 0.0_c);
						VdotH = Value::Max(V.Dot(H.Load()), 0.0_c);
						LdotH = Value::Max(L.Load().Dot(H.Load()), 0.0_c);
						LdotV = Value::Max(L.Load().Dot(V), 0.0_c);

						// distance = (lightPos.Load() - inPosition).Length();
						// radiance = lightColor.Load().XYZ() / (distance.Load() * distance.Load());

						// Fresnel
						F = F0 + (1.0_c - F0) * Value::Pow(1.0_c - NdotV.Load(), 5.0_c);
						//Value F = F0 + (1.0_c - F0) * Value::Pow(1.0_c - VdotH, 5.0_c);
						// ====================

						// Geometry Smith Schlick-GGX
						G = GeometrySchlickGGX(NdotV.Load(), k) * GeometrySchlickGGX(NdotL.Load(), k);
						// ====================

						// NDF
						NdotH2 = NdotH.Load() * NdotH.Load();
						Ddenom = NdotH2.Load() * (a2 - 1.0_c) + 1.0_c;
						Ddenom = 3.14159265358979_c * Ddenom.Load() * Ddenom.Load();
						D = a2 / Value::Max(Ddenom.Load(), 0.001_c);
						// ====================

						kS = F.Load();
						kD = (1.0_c - kS.Load()) * (1.0_c - metallic);

						specular = ((D.Load() * G.Load()) * F.Load()) / (Value::Max(4.0_c * NdotV.Load() * NdotL.Load(), 0.001_c));

						Lo = Lo.Load() + (kD.Load() * diffuseSampleRGB / 3.14159265358979_c + specular.Load()) /** radiance.Load()*/ * NdotL.Load();
						//Lo = Lo.Load() + (kD.Load() * diffuseSampleRGB / 3.14159265358979_c + specular.Load()) * radiance.Load();
						//Lo = Lo.Load() + (kD.Load() + specular.Load()) * radiance.Load();

						loopCounter += Value::MakeConstant(1u);
					});


					//Value diffuse = (1.0_c - F) * (1.0_c - metallic);

					//Value fLambert = Cdiff / 3.14159265358979_c;

					//Value divider = Value::Max(4.0_c * NdotL * NdotV, 0.001_c);

					//Value fCookTorrance = (D * G) * F;

					Value ambient = 0.03_c * diffuseSampleRGB;

					if (materialInfo.hasOcclusionMap)
					{
						ambient = ambient * occlusionMap.ImageSampleImplicitLod(UV).X();
					}

					Value resultColor = ambient + Lo.Load();
					resultColor = resultColor / (resultColor + 1.0_c);
					outColor = Value::MakeVector4f(resultColor.X(), resultColor.Y(), resultColor.Z(), 1.0_c);
				}
				else if (materialInfo.receiveLights)
				{
					Variable pointLight(StorageClassFunction, PointLight_t, "pointLight");
					Variable directionalLight(StorageClassFunction, DirectionalLight_t, "directionalLight");
					Variable lightPosIntensity(StorageClassFunction, vec4f, "lightPosIntensity");
					Variable lightPos(StorageClassFunction, vec3f, "lightPos");
					Variable lightIntensity(StorageClassFunction, f, "lightIntensity");
					Variable lightColor(StorageClassFunction, vec4f, "lightColor");
					Variable distance(StorageClassFunction, f, "distance");
					Variable attenuation(StorageClassFunction, f, "attenuation");
					Variable L(StorageClassFunction, vec3f, "L");
					Value pointLightCount = lights.MakeAccessChain({ 1_c });
					Value direcionalLightCount = lights.MakeAccessChain({ 3_c });
					Variable loopCounter(StorageClassFunction, ui, "loopCounter");
					loopCounter = Value::MakeConstant(0u);

					Variable Ld(StorageClassFunction, vec4f, "Ld");
					Variable Ls(StorageClassFunction, vec4f, "Ls");

					Ld = Value::MakeVector4f(0.0_c);
					Ls = Value::MakeVector4f(0.0_c);

					WhileLoop([&]()
					{
						return loopCounter.Load() < pointLightCount;
					}, [&]()
					{
						pointLight = lights.MakeAccessChain({ 0_c, loopCounter.Load() });
						lightPosIntensity = pointLight.MakeAccessChain({ 1_c });
						lightPos = lightPosIntensity.XYZ();
						lightIntensity = lightPosIntensity.W();
						distance = (lightPos.Load() - inPosition).Length();
						attenuation = 1.0_c / (distance.Load() * distance.Load());
						lightColor = pointLight.MakeAccessChain({ 0_c }) * attenuation.Load() / 300.0_c;
						L = (lightPos - inPosition).Normalize();
						Ld += (lightColor.Load()) * LightDiffuseIntensity(N, L.Load());
						Ls += lightColor.Load() * LightSpecularIntensityBlinnPhong(N, L, V, lightIntensity.Load());
						loopCounter += Value::MakeConstant(1u);
					});

					loopCounter = Value::MakeConstant(0u);

					WhileLoop([&]()
					{
						return loopCounter.Load() < direcionalLightCount;
					}, [&]()
					{
						directionalLight = lights.MakeAccessChain({ 2_c, loopCounter.Load() });
						lightPosIntensity = directionalLight.MakeAccessChain({ 1_c });
						lightPos = lightPosIntensity.XYZ();
						lightIntensity = lightPosIntensity.W();
						lightColor = directionalLight.MakeAccessChain({ 0_c }) / 300.0_c;
						L = lightPos.Normalize();
						Ld += (lightColor.Load()) * LightDiffuseIntensity(N, L.Load());
						Ls += lightColor.Load() * LightSpecularIntensityBlinnPhong(N, L, V, lightIntensity.Load());
						loopCounter += Value::MakeConstant(1u);
					});

					Value ambientColor = diffuseSample * Ka;
					Value diffuseColor = diffuseSample * Ld.Load() * Kd;
					Value resultColor;

					if (materialInfo.hasOcclusionMap)
					{
						ambientColor = ambientColor * occlusionMap.ImageSampleImplicitLod(UV).X();
					}

					if (specularSample.id != 0)
					{
						Value specularColor = specularSample * Ls.Load() * Ks;

						resultColor = ambientColor + diffuseColor + specularColor;
					}
					else
						resultColor = ambientColor + diffuseColor;
					resultColor = resultColor / (resultColor + 1.0_c);

					outColor = Value::MakeVector4f(resultColor.X(), resultColor.Y(), resultColor.Z(), 1.0_c);
					//outColor = outColor.Shuffle(ambientColor + diffuseColor + specularColor, 4, 5, 6, 7);
				}
				else
				{
					if (materialInfo.hasSpecularMap)
					{
						Value ambientColor = diffuseSample * Ka;
						Value diffuseColor = diffuseSample * Kd;
						Value specularColor = Value::MakeVector4f(specularSample) * Ks;

						outColor = outColor.Shuffle(ambientColor + diffuseColor + specularColor, 4, 5, 6, 7);
					}
					else
						outColor = outColor.Shuffle(diffuseSample, 4, 5, 6, 7);
				}
				//*/

				if (materialInfo.correctGamma)
					GammaCorrect(outColor);

				if (materialInfo.hasEnvironmentMap)
				{
					if (materialInfo.hasReflectionMap && !materialInfo.hasRefractionMap)
					{
						Value I = (inPosition - view.MakeAccessChain({ 1_c })).Normalize();
						Value R = Value::Reflect(I, N);
						Value environmentSample = environmentMap.ImageSampleImplicitLod(R);
						Value reflectionSample = reflectionMap.ImageSampleImplicitLod(UV).X();

						outColor = Value::Mix(outColor, environmentSample, Value::MakeVector4f(reflectionSample));
					}
					else if (materialInfo.hasRefractionMap && !materialInfo.hasReflectionMap)
					{
						Value I = (inPosition - view.MakeAccessChain({ 1_c })).Normalize();
						// Value R = Value::Refract(I, N, 0.95_c);
						Value R = Value::Refract(I, N, 1.0_c);
						Value environmentSample = environmentMap.ImageSampleImplicitLod(R);
						Value refractionSample = refractionMap.ImageSampleImplicitLod(UV).X();

						outColor = Value::Mix(outColor, environmentSample, Value::MakeVector4f(refractionSample));
					}
					else if (materialInfo.hasRefractionMap && materialInfo.hasReflectionMap)
					{
						Value I = (inPosition - view.MakeAccessChain({ 1_c })).Normalize();
						Value viewReflection = Value::Reflect(I, N);

						Value reflectedSample = environmentMap.ImageSampleImplicitLod(viewReflection);
						Value refractedSample = environmentMap.ImageSampleImplicitLod(Value::Refract(I, N, 0.95_c));

						Value reflectionSample = reflectionMap.ImageSampleImplicitLod(UV).X();
						Value refractionSample = refractionMap.ImageSampleImplicitLod(UV).X();

						Value reflectionColor = Value::Mix(outColor, reflectedSample, Value::MakeVector4f(reflectionSample));
						Value refractionColor = Value::Mix(outColor, refractedSample, Value::MakeVector4f(refractionSample));

						Value fresnel = N.Dot(viewReflection);

						outColor = Value::Mix(reflectionColor, refractionColor, Value::MakeVector4f(fresnel));
					}
				}

				//if (materialInfo.hasOcclusionMap && !materialInfo.hasMetalicRoughnessMap)
				//{
				//	outColor = outColor * occlusionMap.ImageSampleImplicitLod(UV).X();
				//}

				Value emissiveSample = dyn.MakeAccessChain({ 5_c });
				if (materialInfo.hasEmissiveMap)
					emissiveSample = emissiveSample * emissiveMap.ImageSampleImplicitLod(UV);
				Variable outColorR = outColor.MakeAccessChain({ 0_c });
				Variable outColorG = outColor.MakeAccessChain({ 1_c });
				Variable outColorB = outColor.MakeAccessChain({ 2_c });
				outColorR = outColorR + emissiveSample.X();
				outColorG = outColorG + emissiveSample.Y();
				outColorB = outColorB + emissiveSample.Z();

				Variable outColorA = outColor.MakeAccessChain({ 3_c });
				outColorA = diffuseSample.W();

				/*if (usesNormal == true)
				{
					Variable outColorR = outColor.MakeAccessChain({ 0_c });
					Variable outColorG = outColor.MakeAccessChain({ 1_c });
					Variable outColorB = outColor.MakeAccessChain({ 2_c });
					outColorR = N.X() * 0.5_c + 0.5_c;
					outColorG = N.Y() * 0.5_c + 0.5_c;
					outColorB = N.Z() * 0.5_c + 0.5_c;
				}*/

				//{
				//	Variable inColor(StorageClassInput, vec4f, "inColor");
				//	inColor.AddDecoration(DecorationLocation, 5);
				//	entryPoint->addIdOperand(inColor);
				//	usesColor = true;
				//	outColor = inColor;
				//}

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				//spv::Disassemble(std::cout, shaderCode);
				//std::cout << "Fragment" << std::endl;

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type ui = Type::MakeUint();
				//Type ui64 = Type::MakeUint64();
				//Type ui32 = ShaderBuilder::GetBuilder().makeUintType(32);
				Type vec2f = Type::MakeVector(f, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);
				//Type vec4f = Type::MakeVector(f, 4);
				//Type mat4f = Type::MakeMatrix(f, 4, 4);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				Type ModelUBO_t = DeclareModelUBO_t();

				s_builderContent->variables["inPosition"] = Variable(StorageClassInput, vec3f, "inPosition");
				s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, 0);

				s_builderContent->variables["inUV"] = Variable(StorageClassInput, vec2f, "inUV");
				s_builderContent->variables["inUV"].AddDecoration(DecorationLocation, 1);

				s_builderContent->variables["inNormal"] = Variable(StorageClassInput, vec3f, "inNormal");
				s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, 2);

				s_builderContent->variables["inTangent"] = Variable(StorageClassInput, vec3f, "inTangent");
				s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, 3);

				s_builderContent->variables["inHandedness"] = Variable(StorageClassInput, f, "inHandedness");
				s_builderContent->variables["inHandedness"].AddDecoration(DecorationLocation, 4);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(gl_PerVertex);
				entryPoint->addIdOperand(s_builderContent->variables["inPosition"]);
				entryPoint->addIdOperand(s_builderContent->variables["inUV"]);
				entryPoint->addIdOperand(s_builderContent->variables["inNormal"]);
				entryPoint->addIdOperand(s_builderContent->variables["inTangent"]);
				entryPoint->addIdOperand(s_builderContent->variables["inHandedness"]);

				Value modelMat;
				if (usesPosition || usesNormal || usesTangent)
					modelMat = dyn.MakeAccessChain({ 1_c });

				Value mvpMat = dyn.MakeAccessChain({ 0_c });
				Variable& inPosition = s_builderContent->variables["inPosition"];
				Value homogenePosition = Value::MakeVector4f(inPosition.X(), inPosition.Y(), inPosition.Z(), 1.0_c);
				ProcessGl_PerVertexVertexDefault(mvpMat, homogenePosition, gl_PerVertex);

				if (usesPosition)
				{
					s_builderContent->variables["outPosition"] = Variable(StorageClassOutput, vec3f, "outPosition");
					s_builderContent->variables["outPosition"].AddDecoration(DecorationLocation, 0);
					entryPoint->addIdOperand(s_builderContent->variables["outPosition"]);

					ProcessPositionVertexDefault(modelMat, homogenePosition);
				}
				if (usesUV)
				{
					s_builderContent->variables["outUV"] = Variable(StorageClassOutput, vec2f, "outUV");
					s_builderContent->variables["outUV"].AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(s_builderContent->variables["outUV"]);

					ProcessUVVertexDefault();
				}
				if (usesNormal)
				{
					s_builderContent->variables["outNormal"] = Variable(StorageClassOutput, vec3f, "outNormal");
					s_builderContent->variables["outNormal"].AddDecoration(DecorationLocation, 2);
					entryPoint->addIdOperand(s_builderContent->variables["outNormal"]);

					ProcessNormalVertexDefault(modelMat);
				}
				if (usesTangent)
				{
					s_builderContent->variables["outTangent"] = Variable(StorageClassOutput, vec3f, "outTangent");
					s_builderContent->variables["outTangent"].AddDecoration(DecorationLocation, 3);
					entryPoint->addIdOperand(s_builderContent->variables["outTangent"]);

					ProcessTangentVertexDefault(modelMat);
				}
				if (usesHandedness)
				{
					s_builderContent->variables["outHandedness"] = Variable(StorageClassOutput, f, "outHandedness");
					s_builderContent->variables["outHandedness"].AddDecoration(DecorationLocation, 4);
					entryPoint->addIdOperand(s_builderContent->variables["outHandedness"]);

					ProcessHandednessVertexDefault();
				}
				if (usesColor)
				{
					s_builderContent->variables["outColor"] = Variable(StorageClassOutput, vec4f, "outColor");
					s_builderContent->variables["outColor"].AddDecoration(DecorationLocation, 5);
					entryPoint->addIdOperand(s_builderContent->variables["outColor"]);

					s_builderContent->variables["outColor"] = Value::MakeVector4f(0.7_c, 0.5_c, 0.9_c, 1.0_c);
				}

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				//spv::Disassemble(std::cout, shaderCode);
				//std::cout << "Vertex" << std::endl;

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		Program ShaderBuilder::MakeSkyboxShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type f = Type::MakeFloat();
				Type ui = Type::MakeUint();
				Type vec2ui = Type::MakeVector(ui, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);

				Type imageCubeMap = Type::MakeImage(f, DimCube);
				Type imageCubeMapSampled = Type::MakeSampledImage(imageCubeMap);

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable inPosition = Variable(StorageClassInput, vec3f, "inPosition");
				inPosition.AddDecoration(DecorationLocation, 0);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Variable outId(StorageClassOutput, vec2ui, "outId");
				outId.AddDecoration(DecorationLocation, 1);

				Variable environmentMap = Variable::CreateUniformConstant(imageCubeMapSampled, 0, 1, "environmentMap");
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment);

				Instruction* entryPoint = AddEntryPointDefault(ExecutionModelFragment, mainFunc, "main");
				entryPoint->addIdOperand(inPosition);
				entryPoint->addIdOperand(outColor);
				entryPoint->addIdOperand(outId);

				outColor = environmentMap.ImageSampleImplicitLod(inPosition);

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type vec3f = Type::MakeVector(f, 3);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				Type ModelUBO_t = DeclareModelUBO_t();

				Variable inPosition = Variable(StorageClassInput, vec3f, "inPosition");
				inPosition.AddDecoration(DecorationLocation, 0);

				Variable outPosition = Variable(StorageClassOutput, vec3f, "outPosition");
				outPosition.AddDecoration(DecorationLocation, 0);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(inPosition);
				entryPoint->addIdOperand(outPosition);
				entryPoint->addIdOperand(gl_PerVertex);

				Value mvpMat = dyn.MakeAccessChain({ 0_c });

				Value homogenePosition = Value::MakeVector4f(inPosition.X(), inPosition.Y(), inPosition.Z(), 1.0_c);

				Variable dynmvpTimesTmpPosAccessChain = gl_PerVertex.MakeAccessChain({ 0_c });

				Value temp = mvpMat * homogenePosition;
				dynmvpTimesTmpPosAccessChain = temp.Shuffle(temp, 0, 1, 3, 3);
				//dynmvpTimesTmpPosAccessChain = temp;

				outPosition = inPosition;

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		Program ShaderBuilder::MakeGUIShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec4f = Type::MakeVector(f, 4);

				Type sampledImage_t = Type::MakeSampledImage();

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable inColor = Variable(StorageClassInput, vec4f, "inColor");
				inColor.AddDecoration(DecorationLocation, 0);

				Variable inUV = Variable(StorageClassInput, vec2f, "inUV");
				inUV.AddDecoration(DecorationLocation, 1);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Variable sTexture = Variable::CreateUniformConstant(sampledImage_t, 0, 1, "sTexture");
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment);

				Instruction* entryPoint = AddEntryPointDefault(ExecutionModelFragment, mainFunc, "main");
				entryPoint->addIdOperand(inColor);
				entryPoint->addIdOperand(inUV);
				entryPoint->addIdOperand(outColor);

				outColor = inColor * sTexture.ImageSampleImplicitLod(inUV);

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec4f = Type::MakeVector(f, 4);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				Type PushConstant_t = Type::MakeStruct({ vec2f, vec2f }, "PushConstant_t");

				Variable inPosition = Variable(StorageClassInput, vec2f, "inPosition");
				inPosition.AddDecoration(DecorationLocation, 0);

				Variable inUV = Variable(StorageClassInput, vec2f, "inUV");
				inUV.AddDecoration(DecorationLocation, 1);

				Variable inColor = Variable(StorageClassInput, vec4f, "inColor");
				inColor.AddDecoration(DecorationLocation, 2);

				Variable outColor = Variable(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Variable outUV = Variable(StorageClassOutput, vec2f, "outUV");
				outUV.AddDecoration(DecorationLocation, 1);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");
				//Variable pc = Variable(StorageClassPushConstant, PushConstant_t, "PushConstant");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(inPosition);
				entryPoint->addIdOperand(inUV);
				entryPoint->addIdOperand(inColor);
				entryPoint->addIdOperand(outColor);
				entryPoint->addIdOperand(outUV);
				entryPoint->addIdOperand(gl_PerVertex);

				outColor = inColor;
				outUV = inUV;

				//Value uScale = pc.MakeAccessChain({ 0_c });
				//Value uTranslation = pc.MakeAccessChain({ 1_c });

				Value uScale = Value::MakeVector2f(Value::MakeConstant(2.0f / 1280.0f), Value::MakeConstant(2.0f / 800.0f));
				Value uTranslation = Value::MakeVector2f(Value::MakeConstant(-1.0f), Value::MakeConstant(-1.0f));

				Value newGlPos = inPosition * uScale + uTranslation;

				gl_PerVertex.MakeAccessChain({ 0_c }) = Value::MakeVector4f(newGlPos.X(), newGlPos.Y(), 0.0_c, 1.0_c);

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		Program ShaderBuilder::MakeTextShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type f = Type::MakeFloat();
				Type ui = Type::MakeUint();
				Type vec2ui = Type::MakeVector(ui, 2);
				Type vec2f = Type::MakeVector(f, 2);
				Type vec4f = Type::MakeVector(f, 4);

				Type sampledImage_t = Type::MakeSampledImage();

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable inUV = Variable(StorageClassInput, vec2f, "inUV");
				inUV.AddDecoration(DecorationLocation, 0);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Variable outId(StorageClassOutput, vec2ui, "outId");
				outId.AddDecoration(DecorationLocation, 1);

				Variable sTexture = Variable::CreateUniformConstant(sampledImage_t, 0, 1, "sTexture");
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment);

				Instruction* entryPoint = AddEntryPointDefault(ExecutionModelFragment, mainFunc, "main");
				entryPoint->addIdOperand(inUV);
				entryPoint->addIdOperand(outColor);
				entryPoint->addIdOperand(outId);

				outColor = sTexture.ImageSampleImplicitLod(inUV);

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec4f = Type::MakeVector(f, 4);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				Type ModelUBO_t = DeclareModelUBO_t();
				//Type PushConstant_t = Type::MakeStruct({ vec2f, vec2f }, "PushConstant_t");

				Variable inPosition = Variable(StorageClassInput, vec2f, "inPosition");
				inPosition.AddDecoration(DecorationLocation, 0);

				Variable inUV = Variable(StorageClassInput, vec2f, "inUV");
				inUV.AddDecoration(DecorationLocation, 1);

				Variable outUV = Variable(StorageClassOutput, vec2f, "outUV");
				outUV.AddDecoration(DecorationLocation, 0);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");
				//dsetInfo.AddBinding(vk::DescriptorType::eUniformBufferDynamic, vk::ShaderStageFlagBits::eVertex);
				//Variable pc = Variable(StorageClassPushConstant, PushConstant_t, "PushConstant");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(inPosition);
				entryPoint->addIdOperand(inUV);
				entryPoint->addIdOperand(outUV);
				entryPoint->addIdOperand(gl_PerVertex);

				outUV = inUV;

				//Value uScale = pc.MakeAccessChain({ 0_c });
				//Value uTranslation = pc.MakeAccessChain({ 1_c });

				Value mvpMat = dyn.MakeAccessChain({ 0_c });
				Value homogenePosition = Value::MakeVector4f(inPosition.X(), inPosition.Y(), 0.0_c, 1.0_c);
				ProcessGl_PerVertexVertexDefault(mvpMat, homogenePosition, gl_PerVertex);

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		Program ShaderBuilder::MakeRimShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			bool usesPosition = false;
			bool usesNormal = false;
			bool usesTangent = false;
			bool usesHandedness = false;
			bool usesUV = false;
			bool usesColor = false;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type ui = Type::MakeUint();
				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec2ui = Type::MakeVector(ui, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);
				Type ViewUBO_t = DeclareViewUBO_t();
				Type PointLight_t = DeclarePointLight_t();
				Type DirectionalLight_t = DeclareDirectionalLight_t();
				Type LightUBO_t = DeclareLightUBO_t(PointLight_t, 10, DirectionalLight_t, 10);

				Type image2dSampled = Type::MakeSampledImage();
				Type imageCubeMap = Type::MakeImage(f, spv::DimCube);
				Type imageCubeMapSampled = Type::MakeSampledImage(imageCubeMap);

				sb::Function parallaxFunc;
				if (materialInfo.hasHeightMap)
					parallaxFunc = DeclareParallaxMappingFunc();

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Variable outCompId(StorageClassOutput, vec2ui, "outCompId");
				outCompId.AddDecoration(DecorationLocation, 1);

				dsetInfo.AddBinding(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment); // view
				dsetInfo.AddBinding(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment); // lights

				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // diffuseMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // normalMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // specularMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // heightMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // environmentMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // reflectionMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // refrectionMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // emissive
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // occlusion
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // metalicRoughness

				Type ModelUBO_t = DeclareModelUBO_t();
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				Variable view = Variable::CreateUniform(ViewUBO_t, 0, 1, "view");
				materialInfo.hasViewUBO = true;
				Variable lights = Variable::CreateUniform(LightUBO_t, 1, 1, "lights");
				materialInfo.hasLightUBO = true;

				Variable diffuseMap = Variable::CreateUniformConstant(image2dSampled, 2, 1, "diffuseMap");
				Variable normalMap = Variable::CreateUniformConstant(image2dSampled, 3, 1, "normalMap");
				Variable specularMap = Variable::CreateUniformConstant(image2dSampled, 4, 1, "specularMap");
				Variable displacementMap = Variable::CreateUniformConstant(image2dSampled, 5, 1, "displacementMap");
				Variable environmentMap = Variable::CreateUniformConstant(imageCubeMapSampled, 6, 1, "environmentMap");
				Variable reflectionMap = Variable::CreateUniformConstant(image2dSampled, 7, 1, "reflectionMap");
				Variable refractionMap = Variable::CreateUniformConstant(image2dSampled, 8, 1, "refractionMap");
				Variable emissiveMap = Variable::CreateUniformConstant(image2dSampled, 9, 1, "emissive");
				Variable occlusionMap = Variable::CreateUniformConstant(image2dSampled, 10, 1, "occlusion");
				Variable metalicRoughnessMap = Variable::CreateUniformConstant(image2dSampled, 11, 1, "metalicRoughness");

				Instruction *entryPoint = GetBuilder().addEntryPoint(ExecutionModelFragment, mainFunc.func, "main");
				entryPoint->addIdOperand(outColor);
				entryPoint->addIdOperand(outCompId);

				Variable UBOComponentId = dyn.MakeAccessChain({ 2_c });
				Variable UBOActorId = dyn.MakeAccessChain({ 3_c });
				outCompId.MakeAccessChain({ 0_c }) = UBOComponentId;
				outCompId.MakeAccessChain({ 1_c }) = UBOActorId;
				//outCompId = UBOComponentId;

				Value N;
				if (materialInfo.hasNormalMap || materialInfo.receiveLights || materialInfo.hasReflectionMap ||
					materialInfo.hasRefractionMap || materialInfo.hasMetalicRoughnessMap)
				{
					Variable inNormal(StorageClassInput, vec3f, "inNormal");
					inNormal.AddDecoration(DecorationLocation, 2);
					entryPoint->addIdOperand(inNormal);
					usesNormal = true;

					N = inNormal.Normalize();
				}

				Value V;
				Value inPosition;
				if (materialInfo.hasHeightMap || materialInfo.receiveLights || materialInfo.hasReflectionMap ||
					materialInfo.hasRefractionMap || materialInfo.hasMetalicRoughnessMap)
				{
					Variable varInPosition(StorageClassInput, vec3f, "inPosition");
					varInPosition.AddDecoration(DecorationLocation, 0);
					entryPoint->addIdOperand(varInPosition);
					usesPosition = true;
					inPosition = varInPosition;

					V = (view.MakeAccessChain({ 1_c }) - inPosition).Normalize();
				}

				Value UV;
				if (materialInfo.HasMap())
				{
					usesUV = true;

					Variable inUV(StorageClassInput, vec2f, "inUV");
					inUV.AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(inUV);

					UV = inUV;
				}

				Value diffuseSample;
				if (materialInfo.hasDiffuseMap)
				{
					usesUV = true;
					diffuseSample = diffuseMap.ImageSampleImplicitLod(UV);

					// if (materialInfo.isTransparent)
					// {
					// 	Block& discard = GetBuilder().makeNewBlock();
					// 	Block& noDiscard = GetBuilder().makeNewBlock();

					// 	GetBuilder().createConditionalBranch(diffuseSample.W() <= 0.0_c, &discard, &noDiscard);

					// 	GetBuilder().setBuildPoint(&discard);
					// 	GetBuilder().makeDiscard();
					// 	GetBuilder().setBuildPoint(&noDiscard);
					// }
				}
				else
					diffuseSample = Value::MakeConstant(std::array<float, 4>({ 1.0f, 1.0f, 1.0f, 1.0f }));

				if (materialInfo.receiveLights)
				{
					// Value VdotN = Value::Max(V.Dot(N), 0.0_c);
					Value VdotN = V.Dot(N);

					outColor = outColor.Shuffle(diffuseSample * VdotN, 4, 5, 6, 7);
				}
				else
					outColor = diffuseSample;

				if (materialInfo.correctGamma)
					GammaCorrect(outColor);

				Variable outColorA = outColor.MakeAccessChain({ 3_c });
				outColorA = diffuseSample.W();

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				//spv::Disassemble(std::cout, shaderCode);
				//std::cout << "Fragment" << std::endl;

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type ui = Type::MakeUint();
				//Type ui64 = Type::MakeUint64();
				//Type ui32 = ShaderBuilder::GetBuilder().makeUintType(32);
				Type vec2f = Type::MakeVector(f, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);
				//Type vec4f = Type::MakeVector(f, 4);
				//Type mat4f = Type::MakeMatrix(f, 4, 4);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				Type ModelUBO_t = DeclareModelUBO_t();

				s_builderContent->variables["inPosition"] = Variable(StorageClassInput, vec3f, "inPosition");
				s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, 0);

				s_builderContent->variables["inUV"] = Variable(StorageClassInput, vec2f, "inUV");
				s_builderContent->variables["inUV"].AddDecoration(DecorationLocation, 1);

				s_builderContent->variables["inNormal"] = Variable(StorageClassInput, vec3f, "inNormal");
				s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, 2);

				s_builderContent->variables["inTangent"] = Variable(StorageClassInput, vec3f, "inTangent");
				s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, 3);

				s_builderContent->variables["inHandedness"] = Variable(StorageClassInput, f, "inHandedness");
				s_builderContent->variables["inHandedness"].AddDecoration(DecorationLocation, 4);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(gl_PerVertex);
				entryPoint->addIdOperand(s_builderContent->variables["inPosition"]);
				entryPoint->addIdOperand(s_builderContent->variables["inUV"]);
				entryPoint->addIdOperand(s_builderContent->variables["inNormal"]);
				entryPoint->addIdOperand(s_builderContent->variables["inTangent"]);
				entryPoint->addIdOperand(s_builderContent->variables["inHandedness"]);

				Value modelMat;
				if (usesPosition || usesNormal || usesTangent)
					modelMat = dyn.MakeAccessChain({ 1_c });

				Value mvpMat = dyn.MakeAccessChain({ 0_c });
				Variable& inPosition = s_builderContent->variables["inPosition"];
				Value homogenePosition = Value::MakeVector4f(inPosition.X(), inPosition.Y(), inPosition.Z(), 1.0_c);
				ProcessGl_PerVertexVertexDefault(mvpMat, homogenePosition, gl_PerVertex);

				if (usesPosition)
				{
					s_builderContent->variables["outPosition"] = Variable(StorageClassOutput, vec3f, "outPosition");
					s_builderContent->variables["outPosition"].AddDecoration(DecorationLocation, 0);
					entryPoint->addIdOperand(s_builderContent->variables["outPosition"]);

					ProcessPositionVertexDefault(modelMat, homogenePosition);
				}
				if (usesUV)
				{
					s_builderContent->variables["outUV"] = Variable(StorageClassOutput, vec2f, "outUV");
					s_builderContent->variables["outUV"].AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(s_builderContent->variables["outUV"]);

					ProcessUVVertexDefault();
				}
				if (usesNormal)
				{
					s_builderContent->variables["outNormal"] = Variable(StorageClassOutput, vec3f, "outNormal");
					s_builderContent->variables["outNormal"].AddDecoration(DecorationLocation, 2);
					entryPoint->addIdOperand(s_builderContent->variables["outNormal"]);

					ProcessNormalVertexDefault(modelMat);
				}
				if (usesTangent)
				{
					s_builderContent->variables["outTangent"] = Variable(StorageClassOutput, vec3f, "outTangent");
					s_builderContent->variables["outTangent"].AddDecoration(DecorationLocation, 3);
					entryPoint->addIdOperand(s_builderContent->variables["outTangent"]);

					ProcessTangentVertexDefault(modelMat);
				}
				if (usesHandedness)
				{
					s_builderContent->variables["outHandedness"] = Variable(StorageClassOutput, f, "outHandedness");
					s_builderContent->variables["outHandedness"].AddDecoration(DecorationLocation, 4);
					entryPoint->addIdOperand(s_builderContent->variables["outHandedness"]);

					ProcessHandednessVertexDefault();
				}
				if (usesColor)
				{
					s_builderContent->variables["outColor"] = Variable(StorageClassOutput, vec4f, "outColor");
					s_builderContent->variables["outColor"].AddDecoration(DecorationLocation, 5);
					entryPoint->addIdOperand(s_builderContent->variables["outColor"]);

					s_builderContent->variables["outColor"] = Value::MakeVector4f(0.7_c, 0.5_c, 0.9_c, 1.0_c);
				}

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				//spv::Disassemble(std::cout, shaderCode);
				//std::cout << "Vertex" << std::endl;

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		Program ShaderBuilder::MakeFirstLightScatteringObstacleShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			bool usesPosition = false;
			bool usesNormal = false;
			bool usesTangent = false;
			bool usesHandedness = false;
			bool usesUV = false;
			bool usesColor = false;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type f = Type::MakeFloat();
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Instruction *entryPoint = GetBuilder().addEntryPoint(ExecutionModelFragment, mainFunc.func, "main");
				entryPoint->addIdOperand(outColor);

				Variable outColorR = outColor.MakeAccessChain({ 0_c });
				Variable outColorG = outColor.MakeAccessChain({ 1_c });
				Variable outColorB = outColor.MakeAccessChain({ 2_c });
				Variable outColorA = outColor.MakeAccessChain({ 3_c });
				outColorR = 0.0_c;
				outColorG = 0.0_c;
				outColorB = 0.0_c;
				outColorA = 0.0_c;

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec3f = Type::MakeVector(f, 3);
				//Type vec4f = Type::MakeVector(f, 4);
				//Type mat4f = Type::MakeMatrix(f, 4, 4);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				Type ModelUBO_t = DeclareModelUBO_t();

				s_builderContent->variables["inPosition"] = Variable(StorageClassInput, vec3f, "inPosition");
				s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, 0);

				s_builderContent->variables["inUV"] = Variable(StorageClassInput, vec2f, "inUV");
				s_builderContent->variables["inUV"].AddDecoration(DecorationLocation, 1);

				s_builderContent->variables["inNormal"] = Variable(StorageClassInput, vec3f, "inNormal");
				s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, 2);

				s_builderContent->variables["inTangent"] = Variable(StorageClassInput, vec3f, "inTangent");
				s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, 3);

				s_builderContent->variables["inHandedness"] = Variable(StorageClassInput, f, "inHandedness");
				s_builderContent->variables["inHandedness"].AddDecoration(DecorationLocation, 4);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(gl_PerVertex);
				entryPoint->addIdOperand(s_builderContent->variables["inPosition"]);
				entryPoint->addIdOperand(s_builderContent->variables["inUV"]);
				entryPoint->addIdOperand(s_builderContent->variables["inNormal"]);
				entryPoint->addIdOperand(s_builderContent->variables["inTangent"]);
				entryPoint->addIdOperand(s_builderContent->variables["inHandedness"]);

				Value modelMat;
				if (usesPosition || usesNormal || usesTangent)
					modelMat = dyn.MakeAccessChain({ 1_c });

				Value mvpMat = dyn.MakeAccessChain({ 0_c });
				Variable& inPosition = s_builderContent->variables["inPosition"];
				Value homogenePosition = Value::MakeVector4f(inPosition.X(), inPosition.Y(), inPosition.Z(), 1.0_c);
				ProcessGl_PerVertexVertexDefault(mvpMat, homogenePosition, gl_PerVertex);

				if (usesPosition)
				{
					s_builderContent->variables["outPosition"] = Variable(StorageClassOutput, vec3f, "outPosition");
					s_builderContent->variables["outPosition"].AddDecoration(DecorationLocation, 0);
					entryPoint->addIdOperand(s_builderContent->variables["outPosition"]);

					ProcessPositionVertexDefault(modelMat, homogenePosition);
				}
				if (usesUV)
				{
					s_builderContent->variables["outUV"] = Variable(StorageClassOutput, vec2f, "outUV");
					s_builderContent->variables["outUV"].AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(s_builderContent->variables["outUV"]);

					ProcessUVVertexDefault();
				}
				if (usesNormal)
				{
					s_builderContent->variables["outNormal"] = Variable(StorageClassOutput, vec3f, "outNormal");
					s_builderContent->variables["outNormal"].AddDecoration(DecorationLocation, 2);
					entryPoint->addIdOperand(s_builderContent->variables["outNormal"]);

					ProcessNormalVertexDefault(modelMat);
				}
				if (usesTangent)
				{
					s_builderContent->variables["outTangent"] = Variable(StorageClassOutput, vec3f, "outTangent");
					s_builderContent->variables["outTangent"].AddDecoration(DecorationLocation, 3);
					entryPoint->addIdOperand(s_builderContent->variables["outTangent"]);

					ProcessTangentVertexDefault(modelMat);
				}
				if (usesHandedness)
				{
					s_builderContent->variables["outHandedness"] = Variable(StorageClassOutput, f, "outHandedness");
					s_builderContent->variables["outHandedness"].AddDecoration(DecorationLocation, 4);
					entryPoint->addIdOperand(s_builderContent->variables["outHandedness"]);

					ProcessHandednessVertexDefault();
				}

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		Program ShaderBuilder::MakeFirstLightScatteringWhiteLightShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			bool usesPosition = false;
			bool usesNormal = false;
			bool usesTangent = false;
			bool usesHandedness = false;
			bool usesUV = false;
			bool usesColor = false;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type f = Type::MakeFloat();
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Instruction *entryPoint = GetBuilder().addEntryPoint(ExecutionModelFragment, mainFunc.func, "main");
				entryPoint->addIdOperand(outColor);

				Value color = Value::MakeConstant(Color4f::White());

				outColor = color;

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec3f = Type::MakeVector(f, 3);
				//Type vec4f = Type::MakeVector(f, 4);
				//Type mat4f = Type::MakeMatrix(f, 4, 4);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				Type ModelUBO_t = DeclareModelUBO_t();

				s_builderContent->variables["inPosition"] = Variable(StorageClassInput, vec3f, "inPosition");
				s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, 0);

				s_builderContent->variables["inUV"] = Variable(StorageClassInput, vec2f, "inUV");
				s_builderContent->variables["inUV"].AddDecoration(DecorationLocation, 1);

				s_builderContent->variables["inNormal"] = Variable(StorageClassInput, vec3f, "inNormal");
				s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, 2);

				s_builderContent->variables["inTangent"] = Variable(StorageClassInput, vec3f, "inTangent");
				s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, 3);

				s_builderContent->variables["inHandedness"] = Variable(StorageClassInput, f, "inHandedness");
				s_builderContent->variables["inHandedness"].AddDecoration(DecorationLocation, 4);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(gl_PerVertex);
				entryPoint->addIdOperand(s_builderContent->variables["inPosition"]);
				entryPoint->addIdOperand(s_builderContent->variables["inUV"]);
				entryPoint->addIdOperand(s_builderContent->variables["inNormal"]);
				entryPoint->addIdOperand(s_builderContent->variables["inTangent"]);
				entryPoint->addIdOperand(s_builderContent->variables["inHandedness"]);

				Value modelMat;
				if (usesPosition || usesNormal || usesTangent)
					modelMat = dyn.MakeAccessChain({ 1_c });

				Value mvpMat = dyn.MakeAccessChain({ 0_c });
				Variable& inPosition = s_builderContent->variables["inPosition"];
				Value homogenePosition = Value::MakeVector4f(inPosition.X(), inPosition.Y(), inPosition.Z(), 1.0_c);
				ProcessGl_PerVertexVertexDefault(mvpMat, homogenePosition, gl_PerVertex);

				if (usesPosition)
				{
					s_builderContent->variables["outPosition"] = Variable(StorageClassOutput, vec3f, "outPosition");
					s_builderContent->variables["outPosition"].AddDecoration(DecorationLocation, 0);
					entryPoint->addIdOperand(s_builderContent->variables["outPosition"]);

					ProcessPositionVertexDefault(modelMat, homogenePosition);
				}
				if (usesUV)
				{
					s_builderContent->variables["outUV"] = Variable(StorageClassOutput, vec2f, "outUV");
					s_builderContent->variables["outUV"].AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(s_builderContent->variables["outUV"]);

					ProcessUVVertexDefault();
				}
				if (usesNormal)
				{
					s_builderContent->variables["outNormal"] = Variable(StorageClassOutput, vec3f, "outNormal");
					s_builderContent->variables["outNormal"].AddDecoration(DecorationLocation, 2);
					entryPoint->addIdOperand(s_builderContent->variables["outNormal"]);

					ProcessNormalVertexDefault(modelMat);
				}
				if (usesTangent)
				{
					s_builderContent->variables["outTangent"] = Variable(StorageClassOutput, vec3f, "outTangent");
					s_builderContent->variables["outTangent"].AddDecoration(DecorationLocation, 3);
					entryPoint->addIdOperand(s_builderContent->variables["outTangent"]);

					ProcessTangentVertexDefault(modelMat);
				}
				if (usesHandedness)
				{
					s_builderContent->variables["outHandedness"] = Variable(StorageClassOutput, f, "outHandedness");
					s_builderContent->variables["outHandedness"].AddDecoration(DecorationLocation, 4);
					entryPoint->addIdOperand(s_builderContent->variables["outHandedness"]);

					ProcessHandednessVertexDefault();
				}

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		Program ShaderBuilder::MakeLastLightScatteringShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type ui = Type::MakeUint();
				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);
				Type vec4ui = Type::MakeVector(ui, 4);
				//Type mat3f = Type::MakeMatrix(f, 3, 3);
				//Type mat4f = Type::MakeMatrix(f, 4, 4);
				//Type ViewUBO_t = DeclareViewUBO_t();
				//Type PointLight_t = DeclarePointLight_t();
				//Type LightUBO_t = DeclareLightUBO_t(PointLight_t, 5);
				Type image2dSampled = Type::MakeSampledImage();
				//Type image2d = Type::MakeImage(ui, DimSubpassData, false, false, false, 2);
				Type LightScatteringUBO_t = Type::MakeStruct({ f, f, f, f, vec2f }, "LightScatteringUBO_t");
				LightScatteringUBO_t.AddMemberDecoration(0, DecorationOffset, 0);
				LightScatteringUBO_t.AddMemberDecoration(1, DecorationOffset, sizeof(float));
				LightScatteringUBO_t.AddMemberDecoration(2, DecorationOffset, sizeof(float) * 2);
				LightScatteringUBO_t.AddMemberDecoration(3, DecorationOffset, sizeof(float) * 3);
				LightScatteringUBO_t.AddMemberDecoration(4, DecorationOffset, sizeof(float) * 4);
				LightScatteringUBO_t.AddDecoration(DecorationBlock);

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Variable inUV(StorageClassInput, vec2f, "inUV");
				inUV.AddDecoration(DecorationLocation, 0);

				Variable firstPass = Variable::CreateUniformConstant(image2dSampled, 0, 0, "firstPass");
				Variable lightScatteringUBO = Variable::CreateUniform(LightScatteringUBO_t, 1, 0, "lightScatteringUBO");
				//materialInfo.hasLightScatteringUBO = true;
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment);
				dsetInfo.AddBinding(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment);

				Instruction* entryPoint = GetBuilder().addEntryPoint(ExecutionModelFragment, mainFunc.func, "main");
				entryPoint->addIdOperand(outColor);
				entryPoint->addIdOperand(inUV);

				Variable decay(StorageClassFunction, f, "decay");
				Variable weight(StorageClassFunction, f, "weight");

				Value exposure = lightScatteringUBO.MakeAccessChain({ 0_c });
				decay = lightScatteringUBO.MakeAccessChain({ 1_c });
				Value density = lightScatteringUBO.MakeAccessChain({ 2_c });
				weight = lightScatteringUBO.MakeAccessChain({ 3_c });
				Value lightPositionOnScreen = lightScatteringUBO.MakeAccessChain({ 4_c });

				//Value exposure = Value::MakeConstant(0.0034f);
				//Value decay = Value::MakeConstant(1.0f);
				//Value density = Value::MakeConstant(0.84f);
				//Value weight = Value::MakeConstant(7.65f);
				//
				//Value exposure = Value::MakeConstant(1.0f);
				//Value decay = Value::MakeConstant(1.0f);
				//Value density = Value::MakeConstant(0.5f);
				//Value weight = Value::MakeConstant(2.5f);
				//
				//Value exposure = Value::MakeConstant(0.92f);
				//Value decay = Value::MakeConstant(0.94815f);
				//Value density = Value::MakeConstant(0.926f);
				//Value weight = Value::MakeConstant(0.58767f);
				//Value lightPositionOnScreen = Value::MakeConstant(Vector2(0.5f, 0.5f));
				//Value lightPositionOnScreen = Value::MakeConstant(Vector2(0.0f, 0.0f));
				Value numSampled = Value::MakeConstant(100);
				Value numSampledf = Value::MakeConstant(100.0f);
				//Value numSampled = Value::MakeConstant(1);
				//Value numSampledf = Value::MakeConstant(1.0f);

				Value deltaTextCoordUnscaled = (inUV - lightPositionOnScreen);
				Variable deltaTextCoord(StorageClassFunction, vec2f, "deltaTextCoord");
				deltaTextCoord = deltaTextCoordUnscaled * (1.0_c / numSampledf * density);

				Variable textCoord(StorageClassFunction, vec2f, "textCoord");
				textCoord = inUV;
				Variable illuminationDecay(StorageClassFunction, f, "illuminationDecay");
				illuminationDecay = 1.0_c;

				//outColor = Value::MakeConstant(std::array<float, 4>({ 0.0f, 0.0f, 0.0f, 0.0f }));
				outColor = firstPass.ImageSampleImplicitLod(inUV);

				Variable i(StorageClassFunction, ui, "i");
				i = Value::MakeConstant(0u);

				Variable sample(StorageClassFunction, vec4f, "sample");

				//*
				Builder::LoopBlocks& loopBlocks = GetBuilder().makeNewLoop(); // TODO implement loops
				Block& moreBlock = GetBuilder().makeNewBlock();

				GetBuilder().createBranch(&loopBlocks.head);

				GetBuilder().setBuildPoint(&loopBlocks.head);
				GetBuilder().createLoopMerge(&loopBlocks.merge, &moreBlock, LoopControlMaskNone, LoopControlDependencyInfiniteMask);
				GetBuilder().createBranch(&loopBlocks.continue_target);

				GetBuilder().setBuildPoint(&loopBlocks.continue_target);
				GetBuilder().createConditionalBranch(i.Load() < numSampled, &loopBlocks.body, &loopBlocks.merge);

				GetBuilder().setBuildPoint(&loopBlocks.body);
				textCoord = textCoord.Load() - deltaTextCoord.Load();
				sample = firstPass.ImageSampleImplicitLod(textCoord.Load());
				//sample = ShaderBuilder::GetBuilder().createBinOp(OpImageRead, vec4f, firstPass, textCoord.Load());
				sample = sample.Load() * illuminationDecay.Load() * weight.Load();

				outColor = outColor.Load() + sample.Load();

				illuminationDecay = illuminationDecay.Load() * decay.Load();

				//++i;
				i = i.Load() + Value::MakeConstant(1u);
				GetBuilder().createBranch(&moreBlock);

				GetBuilder().setBuildPoint(&moreBlock);
				GetBuilder().createBranch(&loopBlocks.head);

				GetBuilder().setBuildPoint(&loopBlocks.merge);

				outColor *= exposure;
				//*/

				//Value length = deltaTextCoordUnscaled.Length();

				// Variable alpha = outColor.MakeAccessChain({ 3_c });
				// alpha = 1.0_c;

				//outColor.MakeAccessChain({ 0_c }) = outColor.MakeAccessChain({ 0_c }) * alpha;
				//outColor.MakeAccessChain({ 1_c }) = outColor.MakeAccessChain({ 1_c }) * alpha;
				//outColor.MakeAccessChain({ 2_c }) = outColor.MakeAccessChain({ 2_c }) * alpha;

				//Value newAlpha = outColor.MakeAccessChain({ 0_c }) * outColor.MakeAccessChain({ 3_c });
				//newAlpha = outColor.MakeAccessChain({ 0_c }) * newAlpha;
				//newAlpha = outColor.MakeAccessChain({ 1_c }) * newAlpha;
				//outColor.MakeAccessChain({ 3_c }) = outColor.MakeAccessChain({ 2_c }) * newAlpha;

				//outColor = firstPass.Load().ImageSampleImplicitLod(inUV);
				//outColor.MakeAccessChain({ 0_c }) = lightPositionOnScreen.X();
				//outColor.MakeAccessChain({ 1_c }) = lightPositionOnScreen.Y();
				//outColor.MakeAccessChain({ 0_c }) = inUV.X();
				//outColor.MakeAccessChain({ 1_c }) = inUV.Y();
				//outColorR = inUV.X();
				//outColorG = inUV.Y();
				//outColorR = deltaTextCoordUnscaled.X();
				//outColorG = deltaTextCoordUnscaled.Y();
				//outColorR = 1.0_c - length;
				//outColorG = 1.0_c - length;
				//outColorB = 1.0_c - length;
				//outColorB = 0.0_c;
				//outColorB = firstPass.ImageSampleImplicitLod(inUV).Y();
				//outColorG = firstPass.ImageSampleImplicitLod(inUV).X();
				//outColorB = firstPass.ImageSampleImplicitLod(inUV).X();
				// outColorA = 1.0_c;

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec3f = Type::MakeVector(f, 3);
				//Type vec4f = Type::MakeVector(f, 4);
				//Type mat4f = Type::MakeMatrix(f, 4, 4);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				//Type ModelUBO_t = DeclareModelUBO_t();
				//Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				Variable inPosition(StorageClassInput, vec2f, "inPosition");
				inPosition.AddDecoration(DecorationLocation, 0);

				Variable inUV(StorageClassInput, vec2f, "inUV");
				inUV.AddDecoration(DecorationLocation, 1);

				Variable outUV(StorageClassOutput, vec2f, "outUV");
				outUV.AddDecoration(DecorationLocation, 0);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(gl_PerVertex);
				entryPoint->addIdOperand(inPosition);
				entryPoint->addIdOperand(inUV);
				entryPoint->addIdOperand(outUV);

				//Value homogenePosition = Value::MakeVector4f(inPosition.X(), inPosition.Y(), 0.0_c, 1.0_c);

				//Variable dynmvpTimesTmpPosAccessChain = gl_PerVertex.MakeAccessChain({ 0_c });

				//dynmvpTimesTmpPosAccessChain = homogenePosition;

				gl_PerVertex.MakeAccessChain({ 0_c, 0_c }) = inPosition.X();
				gl_PerVertex.MakeAccessChain({ 0_c, 1_c }) = inPosition.Y();

				outUV = inUV;
				//outUV.MakeAccessChain({ 0_c }) = 1.0_c - inUV.X();
				//outUV.MakeAccessChain({ 1_c }) = 1.0_c - inUV.Y();

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				// spv::Disassemble(std::cout, shaderCode);

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		Program ShaderBuilder::MakeSelectedOutlineShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			bool usesPosition = false;
			bool usesNormal = false;
			bool usesTangent = false;
			bool usesHandedness = false;
			bool usesUV = false;
			bool usesColor = false;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type ui = Type::MakeUint();
				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec2ui = Type::MakeVector(ui, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);
				Type ViewUBO_t = DeclareViewUBO_t();
				Type PointLight_t = DeclarePointLight_t();
				Type DirectionalLight_t = DeclareDirectionalLight_t();
				Type LightUBO_t = DeclareLightUBO_t(PointLight_t, 10, DirectionalLight_t, 10);

				Type image2dSampled = Type::MakeSampledImage();
				Type imageCubeMap = Type::MakeImage(f, spv::DimCube);
				Type imageCubeMapSampled = Type::MakeSampledImage(imageCubeMap);

				//sb::Function parallaxFunc;
				//if (materialInfo.hasHeightMap)
				//	parallaxFunc = DeclareParallaxMappingFunc();

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				Variable outCompId(StorageClassOutput, vec2ui, "outCompId");
				outCompId.AddDecoration(DecorationLocation, 1);

				dsetInfo.AddBinding(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment); // view
				dsetInfo.AddBinding(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment); // lights

				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // diffuseMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // normalMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // specularMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // heightMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // environmentMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // reflectionMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // refrectionMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // emissive
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // occlusion
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // metalicRoughness

				Type ModelUBO_t = DeclareModelUBO_t();
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				Variable view = Variable::CreateUniform(ViewUBO_t, 0, 1, "view");
				materialInfo.hasViewUBO = true;
				Variable lights = Variable::CreateUniform(LightUBO_t, 1, 1, "lights");
				materialInfo.hasLightUBO = true;

				Variable diffuseMap = Variable::CreateUniformConstant(image2dSampled, 2, 1, "diffuseMap");
				Variable normalMap = Variable::CreateUniformConstant(image2dSampled, 3, 1, "normalMap");
				Variable specularMap = Variable::CreateUniformConstant(image2dSampled, 4, 1, "specularMap");
				Variable displacementMap = Variable::CreateUniformConstant(image2dSampled, 5, 1, "displacementMap");
				Variable environmentMap = Variable::CreateUniformConstant(imageCubeMapSampled, 6, 1, "environmentMap");
				Variable reflectionMap = Variable::CreateUniformConstant(image2dSampled, 7, 1, "reflectionMap");
				Variable refractionMap = Variable::CreateUniformConstant(image2dSampled, 8, 1, "refractionMap");
				Variable emissiveMap = Variable::CreateUniformConstant(image2dSampled, 9, 1, "emissive");
				Variable occlusionMap = Variable::CreateUniformConstant(image2dSampled, 10, 1, "occlusion");
				Variable metalicRoughnessMap = Variable::CreateUniformConstant(image2dSampled, 11, 1, "metalicRoughness");

				Instruction *entryPoint = GetBuilder().addEntryPoint(ExecutionModelFragment, mainFunc.func, "main");
				entryPoint->addIdOperand(outColor);
				entryPoint->addIdOperand(outCompId);

				Value diffuseSample;
				if (materialInfo.hasDiffuseMap)
				{
					usesUV = true;
					Variable inUV(StorageClassInput, vec2f, "inUV");
					inUV.AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(inUV);
					outColor = diffuseMap.ImageSampleImplicitLod(inUV);
				}
				else
					outColor = Value::MakeConstant(std::array<float, 4>({ 1.0f, 1.0f, 1.0f, 1.0f }));

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				//spv::Disassemble(std::cout, shaderCode);
				//std::cout << "Fragment" << std::endl;

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type ui = Type::MakeUint();
				//Type ui64 = Type::MakeUint64();
				//Type ui32 = ShaderBuilder::GetBuilder().makeUintType(32);
				Type vec2f = Type::MakeVector(f, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);
				//Type vec4f = Type::MakeVector(f, 4);
				//Type mat4f = Type::MakeMatrix(f, 4, 4);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				Type ModelUBO_t = DeclareModelUBO_t();

				s_builderContent->variables["inPosition"] = Variable(StorageClassInput, vec3f, "inPosition");
				s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, 0);

				s_builderContent->variables["inUV"] = Variable(StorageClassInput, vec2f, "inUV");
				s_builderContent->variables["inUV"].AddDecoration(DecorationLocation, 1);

				s_builderContent->variables["inNormal"] = Variable(StorageClassInput, vec3f, "inNormal");
				s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, 2);

				s_builderContent->variables["inTangent"] = Variable(StorageClassInput, vec3f, "inTangent");
				s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, 3);

				s_builderContent->variables["inHandedness"] = Variable(StorageClassInput, f, "inHandedness");
				s_builderContent->variables["inHandedness"].AddDecoration(DecorationLocation, 4);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(gl_PerVertex);
				entryPoint->addIdOperand(s_builderContent->variables["inPosition"]);
				entryPoint->addIdOperand(s_builderContent->variables["inUV"]);
				entryPoint->addIdOperand(s_builderContent->variables["inNormal"]);
				entryPoint->addIdOperand(s_builderContent->variables["inTangent"]);
				entryPoint->addIdOperand(s_builderContent->variables["inHandedness"]);

				Value modelMat;
				if (usesPosition || usesNormal || usesTangent)
					modelMat = dyn.MakeAccessChain({ 1_c });

				Value mvpMat = dyn.MakeAccessChain({ 0_c });
				Variable& inPosition = s_builderContent->variables["inPosition"];

				//Value tmp = modelMat.Inverse().Transpose();
				//Value vec3_1 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 0)).XYZ();
				//Value vec3_2 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 1)).XYZ();
				//Value vec3_3 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 2)).XYZ();
				//Value inPosition2 = inPosition + (Value::MakeMatrix3f(vec3_1, vec3_2, vec3_3) * s_builderContent->variables["inNormal"]).Normalize() * 0.1_c;
				//Value inPosition2 = inPosition * 1.1_c;

				//Value homogenePosition = Value::MakeVector4f(inPosition2.X(), inPosition2.Y(), inPosition2.Z(), 1.0_c);
				Value homogenePosition = Value::MakeVector4f(inPosition.X(), inPosition.Y(), inPosition.Z(), 1.0_c);
				ProcessGl_PerVertexVertexDefault(mvpMat, homogenePosition, gl_PerVertex);

				if (usesPosition)
				{
					s_builderContent->variables["outPosition"] = Variable(StorageClassOutput, vec3f, "outPosition");
					s_builderContent->variables["outPosition"].AddDecoration(DecorationLocation, 0);
					entryPoint->addIdOperand(s_builderContent->variables["outPosition"]);

					ProcessPositionVertexDefault(modelMat, homogenePosition);
				}
				if (usesUV)
				{
					s_builderContent->variables["outUV"] = Variable(StorageClassOutput, vec2f, "outUV");
					s_builderContent->variables["outUV"].AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(s_builderContent->variables["outUV"]);

					ProcessUVVertexDefault();
				}
				if (usesNormal)
				{
					s_builderContent->variables["outNormal"] = Variable(StorageClassOutput, vec3f, "outNormal");
					s_builderContent->variables["outNormal"].AddDecoration(DecorationLocation, 2);
					entryPoint->addIdOperand(s_builderContent->variables["outNormal"]);

					ProcessNormalVertexDefault(modelMat);
				}
				if (usesTangent)
				{
					s_builderContent->variables["outTangent"] = Variable(StorageClassOutput, vec3f, "outTangent");
					s_builderContent->variables["outTangent"].AddDecoration(DecorationLocation, 3);
					entryPoint->addIdOperand(s_builderContent->variables["outTangent"]);

					ProcessTangentVertexDefault(modelMat);
				}
				if (usesHandedness)
				{
					s_builderContent->variables["outHandedness"] = Variable(StorageClassOutput, f, "outHandedness");
					s_builderContent->variables["outHandedness"].AddDecoration(DecorationLocation, 4);
					entryPoint->addIdOperand(s_builderContent->variables["outHandedness"]);

					ProcessHandednessVertexDefault();
				}
				if (usesColor)
				{
					s_builderContent->variables["outColor"] = Variable(StorageClassOutput, vec4f, "outColor");
					s_builderContent->variables["outColor"].AddDecoration(DecorationLocation, 5);
					entryPoint->addIdOperand(s_builderContent->variables["outColor"]);

					s_builderContent->variables["outColor"] = Value::MakeVector4f(0.7_c, 0.5_c, 0.9_c, 1.0_c);
				}

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				//spv::Disassemble(std::cout, shaderCode);
				//std::cout << "Vertex" << std::endl;

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		Program ShaderBuilder::MakeHorizontalBlurShaderProgram(MaterialInfo& materialInfo, DescriptorSetLayoutInfo& dsetInfo)
		{
			Program program;

			using namespace spv;

			bool usesPosition = false;
			bool usesNormal = false;
			bool usesTangent = false;
			bool usesHandedness = false;
			bool usesUV = false;
			bool usesColor = false;

			{
				Begin();

				//=====================================
				//				  Main
				//=====================================

				Type ui = Type::MakeUint();
				Type f = Type::MakeFloat();
				Type vec2f = Type::MakeVector(f, 2);
				Type vec2ui = Type::MakeVector(ui, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);
				Type ViewUBO_t = DeclareViewUBO_t();
				Type PointLight_t = DeclarePointLight_t();
				Type DirectionalLight_t = DeclareDirectionalLight_t();
				Type LightUBO_t = DeclareLightUBO_t(PointLight_t, 10, DirectionalLight_t, 10);

				Type image2dSampled = Type::MakeSampledImage();
				Type imageCubeMap = Type::MakeImage(f, spv::DimCube);
				Type imageCubeMapSampled = Type::MakeSampledImage(imageCubeMap);

				//sb::Function parallaxFunc;
				//if (materialInfo.hasHeightMap)
				//	parallaxFunc = DeclareParallaxMappingFunc();

				sb::Function mainFunc = sb::Function::MakeEntryPoint("main");
				mainFunc.AddExecutionMode(ExecutionModeOriginUpperLeft);

				Variable outColor(StorageClassOutput, vec4f, "outColor");
				outColor.AddDecoration(DecorationLocation, 0);

				//Variable outCompId(StorageClassOutput, vec2ui, "outCompId");
				//outCompId.AddDecoration(DecorationLocation, 1);

				dsetInfo.AddBinding(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment); // view
				dsetInfo.AddBinding(vk::DescriptorType::eUniformBuffer, vk::ShaderStageFlagBits::eFragment); // lights

				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // diffuseMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // normalMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // specularMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // heightMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // environmentMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // reflectionMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // refrectionMap
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // emissive
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // occlusion
				dsetInfo.AddBinding(vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment); // metalicRoughness

				Type ModelUBO_t = DeclareModelUBO_t();
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				Variable view = Variable::CreateUniform(ViewUBO_t, 0, 1, "view");
				materialInfo.hasViewUBO = true;
				Variable lights = Variable::CreateUniform(LightUBO_t, 1, 1, "lights");
				materialInfo.hasLightUBO = true;

				Variable diffuseMap = Variable::CreateUniformConstant(image2dSampled, 2, 1, "diffuseMap");
				Variable normalMap = Variable::CreateUniformConstant(image2dSampled, 3, 1, "normalMap");
				Variable specularMap = Variable::CreateUniformConstant(image2dSampled, 4, 1, "specularMap");
				Variable displacementMap = Variable::CreateUniformConstant(image2dSampled, 5, 1, "displacementMap");
				Variable environmentMap = Variable::CreateUniformConstant(imageCubeMapSampled, 6, 1, "environmentMap");
				Variable reflectionMap = Variable::CreateUniformConstant(image2dSampled, 7, 1, "reflectionMap");
				Variable refractionMap = Variable::CreateUniformConstant(image2dSampled, 8, 1, "refractionMap");
				Variable emissiveMap = Variable::CreateUniformConstant(image2dSampled, 9, 1, "emissive");
				Variable occlusionMap = Variable::CreateUniformConstant(image2dSampled, 10, 1, "occlusion");
				Variable metalicRoughnessMap = Variable::CreateUniformConstant(image2dSampled, 11, 1, "metalicRoughness");

				Instruction *entryPoint = GetBuilder().addEntryPoint(ExecutionModelFragment, mainFunc.func, "main");
				entryPoint->addIdOperand(outColor);
				//entryPoint->addIdOperand(outCompId);

				Value diffuseSample;
				if (materialInfo.hasDiffuseMap)
				{
					usesUV = true;
					Variable inUV(StorageClassInput, vec2f, "inUV");
					inUV.AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(inUV);
					outColor = diffuseMap.ImageSampleImplicitLod(inUV);


				}
				else
					outColor = Value::MakeConstant(std::array<float, 4>({ 1.0f, 1.0f, 1.0f, 1.0f }));

				mainFunc.ReturnImplicit();
				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				//spv::Disassemble(std::cout, shaderCode);
				//std::cout << "Fragment" << std::endl;

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eFragment);

				End();
			}

			{
				Begin();

				Type f = Type::MakeFloat();
				Type ui = Type::MakeUint();
				//Type ui64 = Type::MakeUint64();
				//Type ui32 = ShaderBuilder::GetBuilder().makeUintType(32);
				Type vec2f = Type::MakeVector(f, 2);
				Type vec3f = Type::MakeVector(f, 3);
				Type vec4f = Type::MakeVector(f, 4);
				//Type vec4f = Type::MakeVector(f, 4);
				//Type mat4f = Type::MakeMatrix(f, 4, 4);

				Type gl_PerVertex_t = Declaregl_PerVertex_t();
				Type ModelUBO_t = DeclareModelUBO_t();

				s_builderContent->variables["inPosition"] = Variable(StorageClassInput, vec3f, "inPosition");
				s_builderContent->variables["inPosition"].AddDecoration(DecorationLocation, 0);

				s_builderContent->variables["inUV"] = Variable(StorageClassInput, vec2f, "inUV");
				s_builderContent->variables["inUV"].AddDecoration(DecorationLocation, 1);

				s_builderContent->variables["inNormal"] = Variable(StorageClassInput, vec3f, "inNormal");
				s_builderContent->variables["inNormal"].AddDecoration(DecorationLocation, 2);

				s_builderContent->variables["inTangent"] = Variable(StorageClassInput, vec3f, "inTangent");
				s_builderContent->variables["inTangent"].AddDecoration(DecorationLocation, 3);

				s_builderContent->variables["inHandedness"] = Variable(StorageClassInput, f, "inHandedness");
				s_builderContent->variables["inHandedness"].AddDecoration(DecorationLocation, 4);

				Variable gl_PerVertex(StorageClassOutput, gl_PerVertex_t, "gl_PerVertex");
				Variable dyn = Variable::CreateUniform(ModelUBO_t, 0, 0, "ModelUBO");

				auto mainFunc = sb::Function::MakeEntryPoint("main");
				auto entryPoint = AddEntryPointDefault(ExecutionModelVertex, mainFunc, "main");
				entryPoint->addIdOperand(gl_PerVertex);
				entryPoint->addIdOperand(s_builderContent->variables["inPosition"]);
				entryPoint->addIdOperand(s_builderContent->variables["inUV"]);
				entryPoint->addIdOperand(s_builderContent->variables["inNormal"]);
				entryPoint->addIdOperand(s_builderContent->variables["inTangent"]);
				entryPoint->addIdOperand(s_builderContent->variables["inHandedness"]);

				Value modelMat;
				if (usesPosition || usesNormal || usesTangent)
					modelMat = dyn.MakeAccessChain({ 1_c });

				Value mvpMat = dyn.MakeAccessChain({ 0_c });
				Variable& inPosition = s_builderContent->variables["inPosition"];

				//Value tmp = modelMat.Inverse().Transpose();
				//Value vec3_1 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 0)).XYZ();
				//Value vec3_2 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 1)).XYZ();
				//Value vec3_3 = Value(ShaderBuilder::GetBuilder().createCompositeExtract(tmp, vec4f, 2)).XYZ();
				//Value inPosition2 = inPosition + (Value::MakeMatrix3f(vec3_1, vec3_2, vec3_3) * s_builderContent->variables["inNormal"]).Normalize() * 0.1_c;
				//Value inPosition2 = inPosition * 1.1_c;

				//Value homogenePosition = Value::MakeVector4f(inPosition2.X(), inPosition2.Y(), inPosition2.Z(), 1.0_c);
				Value homogenePosition = Value::MakeVector4f(inPosition.X(), inPosition.Y(), inPosition.Z(), 1.0_c);
				ProcessGl_PerVertexVertexDefault(mvpMat, homogenePosition, gl_PerVertex);

				if (usesPosition)
				{
					s_builderContent->variables["outPosition"] = Variable(StorageClassOutput, vec3f, "outPosition");
					s_builderContent->variables["outPosition"].AddDecoration(DecorationLocation, 0);
					entryPoint->addIdOperand(s_builderContent->variables["outPosition"]);

					ProcessPositionVertexDefault(modelMat, homogenePosition);
				}
				if (usesUV)
				{
					s_builderContent->variables["outUV"] = Variable(StorageClassOutput, vec2f, "outUV");
					s_builderContent->variables["outUV"].AddDecoration(DecorationLocation, 1);
					entryPoint->addIdOperand(s_builderContent->variables["outUV"]);

					ProcessUVVertexDefault();
				}
				if (usesNormal)
				{
					s_builderContent->variables["outNormal"] = Variable(StorageClassOutput, vec3f, "outNormal");
					s_builderContent->variables["outNormal"].AddDecoration(DecorationLocation, 2);
					entryPoint->addIdOperand(s_builderContent->variables["outNormal"]);

					ProcessNormalVertexDefault(modelMat);
				}
				if (usesTangent)
				{
					s_builderContent->variables["outTangent"] = Variable(StorageClassOutput, vec3f, "outTangent");
					s_builderContent->variables["outTangent"].AddDecoration(DecorationLocation, 3);
					entryPoint->addIdOperand(s_builderContent->variables["outTangent"]);

					ProcessTangentVertexDefault(modelMat);
				}
				if (usesHandedness)
				{
					s_builderContent->variables["outHandedness"] = Variable(StorageClassOutput, f, "outHandedness");
					s_builderContent->variables["outHandedness"].AddDecoration(DecorationLocation, 4);
					entryPoint->addIdOperand(s_builderContent->variables["outHandedness"]);

					ProcessHandednessVertexDefault();
				}
				if (usesColor)
				{
					s_builderContent->variables["outColor"] = Variable(StorageClassOutput, vec4f, "outColor");
					s_builderContent->variables["outColor"].AddDecoration(DecorationLocation, 5);
					entryPoint->addIdOperand(s_builderContent->variables["outColor"]);

					s_builderContent->variables["outColor"] = Value::MakeVector4f(0.7_c, 0.5_c, 0.9_c, 1.0_c);
				}

				mainFunc.Leave();

				std::vector<uint32> shaderCode;
				GetBuilder().dump(shaderCode);

				//spv::Disassemble(std::cout, shaderCode);
				//std::cout << "Vertex" << std::endl;

				program.AddShaderModule(Context::GetDevice().Get().createShaderModuleUnique(
					vk::ShaderModuleCreateInfo(
						vk::ShaderModuleCreateFlags(),
						uint32(shaderCode.size()) * sizeof(uint32),
						shaderCode.data()
					)
				), vk::ShaderStageFlagBits::eVertex);

				End();
			}

			return program;
		}

		void ShaderBuilder::Disassemble(const std::vector<uint32>& shaderCode)
		{
			spv::Disassemble(std::cout, shaderCode);
		}
	}
}