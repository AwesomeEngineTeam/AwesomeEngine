#include <Awesome/Renderer/Memory/BufferManager.hpp>
#include <Awesome/Renderer/Control/RenderSurface.hpp>
#include <Awesome/Renderer/GUI/GUIManager.hpp>

#include <Awesome/Windowing/Input/InputManager.hpp>

#include <Awesome/Maths/Transform.hpp>

#include <Awesome/SDK/Scene/Actor.hpp>
#include <Awesome/SDK/System/RenderSystem.hpp>
#include <Awesome/SDK/Components/Physical/BoxCollider.hpp>
#include <Awesome/SDK/Components/Physical/CapsuleCollider.hpp>
#include <Awesome/SDK/Components/Physical/SphereCollider.hpp>
#include <Awesome/SDK/Components/Physical/PlaneCollider.hpp>
#include <Awesome/SDK/Components/Physical/Rigidbody.hpp>
#include <Awesome/SDK/Resources/OBJLoader.hpp>
#include <Awesome/SDK/Resources/FontLoader.hpp>
#include <Awesome/SDK/Resources/TextureLoader.hpp>

#include <Awesome/SDK/BaseApplication.hpp>

#include <unordered_map>
#include <functional>

namespace aw
{
	namespace sdk
	{
		DescriptorSetLayoutInfo RenderSystem::s_modelUBDLayoutInfo{};
		vk::UniqueDescriptorSetLayout RenderSystem::s_modelUBDLayout{};

		std::vector<RenderSystem::ModelUBDGroup> RenderSystem::s_modelUBDGroups;

		std::vector<std::unique_ptr<MeshRenderer>> RenderSystem::s_debug3DLines;

		std::vector<MeshRenderer*> RenderSystem::s_meshRenderers;
		std::vector<Camera*> RenderSystem::s_cameras;
		std::vector<PointLight*> RenderSystem::s_pointLights;
		std::vector<DirectionalLight*> RenderSystem::s_directionalLights;
		std::vector<Text2D*> RenderSystem::s_text2Ds;
		std::vector<Billboard*> RenderSystem::s_billboards;

		std::vector<Component*> RenderSystem::s_compsToInstantiate;
		std::mutex RenderSystem::s_instMutex;

		std::vector<RenderSystem::Gizmo2D> RenderSystem::s_gizmos2D{};
		std::vector<RenderSystem::Gizmo3D> RenderSystem::s_gizmos3D{};

		uint32 RenderSystem::s_width{ 0 };
		uint32 RenderSystem::s_height{ 0 };

		uint32 RenderSystem::s_firstLightScatteringObstacleMaterialIndex{ 0 };
		uint32 RenderSystem::s_firstLightScatteringWhiteLightMaterialIndex{ 0 };
		uint32 RenderSystem::s_lastLightScatteringMaterialIndex{ 0 };
		uint32 RenderSystem::s_forwardWhiteLightMaterialIndex{ 0 };
		uint32 RenderSystem::s_selectedOutlineMaterialIndex{ 0 };

		uint32 RenderSystem::s_textMaterialIndex{ 0 };
		FontAtlas RenderSystem::s_fontAtlas;

		std::vector<Material> RenderSystem::s_materials{};
		std::vector<Mesh*> RenderSystem::s_meshes{};
		std::vector<Pass> RenderSystem::s_passes{};
		std::vector<SampledTextureHandle> RenderSystem::s_textures{};

		uint64 RenderSystem::s_modelUBDSize{ 0 };
		uint64 RenderSystem::s_modelUBDCountPerPool{ 0 };

		std::unordered_map<RenderSystem::InstanceMaterialIndices, ShaderHandle, RenderSystem::InstanceMaterialIndicesHash> RenderSystem::s_instanceMaterials{};

		Mesh RenderSystem::s_sceenQuad{};
		Mesh RenderSystem::s_3dQuad{};

		//Model RenderSystem::s_3dQuadModel{};
		//ModelInstance RenderSystem::s_3dQuadModelInstance{};

		Skybox* RenderSystem::s_skybox{ nullptr };

		Framegraph RenderSystem::s_framegraph;
		FrameResourcesManager RenderSystem::s_frameResourcesManager{};
		RenderQueue RenderSystem::s_opaqueRQ;
		RenderQueue RenderSystem::s_transparentRQ;
		RenderQueue RenderSystem::s_lightScatteringEmitterRQ;
		RenderQueue RenderSystem::s_lightScatteringObstacleRQ;

		Pass RenderSystem::s_forwardPass{};
		Pass RenderSystem::s_transparencyPass{};
		Pass RenderSystem::s_firstLightScatteringPass{};
		Pass RenderSystem::s_lastLightScatteringPass{};

		uint32 RenderSystem::s_forwardPassIndex{ 0 };
		uint32 RenderSystem::s_transparencyPassIndex{ 0 };
		uint32 RenderSystem::s_firstLightScatteringPassIndex{ 0 };
		uint32 RenderSystem::s_lastLightScatteringPassIndex{ 0 };

		uint32 RenderSystem::s_presentImageIndex{ 0 };
		uint32 RenderSystem::s_firstLightScatterImageIndex{ 0 };

		#ifdef WITH_EDITOR
		uint32 RenderSystem::s_componentIdImageIndex{ 0 };
		SubBuffer RenderSystem::s_componentIdSubBuffer{};

		uint32 RenderSystem::s_redUnlitMaterialIndex{ ~0 };
		uint32 RenderSystem::s_greenUnlitMaterialIndex{ ~0 };
		uint32 RenderSystem::s_blueUnlitMaterialIndex{ ~0 };

		uint32 RenderSystem::s_redLandmarkMaterialIndex{ ~0 };
		uint32 RenderSystem::s_greenLandmarkMaterialIndex{ ~0 };
		uint32 RenderSystem::s_blueLandmarkMaterialIndex{ ~0 };
		uint32 RenderSystem::s_whiteLandmarkMaterialIndex{ ~0 };
		uint32 RenderSystem::s_yellowLandmarkMaterialIndex{ ~0 };
		uint32 RenderSystem::s_cyanLandmarkMaterialIndex{ ~0 };
		uint32 RenderSystem::s_magentaLandmarkMaterialIndex{ ~0 };
		#endif

		UniformBuffer RenderSystem::s_viewUBO;
		UniformBuffer RenderSystem::s_lightsUBO;
		UniformBuffer RenderSystem::s_lightScatteringUBO;

		vk::UniqueCommandBuffer RenderSystem::s_copyCommandBuffer{};

		ResourceManager RenderSystem::s_resourceManager;

		LightScatteringUBO RenderSystem::LightScatteringUBO{};

		#ifdef WITH_EDITOR
		uint32 RenderSystem::s_quadMeshIndex{ ~0 };
		uint32 RenderSystem::s_debugMaterialIndex{ ~0 };
		uint32 RenderSystem::s_lightGizmoMaterialIndex{ ~0 };
		uint32 RenderSystem::s_sunGizmoMaterialIndex{ ~0 };
		uint32 RenderSystem::s_cameraGizmoMaterialIndex{ ~0 };

		uint32 RenderSystem::s_landmarkForwardPassIndex{ ~0 };
		Pass RenderSystem::s_landmarkForwardPass{};

		RenderSystem::Gizmo3D RenderSystem::s_xLandmark{};
		RenderSystem::Gizmo3D RenderSystem::s_yLandmark{};
		RenderSystem::Gizmo3D RenderSystem::s_zLandmark{};
		RenderSystem::Gizmo3D RenderSystem::s_cLandmark{};
		RenderSystem::Gizmo3D RenderSystem::s_xyLandmark{};
		RenderSystem::Gizmo3D RenderSystem::s_yzLandmark{};
		RenderSystem::Gizmo3D RenderSystem::s_zxLandmark{};
		#endif

		Camera* RenderSystem::editorCamera{ nullptr };

		// RenderSystem::RenderSystem(uint64 maxPointLights) :
		// 	m_maxPointLights{ maxPointLights },
		// 	m_maxDirectionalLights{ maxPointLights }
		// {
		// 	InitModelUBD(sizeof(ModelUBO), 30);

		// 	vk::CommandBufferAllocateInfo cpyCmdBufAllocInfo(Context::GetCommandPool(), vk::CommandBufferLevel::eSecondary, 1);
		// 	m_copyCommandBuffer = std::move(Context::GetDevice().Get().allocateCommandBuffersUnique(cpyCmdBufAllocInfo)[0]);

		// 	LoadPrimitives();
		// 	Init();
		// }

		void RenderSystem::Init()
		{
			InitModelUBD(sizeof(ModelUBO), 30);

			vk::CommandBufferAllocateInfo cpyCmdBufAllocInfo(Context::GetCommandPool(), vk::CommandBufferLevel::eSecondary, 1);
			s_copyCommandBuffer = std::move(Context::GetDevice().Get().allocateCommandBuffersUnique(cpyCmdBufAllocInfo)[0]);

			s_componentIdSubBuffer = BufferManager::Reserve(512 * 512 * sizeof(uint32), false, BufferType::View);

			LoadPrimitives();
			LoadDefaultTextures();

			s_viewUBO.Create(sizeof(ViewUBO));
			s_lightsUBO.Create((sizeof(PointLightUBO) * s_maxPointLights + sizeof(uint32)) * 2);
			s_lightScatteringUBO.Create(sizeof(LightScatteringUBO));

			{
				s_sceenQuad.vertexLayout = &VertexP2T::Layout;
				s_sceenQuad.indexType = vk::IndexType::eUint16;

				std::vector<uint8> bytes(4 * sizeof(VertexP2T));

				{
					VectorView<VertexP2T> vertices(bytes);
					vertices[0].Get<PositionXY, Vector2>() = { -1,  1 };
					vertices[0].Get<TextureUV, Vector2>() = { 0,  1 };

					vertices[1].Get<PositionXY, Vector2>() = { 1,  1 };
					vertices[1].Get<TextureUV, Vector2>() = { 1,  1 };

					vertices[2].Get<PositionXY, Vector2>() = { -1, -1 };
					vertices[2].Get<TextureUV, Vector2>() = { 0,  0 };

					vertices[3].Get<PositionXY, Vector2>() = { 1, -1 };
					vertices[3].Get<TextureUV, Vector2>() = { 1,  0 };

					s_sceenQuad.SetVertices(bytes);
				}

				bytes.resize(6 * sizeof(uint16));

				for (uint32 i{ 0 }; i < bytes.size(); i++)
					bytes[i] = 0;

				{
					VectorView<uint16> indices(bytes);
					indices[0] = 0;
					indices[1] = 1;
					indices[2] = 2;
					indices[3] = 1;
					indices[4] = 3;
					indices[5] = 2;

					s_sceenQuad.SetIndices(bytes);
					s_3dQuad.SetIndices(bytes);
				}

				s_sceenQuad.Flush();

				s_3dQuad.vertexLayout = &VertexPTNTaH::Layout;
				s_3dQuad.indexType = vk::IndexType::eUint16;

				bytes.resize(4 * sizeof(VertexPTNTaH));

				{
					VectorView<VertexPTNTaH> vertices(bytes);
					vertices[0].Get<PositionXYZ, Vector3>() = { -1, 1, 0 };
					vertices[0].Get<TextureUV, Vector2>() = { 0, 1 };
					vertices[0].Get<NormalXYZ, Vector3>() = { 0, 0, 1 };
					vertices[0].Get<TangentXYZ, Vector3>() = { 1, 0, 0 };
					vertices[0].Get<Handedness, float>() = 1;

					vertices[1].Get<PositionXYZ, Vector3>() = { 1, 1, 0 };
					vertices[1].Get<TextureUV, Vector2>() = { 1, 1 };
					vertices[1].Get<NormalXYZ, Vector3>() = { 0, 0, 1 };
					vertices[1].Get<TangentXYZ, Vector3>() = { 1, 0, 0 };
					vertices[1].Get<Handedness, float>() = 1;

					vertices[2].Get<PositionXYZ, Vector3>() = { -1, -1, 0 };
					vertices[2].Get<TextureUV, Vector2>() = { 0, 0 };
					vertices[2].Get<NormalXYZ, Vector3>() = { 0, 0, 1 };
					vertices[2].Get<TangentXYZ, Vector3>() = { 1, 0, 0 };
					vertices[2].Get<Handedness, float>() = 1;

					vertices[3].Get<PositionXYZ, Vector3>() = { 1, -1, 0 };
					vertices[3].Get<TextureUV, Vector2>() = { 1, 0 };
					vertices[3].Get<NormalXYZ, Vector3>() = { 0, 0, 1 };
					vertices[3].Get<TangentXYZ, Vector3>() = { 1, 0, 0 };
					vertices[3].Get<Handedness, float>() = 1;

					s_3dQuad.SetVertices(bytes);
				}

				s_3dQuad.Flush();
			}

			s_meshes.push_back(&s_sceenQuad);
			uint32 sceenQuadIndex = s_meshes.size() - 1;
			{
				Material mat;
				mat.materialInfo.receiveLights = false;
				s_resourceManager.materialManager.CreateMaterial(mat, sb::ShaderBuilder::MakeShaderProgram);

				RegisterMaterial(std::move(mat));
			}
			{
				s_fontAtlas = FontLoader::Load(s_resourceManager, "assets/fonts/DroidSans.ttf", 64.0f);

				vk::SamplerCreateInfo samplerCreateInfo(
					vk::SamplerCreateFlags(),
					vk::Filter::eLinear,
					vk::Filter::eLinear,
					vk::SamplerMipmapMode::eLinear,
					vk::SamplerAddressMode::eRepeat,
					vk::SamplerAddressMode::eRepeat,
					vk::SamplerAddressMode::eRepeat,
					0.0f,
					true,
					8.0f,
					false,
					vk::CompareOp::eAlways,
					-1000,
					1000,
					vk::BorderColor::eFloatTransparentBlack,
					false
				);

				SampledTexture atlasTextureUnique(samplerCreateInfo);
				atlasTextureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					s_fontAtlas.atlas,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				SampledTextureHandle atlasTexture = s_resourceManager.Register("atlasTexture", std::move(atlasTextureUnique));

				Material textMaterial;
				//textMaterial.SetDiffuse(atlasTexture);
				textMaterial.materialInfo.receiveLights = false;
				textMaterial.materialInfo.correctGamma = false;
				textMaterial.materialInfo.isTransparent = true;
				textMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
				textMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				s_resourceManager.materialManager.CreateMaterial(textMaterial, sb::ShaderBuilder::MakeTextShaderProgram);
				textMaterial.SetSampledTexture(atlasTexture, 0);
				s_textMaterialIndex = RegisterMaterial(std::move(textMaterial));



				/*TexelBuffer redPixelTexels;
				redPixelTexels.channels = 4;
				redPixelTexels.width = 1;
				redPixelTexels.height = 1;
				redPixelTexels.texels = { 255, 0, 0, 255 };

				SampledTexture redPixelUnique;
				redPixelUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					redPixelTexels,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				//SampledTextureHandle redPixel = SampledTextureHandle(redPixelUnique);
				SampledTextureHandle redPixel = s_resourceManager.Register("selectedOutlineColorTexture", std::move(redPixelUnique));
				uint32 redPixelIndex = RegisterTexture(redPixel);*/

				Material selectedOutlineMaterial;
				selectedOutlineMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::Red));
				selectedOutlineMaterial.materialInfo.receiveLights = false;
				selectedOutlineMaterial.materialInfo.correctGamma = true;
				selectedOutlineMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eFront);
				selectedOutlineMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				selectedOutlineMaterial.materialPipelineInfo.SetDepthTest(true);
				//selectedOutlineMaterial.materialPipelineInfo.SetDepthWriteEnabled(false);
				s_resourceManager.materialManager.CreateMaterial(selectedOutlineMaterial, sb::ShaderBuilder::MakeSelectedOutlineShaderProgram);
				s_selectedOutlineMaterialIndex = RegisterMaterial(std::move(selectedOutlineMaterial));
			}

			#ifdef WITH_EDITOR
			{
				Mesh quadMesh;

				std::vector<uint8> bytes(4 * VertexPTNTaH::Size);

				{
					VectorView<VertexPTNTaH> vertices(bytes);
					vertices[0].Get<PositionXYZ, Vector3>() = { -1, 1, 0 };
					vertices[0].Get<TextureUV, Vector2>() = { 0, 1 };
					vertices[0].Get<NormalXYZ, Vector3>() = { 0, 0, 1 };
					vertices[0].Get<TangentXYZ, Vector3>() = { 1, 0, 0 };
					vertices[0].Get<Handedness, float>() = 1;

					vertices[1].Get<PositionXYZ, Vector3>() = { 1, 1, 0 };
					vertices[1].Get<TextureUV, Vector2>() = { 1, 1 };
					vertices[1].Get<NormalXYZ, Vector3>() = { 0, 0, 1 };
					vertices[1].Get<TangentXYZ, Vector3>() = { 1, 0, 0 };
					vertices[1].Get<Handedness, float>() = 1;

					vertices[2].Get<PositionXYZ, Vector3>() = { -1, -1, 0 };
					vertices[2].Get<TextureUV, Vector2>() = { 0, 0 };
					vertices[2].Get<NormalXYZ, Vector3>() = { 0, 0, 1 };
					vertices[2].Get<TangentXYZ, Vector3>() = { 1, 0, 0 };
					vertices[2].Get<Handedness, float>() = 1;

					vertices[3].Get<PositionXYZ, Vector3>() = { 1, -1, 0 };
					vertices[3].Get<TextureUV, Vector2>() = { 1, 0 };
					vertices[3].Get<NormalXYZ, Vector3>() = { 0, 0, 1 };
					vertices[3].Get<TangentXYZ, Vector3>() = { 1, 0, 0 };
					vertices[3].Get<Handedness, float>() = 1;

					quadMesh.SetVertices(bytes);
				}

				quadMesh.indexType = vk::IndexType::eUint16;

				bytes.resize(6 * sizeof(uint16));

				for (uint32 i{ 0 }; i < bytes.size(); i++)
					bytes[i] = 0;

				{
					VectorView<uint16> indices(bytes);
					indices[0] = 0;
					indices[1] = 1;
					indices[2] = 2;
					indices[3] = 1;
					indices[4] = 3;
					indices[5] = 2;

					quadMesh.SetIndices(bytes);
				}

				quadMesh.Flush();

				Mesh* quadMeshPtr = s_resourceManager.Register("primitive:VerticalQuad", std::move(quadMesh));
				s_quadMeshIndex = RegisterMesh(quadMeshPtr);
			}

			{
				//vk::CommandBufferAllocateInfo cpyCmdBufAllocInfo(Context::GetCommandPool(), vk::CommandBufferLevel::ePrimary, 1);
				//vk::UniqueCommandBuffer copyCmdBuffer(
				//	std::move(Context::GetDevice().Get().allocateCommandBuffersUnique(cpyCmdBufAllocInfo)[0])
				//);

				SampledTextureHandle lightGizmoTexture;
				TexelBuffer texelBuffer = sdk::TextureLoader::Load("assetsEditor/gizmos/light-gizmo.png");
				SampledTexture lightGizmoTextureUnique;
				lightGizmoTextureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					std::move(texelBuffer),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				lightGizmoTexture = s_resourceManager.Register("lightGizmoTexture", std::move(lightGizmoTextureUnique));
				uint32 lightGizmoTextureIndex = RegisterTexture(lightGizmoTexture);

				Material lightGizmoMaterial;
				lightGizmoMaterial.SetDiffuse(lightGizmoTextureIndex);
				lightGizmoMaterial.materialInfo.receiveLights = false;
				lightGizmoMaterial.materialInfo.isTransparent = true;
				lightGizmoMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
				lightGizmoMaterial.materialPipelineInfo.SetDepthWriteEnabled(false);
				lightGizmoMaterial.materialPipelineInfo.SetDepthTest(false);
				s_resourceManager.materialManager.CreateMaterial(lightGizmoMaterial, sb::ShaderBuilder::MakeShaderProgram);
				s_lightGizmoMaterialIndex = RegisterMaterial(std::move(lightGizmoMaterial));

				SampledTextureHandle sunGizmoTexture;
				texelBuffer = sdk::TextureLoader::Load("assetsEditor/gizmos/sun-gizmo.png");
				SampledTexture sunGizmoTextureUnique;
				sunGizmoTextureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					std::move(texelBuffer),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				sunGizmoTexture = s_resourceManager.Register("sunGizmoTexture", std::move(sunGizmoTextureUnique));
				uint32 sunGizmoTextureIndex = RegisterTexture(sunGizmoTexture);

				Material sunGizmoMaterial;
				sunGizmoMaterial.SetDiffuse(sunGizmoTextureIndex);
				sunGizmoMaterial.materialInfo.receiveLights = false;
				sunGizmoMaterial.materialInfo.isTransparent = true;
				sunGizmoMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
				sunGizmoMaterial.materialPipelineInfo.SetDepthWriteEnabled(false);
				sunGizmoMaterial.materialPipelineInfo.SetDepthTest(false);
				s_resourceManager.materialManager.CreateMaterial(sunGizmoMaterial, sb::ShaderBuilder::MakeShaderProgram);
				s_sunGizmoMaterialIndex = RegisterMaterial(std::move(sunGizmoMaterial));

				SampledTextureHandle cameraGizmoTexture;
				texelBuffer = sdk::TextureLoader::Load("assetsEditor/gizmos/camera-gizmo.png");
				SampledTexture cameraGizmoTextureUnique;
				cameraGizmoTextureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					std::move(texelBuffer),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				cameraGizmoTexture = s_resourceManager.Register("cameraGizmoTexture", std::move(cameraGizmoTextureUnique));
				uint32 cameraGizmoTextureIndex = RegisterTexture(cameraGizmoTexture);

				Material cameraGizmoMaterial;
				cameraGizmoMaterial.SetDiffuse(cameraGizmoTextureIndex);
				cameraGizmoMaterial.materialInfo.receiveLights = false;
				cameraGizmoMaterial.materialInfo.isTransparent = true;
				cameraGizmoMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
				cameraGizmoMaterial.materialPipelineInfo.SetDepthWriteEnabled(false);
				cameraGizmoMaterial.materialPipelineInfo.SetDepthTest(false);
				s_resourceManager.materialManager.CreateMaterial(cameraGizmoMaterial, sb::ShaderBuilder::MakeShaderProgram);
				s_cameraGizmoMaterialIndex = RegisterMaterial(std::move(cameraGizmoMaterial));

				Material debugMaterial;
				debugMaterial.materialPipelineInfo.SetDepthTest(false);
				debugMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eLine);
				debugMaterial.materialPipelineInfo.SetPrimitiveTopology(vk::PrimitiveTopology::eLineList);
				debugMaterial.materialInfo.receiveLights = false;
				debugMaterial.materialInfo.isTransparent = true;
				s_resourceManager.materialManager.CreateMaterial(debugMaterial, sb::ShaderBuilder::MakeShaderProgram);
				s_debugMaterialIndex = RegisterMaterial(std::move(debugMaterial));



				Material redUnlitMaterial;
				redUnlitMaterial.materialInfo.receiveLights = false;
				redUnlitMaterial.materialInfo.correctGamma = false;
				redUnlitMaterial.materialInfo.isTransparent = false;
				redUnlitMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eBack);
				redUnlitMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				//redUnlitMaterial.materialPipelineInfo.SetDepthTest(false);
				redUnlitMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::Red));
				s_resourceManager.materialManager.CreateMaterial(redUnlitMaterial, sb::ShaderBuilder::MakeShaderProgram);
				s_redUnlitMaterialIndex = RegisterMaterial(std::move(redUnlitMaterial));

				Material greenUnlitMaterial;
				greenUnlitMaterial.materialInfo.receiveLights = false;
				greenUnlitMaterial.materialInfo.correctGamma = false;
				greenUnlitMaterial.materialInfo.isTransparent = false;
				greenUnlitMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eBack);
				greenUnlitMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				//greenUnlitMaterial.materialPipelineInfo.SetDepthTest(false);
				greenUnlitMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::Green));
				s_resourceManager.materialManager.CreateMaterial(greenUnlitMaterial, sb::ShaderBuilder::MakeShaderProgram);
				s_greenUnlitMaterialIndex = RegisterMaterial(std::move(greenUnlitMaterial));

				Material blueUnlitMaterial;
				blueUnlitMaterial.materialInfo.receiveLights = false;
				blueUnlitMaterial.materialInfo.correctGamma = false;
				blueUnlitMaterial.materialInfo.isTransparent = false;
				blueUnlitMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eBack);
				blueUnlitMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				//blueUnlitMaterial.materialPipelineInfo.SetDepthTest(false);
				blueUnlitMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::Blue));
				s_resourceManager.materialManager.CreateMaterial(blueUnlitMaterial, sb::ShaderBuilder::MakeShaderProgram);
				s_blueUnlitMaterialIndex = RegisterMaterial(std::move(blueUnlitMaterial));

				Material redLandmarkMaterial;
				redLandmarkMaterial.materialInfo.receiveLights = true;
				redLandmarkMaterial.materialInfo.correctGamma = false;
				redLandmarkMaterial.materialInfo.isTransparent = false;
				redLandmarkMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eBack);
				redLandmarkMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				redLandmarkMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::Red));
				s_resourceManager.materialManager.CreateMaterial(redLandmarkMaterial, sb::ShaderBuilder::MakeRimShaderProgram);
				s_redLandmarkMaterialIndex = RegisterMaterial(std::move(redLandmarkMaterial));

				Material greenLandmarkMaterial;
				greenLandmarkMaterial.materialInfo.receiveLights = true;
				greenLandmarkMaterial.materialInfo.correctGamma = false;
				greenLandmarkMaterial.materialInfo.isTransparent = false;
				greenLandmarkMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eBack);
				greenLandmarkMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				greenLandmarkMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::Green));
				s_resourceManager.materialManager.CreateMaterial(greenLandmarkMaterial, sb::ShaderBuilder::MakeRimShaderProgram);
				s_greenLandmarkMaterialIndex = RegisterMaterial(std::move(greenLandmarkMaterial));

				Material blueLandmarkMaterial;
				blueLandmarkMaterial.materialInfo.receiveLights = true;
				blueLandmarkMaterial.materialInfo.correctGamma = false;
				blueLandmarkMaterial.materialInfo.isTransparent = false;
				blueLandmarkMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eBack);
				blueLandmarkMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				blueLandmarkMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::Blue));
				s_resourceManager.materialManager.CreateMaterial(blueLandmarkMaterial, sb::ShaderBuilder::MakeRimShaderProgram);
				s_blueLandmarkMaterialIndex = RegisterMaterial(std::move(blueLandmarkMaterial));

				Material whiteLandmarkMaterial;
				whiteLandmarkMaterial.materialInfo.receiveLights = true;
				whiteLandmarkMaterial.materialInfo.correctGamma = false;
				whiteLandmarkMaterial.materialInfo.isTransparent = false;
				whiteLandmarkMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eBack);
				whiteLandmarkMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				whiteLandmarkMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::White));
				s_resourceManager.materialManager.CreateMaterial(whiteLandmarkMaterial, sb::ShaderBuilder::MakeRimShaderProgram);
				s_whiteLandmarkMaterialIndex = RegisterMaterial(std::move(whiteLandmarkMaterial));

				Material yellowLandmarkMaterial;
				yellowLandmarkMaterial.materialInfo.receiveLights = false;
				yellowLandmarkMaterial.materialInfo.correctGamma = false;
				yellowLandmarkMaterial.materialInfo.isTransparent = true;
				yellowLandmarkMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
				yellowLandmarkMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				yellowLandmarkMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::YellowHalfTransparent));
				s_resourceManager.materialManager.CreateMaterial(yellowLandmarkMaterial, sb::ShaderBuilder::MakeRimShaderProgram);
				s_yellowLandmarkMaterialIndex = RegisterMaterial(std::move(yellowLandmarkMaterial));

				Material cyanLandmarkMaterial;
				cyanLandmarkMaterial.materialInfo.receiveLights = false;
				cyanLandmarkMaterial.materialInfo.correctGamma = false;
				cyanLandmarkMaterial.materialInfo.isTransparent = true;
				cyanLandmarkMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
				cyanLandmarkMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				cyanLandmarkMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::CyanHalfTransparent));
				s_resourceManager.materialManager.CreateMaterial(cyanLandmarkMaterial, sb::ShaderBuilder::MakeRimShaderProgram);
				s_cyanLandmarkMaterialIndex = RegisterMaterial(std::move(cyanLandmarkMaterial));

				Material magentaLandmarkMaterial;
				magentaLandmarkMaterial.materialInfo.receiveLights = false;
				magentaLandmarkMaterial.materialInfo.correctGamma = false;
				magentaLandmarkMaterial.materialInfo.isTransparent = true;
				magentaLandmarkMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
				magentaLandmarkMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
				magentaLandmarkMaterial.SetDiffuse(static_cast<uint32>(DefaultTextureIndex::MagentaHalfTransparent));
				s_resourceManager.materialManager.CreateMaterial(magentaLandmarkMaterial, sb::ShaderBuilder::MakeRimShaderProgram);
				s_magentaLandmarkMaterialIndex = RegisterMaterial(std::move(magentaLandmarkMaterial));

			}
			uint32 landmarkDepthIndex;
			#endif

			s_opaqueRQ.ResizeCommandBuffers(1);

			uint32 depthIndex;
			//uint32 blur1Index;
			//uint32 blur2Index;
			{
				AttachmentDescription desc;
				desc.extent = vk::Extent3D(512, 512, 1);
				desc.hasWindowExtent = true;

				desc.aspect = vk::ImageAspectFlagBits::eColor;
				desc.format = RenderSurface::GetPresentFormat();
				desc.isPresentAttachment = true;
				desc.layout = vk::ImageLayout::eTransferSrcOptimal;
				s_presentImageIndex = s_frameResourcesManager.AddDescription(desc);

				desc.aspect = vk::ImageAspectFlagBits::eDepth;
				desc.format = RenderSurface::GetDepthFormat();
				desc.isPresentAttachment = false;
				desc.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
				depthIndex = s_frameResourcesManager.AddDescription(desc);

				desc.aspect = vk::ImageAspectFlagBits::eColor;
				desc.format = vk::Format::eR8G8B8A8Unorm;
				desc.layout = vk::ImageLayout::eColorAttachmentOptimal;
				s_firstLightScatterImageIndex = s_frameResourcesManager.AddDescription(desc);

				#ifdef WITH_EDITOR
				//auto props = Context::GetDevice().GetPhysicalDevice().getFormatProperties(vk::Format::eR32G32Uint);

				//auto linear = vk::to_string(props.linearTilingFeatures);
				//auto optimal = vk::to_string(props.optimalTilingFeatures);
				//auto buffer = vk::to_string(props.bufferFeatures);

				//LOG_LOG(linear);
				//LOG_LOG(optimal);
				//LOG_LOG(buffer);

				desc.isPresentAttachment = true;
				desc.format = vk::Format::eR32G32Uint;
				desc.layout = vk::ImageLayout::eTransferSrcOptimal;
				s_componentIdImageIndex = s_frameResourcesManager.AddDescription(desc);

				desc.aspect = vk::ImageAspectFlagBits::eDepth;
				desc.format = RenderSurface::GetDepthFormat();
				desc.isPresentAttachment = false;
				desc.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
				landmarkDepthIndex = s_frameResourcesManager.AddDescription(desc);
				#endif

				//desc.aspect = vk::ImageAspectFlagBits::eColor;
				//desc.format = vk::Format::eR8G8B8A8Unorm;
				//desc.isPresentAttachment = false;
				//desc.layout = vk::ImageLayout::eColorAttachmentOptimal;
				//blur1Index = m_frameResourcesManager.AddDescription(desc);
				//blur2Index = m_frameResourcesManager.AddDescription(desc);
			}

			// first light scattering
			{
				s_firstLightScatteringPassIndex = s_passes.size();

				s_firstLightScatteringPass = s_framegraph.AddPass(
					s_frameResourcesManager,
					[&](std::vector<AttachmentProcess>& inputs, std::vector<AttachmentProcess>& outputs, vk::Extent2D& extent, vk::Rect2D& renderArea)
					{
						AttachmentProcess proc;
						proc.loadOp = vk::AttachmentLoadOp::eClear;

						proc.attachmentIndex = s_firstLightScatterImageIndex;
						proc.clearValue = vk::ClearColorValue(Color4f::BlackTransparent());
						proc.colorBlendState.blendEnable = 0;
						proc.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
						outputs.push_back(proc);

						proc.attachmentIndex = depthIndex;
						proc.clearValue = vk::ClearDepthStencilValue(1.0f);
						proc.colorBlendState.blendEnable = 0;
						proc.initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						outputs.push_back(proc);

						extent = vk::Extent2D(512, 512);
						renderArea.extent = vk::Extent2D(512, 512);
					},
					nullptr,
					[&](vk::CommandBuffer commandBuffer)
					{
						s_lightScatteringEmitterRQ.DrawSorted(commandBuffer);
						s_lightScatteringObstacleRQ.DrawSorted(commandBuffer);
					},
					nullptr
				);

				s_passes.push_back(s_firstLightScatteringPass);
			}

			// forward
			{
				s_forwardPassIndex = s_passes.size();

				s_forwardPass = s_framegraph.AddPass(
					s_frameResourcesManager,
					[&](std::vector<AttachmentProcess>& inputs, std::vector<AttachmentProcess>& outputs, vk::Extent2D& extent, vk::Rect2D& renderArea)
					{
						AttachmentProcess proc;
						proc.loadOp = vk::AttachmentLoadOp::eClear;

						proc.attachmentIndex = s_presentImageIndex;
						proc.clearValue = vk::ClearColorValue(Color4f::DarkSublime());
						proc.colorBlendState.blendEnable = 0;
						proc.initialLayout = vk::ImageLayout::eTransferSrcOptimal;
						proc.subpassLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
						outputs.push_back(proc);

						proc.attachmentIndex = depthIndex;
						proc.clearValue = vk::ClearDepthStencilValue(1.0f);
						proc.colorBlendState.blendEnable = 0;
						proc.initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						outputs.push_back(proc);

						#ifdef WITH_EDITOR
						proc.attachmentIndex = s_componentIdImageIndex;
						proc.clearValue = vk::ClearColorValue(std::array<uint32, 4>({ ~0, ~0, ~0, ~0 }));
						proc.colorBlendState.blendEnable = 0;
						proc.initialLayout = vk::ImageLayout::eTransferSrcOptimal;
						proc.subpassLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
						outputs.push_back(proc);
						#endif

						extent = vk::Extent2D(512, 512);
						renderArea.extent = vk::Extent2D(512, 512);
					},
					nullptr,
					[&](vk::CommandBuffer commandBuffer)
					{
						s_opaqueRQ.DrawSorted(commandBuffer);

						if (s_skybox)
						{
							Material& material = s_materials[s_skybox->m_materialIndex];
							Mesh* mesh = s_meshes[s_skybox->m_meshIndex];

							ShaderHandle shader = GetOrCreateInstanceMaterialShader({ s_skybox->m_materialIndex, s_skybox->m_meshIndex, 1 });

							shader.Bind(commandBuffer);
							Material::BindDescriptorSet(
								commandBuffer,
								shader.pipelineLayout,
								{ s_skybox->m_geometryDset, material.GetDescriptorSet() },
								s_skybox->m_dynamicBlock.GetOffset()
							);
							mesh->Draw(commandBuffer);
						}
					},
					nullptr
				);

				s_passes.push_back(s_forwardPass);
			}

			// last light scattering
			{
				s_lastLightScatteringPassIndex = s_passes.size();

				s_lastLightScatteringPass = s_framegraph.AddPass(
					s_frameResourcesManager,
					[&](std::vector<AttachmentProcess>& inputs, std::vector<AttachmentProcess>& outputs, vk::Extent2D& extent, vk::Rect2D& renderArea)
					{
						AttachmentProcess proc;

						proc.loadOp = vk::AttachmentLoadOp::eLoad;
						proc.clearValue = vk::ClearColorValue(Color4f::BlackTransparent());
						proc.attachmentIndex = s_presentImageIndex;
						proc.colorBlendState.blendEnable = 1;
						// proc.colorBlendState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB;
						proc.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
						outputs.push_back(proc);

						extent = vk::Extent2D(512, 512);
						renderArea.extent = vk::Extent2D(512, 512);
					},
					nullptr,
					[&, sceenQuadIndex](vk::CommandBuffer commandBuffer)
					{
						const MaterialHandle& material = s_materials[s_lastLightScatteringMaterialIndex];
						ShaderHandle shader = GetOrCreateInstanceMaterialShader({ s_lastLightScatteringMaterialIndex, sceenQuadIndex, s_lastLightScatteringPassIndex });

						shader.Bind(commandBuffer);
						Material::BindDescriptorSet(
							commandBuffer,
							shader.pipelineLayout,
							{ material.descriptorSet },
							{}
						);
						s_sceenQuad.Draw(commandBuffer);
					},
					nullptr
				);

				s_passes.push_back(s_lastLightScatteringPass);
			}

			// transparency
			{
				s_transparencyPassIndex = s_passes.size();

				s_transparencyPass = s_framegraph.AddPass(
					s_frameResourcesManager,
					[&](std::vector<AttachmentProcess>& inputs, std::vector<AttachmentProcess>& outputs, vk::Extent2D& extent, vk::Rect2D& renderArea)
					{
						AttachmentProcess proc;
						proc.loadOp = vk::AttachmentLoadOp::eLoad;

						proc.attachmentIndex = s_presentImageIndex;
						proc.colorBlendState.blendEnable = 1;
						proc.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eColorAttachmentOptimal;
						// proc.finalLayout = vk::ImageLayout::eTransferSrcOptimal;
						proc.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
						outputs.push_back(proc);

						proc.attachmentIndex = depthIndex;
						proc.colorBlendState.blendEnable = 0;
						proc.initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						outputs.push_back(proc);

						#ifdef WITH_EDITOR
						proc.attachmentIndex = s_componentIdImageIndex;
						proc.colorBlendState.blendEnable = 0;
						proc.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eColorAttachmentOptimal;
						// proc.finalLayout = vk::ImageLayout::eTransferSrcOptimal;
						proc.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
						outputs.push_back(proc);
						#endif

						extent = vk::Extent2D(512, 512);
						renderArea.extent = vk::Extent2D(512, 512);
					},
					nullptr,
					[&](vk::CommandBuffer commandBuffer)
					{
						s_transparentRQ.DrawSorted(commandBuffer);
					},
					nullptr
				);

				s_passes.push_back(s_transparencyPass);
			}

			#ifdef WITH_EDITOR
			// landmarks forward
			{
				s_landmarkForwardPassIndex = s_passes.size();

				s_landmarkForwardPass = s_framegraph.AddPass(
					s_frameResourcesManager,
					[&](std::vector<AttachmentProcess>& inputs, std::vector<AttachmentProcess>& outputs, vk::Extent2D& extent, vk::Rect2D& renderArea)
					{
						AttachmentProcess proc;

						proc.loadOp = vk::AttachmentLoadOp::eLoad;
						proc.attachmentIndex = s_presentImageIndex;
						proc.colorBlendState.blendEnable = 0;
						// proc.colorBlendState.blendEnable = 1;
						// proc.colorBlendState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB;
						proc.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eTransferSrcOptimal;
						outputs.push_back(proc);

						proc.loadOp = vk::AttachmentLoadOp::eClear;
						proc.attachmentIndex = landmarkDepthIndex;
						proc.clearValue = vk::ClearDepthStencilValue(1.0f);
						proc.colorBlendState.blendEnable = 0;
						proc.initialLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
						outputs.push_back(proc);

						proc.loadOp = vk::AttachmentLoadOp::eLoad;
						proc.attachmentIndex = s_componentIdImageIndex;
						proc.clearValue = vk::ClearColorValue(std::array<uint32, 4>({ 0, 0, 0, 0 }));
						proc.colorBlendState.blendEnable = 0;
						proc.initialLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.subpassLayout = vk::ImageLayout::eColorAttachmentOptimal;
						proc.finalLayout = vk::ImageLayout::eTransferSrcOptimal;
						outputs.push_back(proc);

						extent = vk::Extent2D(512, 512);
						renderArea.extent = vk::Extent2D(512, 512);
					},
					nullptr,
					[&](vk::CommandBuffer commandBuffer)
					{
						if (s_xLandmark.isActive == true && s_xLandmark.meshRenderer != nullptr)
						{
							Material& material = s_materials[s_xLandmark.meshRenderer->m_materialIndex];
							Mesh* mesh = s_meshes[s_xLandmark.meshRenderer->m_meshIndex];

							ShaderHandle shader = GetOrCreateInstanceMaterialShader({ s_xLandmark.meshRenderer->m_materialIndex, s_xLandmark.meshRenderer->m_meshIndex, 1 });

							shader.Bind(commandBuffer);
							Material::BindDescriptorSet(
								commandBuffer,
								shader.pipelineLayout,
								{ s_xLandmark.meshRenderer->m_geometryDset, material.GetDescriptorSet() },
								s_xLandmark.meshRenderer->m_dynamicBlock.GetOffset()
							);
							mesh->Draw(commandBuffer);
						}
						if (s_yLandmark.isActive == true && s_yLandmark.meshRenderer != nullptr)
						{
							Material& material = s_materials[s_yLandmark.meshRenderer->m_materialIndex];
							Mesh* mesh = s_meshes[s_yLandmark.meshRenderer->m_meshIndex];

							ShaderHandle shader = GetOrCreateInstanceMaterialShader({ s_yLandmark.meshRenderer->m_materialIndex, s_yLandmark.meshRenderer->m_meshIndex, 1 });

							shader.Bind(commandBuffer);
							Material::BindDescriptorSet(
								commandBuffer,
								shader.pipelineLayout,
								{ s_yLandmark.meshRenderer->m_geometryDset, material.GetDescriptorSet() },
								s_yLandmark.meshRenderer->m_dynamicBlock.GetOffset()
							);
							mesh->Draw(commandBuffer);
						}
						if (s_zLandmark.isActive == true && s_zLandmark.meshRenderer != nullptr)
						{
							Material& material = s_materials[s_zLandmark.meshRenderer->m_materialIndex];
							Mesh* mesh = s_meshes[s_zLandmark.meshRenderer->m_meshIndex];

							ShaderHandle shader = GetOrCreateInstanceMaterialShader({ s_zLandmark.meshRenderer->m_materialIndex, s_zLandmark.meshRenderer->m_meshIndex, 1 });

							shader.Bind(commandBuffer);
							Material::BindDescriptorSet(
								commandBuffer,
								shader.pipelineLayout,
								{ s_zLandmark.meshRenderer->m_geometryDset, material.GetDescriptorSet() },
								s_zLandmark.meshRenderer->m_dynamicBlock.GetOffset()
							);
							mesh->Draw(commandBuffer);
						}
						if (s_cLandmark.isActive == true && s_cLandmark.meshRenderer != nullptr)
						{
							Material& material = s_materials[s_cLandmark.meshRenderer->m_materialIndex];
							Mesh* mesh = s_meshes[s_cLandmark.meshRenderer->m_meshIndex];

							ShaderHandle shader = GetOrCreateInstanceMaterialShader({ s_cLandmark.meshRenderer->m_materialIndex, s_cLandmark.meshRenderer->m_meshIndex, 1 });

							shader.Bind(commandBuffer);
							Material::BindDescriptorSet(
								commandBuffer,
								shader.pipelineLayout,
								{ s_cLandmark.meshRenderer->m_geometryDset, material.GetDescriptorSet() },
								s_cLandmark.meshRenderer->m_dynamicBlock.GetOffset()
							);
							mesh->Draw(commandBuffer);
						}
						if (s_xyLandmark.isActive == true && s_xyLandmark.meshRenderer != nullptr)
						{
							Material& material = s_materials[s_xyLandmark.meshRenderer->m_materialIndex];
							Mesh* mesh = s_meshes[s_xyLandmark.meshRenderer->m_meshIndex];

							ShaderHandle shader = GetOrCreateInstanceMaterialShader({ s_xyLandmark.meshRenderer->m_materialIndex, s_xyLandmark.meshRenderer->m_meshIndex, 1 });

							shader.Bind(commandBuffer);
							Material::BindDescriptorSet(
								commandBuffer,
								shader.pipelineLayout,
								{ s_xyLandmark.meshRenderer->m_geometryDset, material.GetDescriptorSet() },
								s_xyLandmark.meshRenderer->m_dynamicBlock.GetOffset()
							);
							mesh->Draw(commandBuffer);
						}
						if (s_yzLandmark.isActive == true && s_yzLandmark.meshRenderer != nullptr)
						{
							Material& material = s_materials[s_yzLandmark.meshRenderer->m_materialIndex];
							Mesh* mesh = s_meshes[s_yzLandmark.meshRenderer->m_meshIndex];

							ShaderHandle shader = GetOrCreateInstanceMaterialShader({ s_yzLandmark.meshRenderer->m_materialIndex, s_yzLandmark.meshRenderer->m_meshIndex, 1 });

							shader.Bind(commandBuffer);
							Material::BindDescriptorSet(
								commandBuffer,
								shader.pipelineLayout,
								{ s_yzLandmark.meshRenderer->m_geometryDset, material.GetDescriptorSet() },
								s_yzLandmark.meshRenderer->m_dynamicBlock.GetOffset()
							);
							mesh->Draw(commandBuffer);
						}
						if (s_zxLandmark.isActive == true && s_zxLandmark.meshRenderer != nullptr)
						{
							Material& material = s_materials[s_zxLandmark.meshRenderer->m_materialIndex];
							Mesh* mesh = s_meshes[s_zxLandmark.meshRenderer->m_meshIndex];

							ShaderHandle shader = GetOrCreateInstanceMaterialShader({ s_zxLandmark.meshRenderer->m_materialIndex, s_zxLandmark.meshRenderer->m_meshIndex, 1 });

							shader.Bind(commandBuffer);
							Material::BindDescriptorSet(
								commandBuffer,
								shader.pipelineLayout,
								{ s_zxLandmark.meshRenderer->m_geometryDset, material.GetDescriptorSet() },
								s_zxLandmark.meshRenderer->m_dynamicBlock.GetOffset()
							);
							mesh->Draw(commandBuffer);
						}
					},
					[&](vk::CommandBuffer commandBuffer)
					{
						vk::ImageSubresourceRange range(vk::ImageAspectFlagBits::eColor, 0, 1, 0, VK_REMAINING_ARRAY_LAYERS);

						vk::ImageMemoryBarrier barrier{};
						barrier.srcAccessMask = vk::AccessFlagBits::eMemoryRead;
						barrier.dstAccessMask = vk::AccessFlagBits::eMemoryWrite | vk::AccessFlagBits::eColorAttachmentWrite;
						barrier.oldLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
						barrier.newLayout = vk::ImageLayout::eColorAttachmentOptimal;
						barrier.srcQueueFamilyIndex = ~0u;
						barrier.dstQueueFamilyIndex = ~0u;
						barrier.image = s_frameResourcesManager.GetImage(s_firstLightScatterImageIndex).GetImage();
						barrier.subresourceRange = range;

						commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eFragmentShader, vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::DependencyFlagBits::eByRegion, {}, {}, barrier);
					}
				);

				s_passes.push_back(s_forwardPass);
			}
			#endif

			//s_resourceManager.materialManager.CreateMaterial(s_3dQuadModelInstance.model->material, s_firstLightScatteringPass, VertexPTNTaH::Layout, sb::ShaderBuilder::MakeFirstLightScatteringWhiteLightShaderProgram);
			////s_resourceManager.materialManager.CreateMaterial(s_forwardWhiteLightMaterial, s_forwardPass, VertexPTNTaH::Layout, sb::ShaderBuilder::MakeFirstLightScatteringWhiteLightShaderProgram);
			//s_resourceManager.materialManager.CreateScreenMaterial(s_lastLightScatteringMaterial, s_lastLightScatteringPass, VertexP2T::Layout, sb::ShaderBuilder::MakeLastLightScatteringShaderProgram);

			Material lastLightScatteringMaterial;
			lastLightScatteringMaterial.materialInfo.isScreenMaterial = true;

			s_resourceManager.materialManager.CreateMaterial(lastLightScatteringMaterial, sb::ShaderBuilder::MakeLastLightScatteringShaderProgram);

			//const Material& lastLightScatteringMaterial = s_materials[s_lastLightScatteringMaterialIndex];
			lastLightScatteringMaterial.SetSampledTexture(s_frameResourcesManager.GetImage(s_firstLightScatterImageIndex), 0);
			lastLightScatteringMaterial.SetUBO(s_lightScatteringUBO, 1);

			s_lastLightScatteringMaterialIndex = RegisterMaterial(std::move(lastLightScatteringMaterial));

			Material firstLightScatteringWhiteLightMaterial;
			s_resourceManager.materialManager.CreateMaterial(firstLightScatteringWhiteLightMaterial, sb::ShaderBuilder::MakeFirstLightScatteringWhiteLightShaderProgram);
			s_firstLightScatteringWhiteLightMaterialIndex = RegisterMaterial(std::move(firstLightScatteringWhiteLightMaterial));

			Material firstLightScatteringObstacleMaterial;
			s_resourceManager.materialManager.CreateMaterial(firstLightScatteringObstacleMaterial, sb::ShaderBuilder::MakeFirstLightScatteringObstacleShaderProgram);
			s_firstLightScatteringObstacleMaterialIndex = RegisterMaterial(std::move(firstLightScatteringObstacleMaterial));

			//s_presentImageIndex = idIndex;
		}

		void RenderSystem::Start(RenderWindow& window, Camera& editorCamera, std::atomic_bool& shouldUpdate, const std::atomic_bool& standardMode, const std::atomic_bool& landmarkControlAbsolute, const std::atomic_int& currentTool, Component** ptrToSelected)
		{
			static std::function<void()> func = [&shouldUpdate, &standardMode, &landmarkControlAbsolute, &currentTool, &window, &editorCamera, ptrToSelected]()
			{
				LOG_LOG("Render thread id: %u", std::this_thread::get_id());
				while (!BaseApplication::ShouldQuit())
				{
					if (shouldUpdate)
					{
						//static Chrono chrono;
						//chrono.Restart();

						if (standardMode)
							Update();
						else
						{
							if(ptrToSelected)
								UpdateInEditor(*ptrToSelected, landmarkControlAbsolute, currentTool);
							else
								UpdateInEditor(nullptr, landmarkControlAbsolute, currentTool);
						}

						shouldUpdate = false;

						// Force Resize
						//TODO Move
						Context::GetDevice().WaitGraphicsIdle();

						if (InputManager::GetKeyDown(KeyCode::Numpad9) || window->ShouldResize())
							window.Resize(window->GetWidth(), window->GetHeight());

						static RenderingResources m_renderingResources;
						m_renderingResources = window.GetNewRenderingResources();

						if (standardMode)
							Draw(m_renderingResources.commandBuffer);
						else
							DrawInEditor(m_renderingResources.commandBuffer, editorCamera);

						window.Draw(GetPresentImage().GetImage());

						//LOG_LOG("Render System took %lfs to update", chrono.GetElapsedTime<Chrono::Seconds>());
					}
					else
					{
						InstantiatePendingComponents();
						GarbageCollect();
						std::this_thread::yield();
					}
				}

				LOG_LOG("Exitting Render thread")
			};

			std::thread thr(func);
			thr.detach();
		}

		void RenderSystem::UnInit()
		{
			//s_renderScene.Destroy();

			s_sceenQuad.Clear();
			s_3dQuad.Clear();

			s_xLandmark.meshRenderer.reset();
			s_yLandmark.meshRenderer.reset();
			s_zLandmark.meshRenderer.reset();
			s_cLandmark.meshRenderer.reset();
			s_xyLandmark.meshRenderer.reset();
			s_yzLandmark.meshRenderer.reset();
			s_zxLandmark.meshRenderer.reset();

			s_debug3DLines.clear();
			s_meshRenderers.clear();
			s_cameras.clear();
			s_pointLights.clear();
			s_directionalLights.clear();
			s_text2Ds.clear();
			s_billboards.clear();
			s_materials.clear();
			s_meshes.clear();
			s_passes.clear();
			s_textures.clear();
			s_instanceMaterials.clear();
			s_copyCommandBuffer.reset();
			s_modelUBDLayout.reset();

			s_viewUBO.~UniformBuffer();
			s_componentIdSubBuffer.~SubBuffer();
			s_lightsUBO.~UniformBuffer();
			s_lightScatteringUBO.~UniformBuffer();

			s_gizmos2D.clear();
			s_gizmos3D.clear();

			s_modelUBDGroups.clear();

			s_opaqueRQ.~RenderQueue();
			s_transparentRQ.~RenderQueue();
			s_lightScatteringEmitterRQ.~RenderQueue();
			s_lightScatteringObstacleRQ.~RenderQueue();

			s_framegraph.~Framegraph();
			s_resourceManager.Clear();
			s_frameResourcesManager.~FrameResourcesManager();
			//s_sceenQuad.~Mesh();
			//s_3dQuad.~Mesh();
		}

		void RenderSystem::InstantiatePendingComponents()
		{
			s_instMutex.lock();
			for (auto component : s_compsToInstantiate)
			{
				if (InheritsOf(component->typeId, TypeInfo<MeshRenderer>::id))
					Register(reinterpret_cast<MeshRenderer*>(component));

				if (InheritsOf(component->typeId, TypeInfo<Skybox>::id))
					Register(reinterpret_cast<Skybox*>(component));

				if (InheritsOf(component->typeId, TypeInfo<Camera>::id))
					Register(reinterpret_cast<Camera*>(component));

				if (InheritsOf(component->typeId, TypeInfo<Light>::id))
					Register(reinterpret_cast<Light*>(component));

				if (InheritsOf(component->typeId, TypeInfo<Text2D>::id))
					Register(reinterpret_cast<Text2D*>(component));

				if (InheritsOf(component->typeId, TypeInfo<Billboard>::id))
					Register(reinterpret_cast<Billboard*>(component));

				//s_scene.AddActor(*(reinterpret_cast<PhysicalComponent*>(component)->m_body));
				//s_phComps.push_back(reinterpret_cast<PhysicalComponent*>(component));
			}

			s_compsToInstantiate.clear();
			s_instMutex.unlock();
		}

		void RenderSystem::GarbageCollect()
		{
			Context::GetDevice().WaitIdle();

			for (auto it = s_meshRenderers.begin(); it != s_meshRenderers.end(); ++it)
			{
				if (!(*it)->GarbageCollectionRequired())
					continue;

				(*it)->m_dynamicBlock.Free();
				(*it)->m_isUsed = false;

				s_meshRenderers.erase(it);

				break; //Lazy
			}

			for (auto it = s_cameras.begin(); it != s_cameras.end(); ++it)
			{
				if (!(*it)->GarbageCollectionRequired())
					continue;

				(*it)->m_isUsed = false;
				s_cameras.erase(it);

				break; //Lazy
			}

			for (auto it = s_pointLights.begin(); it != s_pointLights.end(); ++it)
			{
				if (!(*it)->GarbageCollectionRequired())
					continue;

				(*it)->m_isUsed = false;
				s_pointLights.erase(it);

				break; //Lazy
			}

			for (auto it = s_directionalLights.begin(); it != s_directionalLights.end(); ++it)
			{
				if (!(*it)->GarbageCollectionRequired())
					continue;

				(*it)->m_isUsed = false;
				s_directionalLights.erase(it);

				break; //Lazy
			}

			for (auto it = s_text2Ds.begin(); it != s_text2Ds.end(); ++it)
			{
				if (!(*it)->GarbageCollectionRequired())
					continue;

				(*it)->m_isUsed = false;
				s_text2Ds.erase(it);

				break; //Lazy
			}

			for (auto it = s_billboards.begin(); it != s_billboards.end(); ++it)
			{
				if (!(*it)->GarbageCollectionRequired())
					continue;

				(*it)->m_dynamicBlock.Free();
				(*it)->m_isUsed = false;

				s_billboards.erase(it);

				break; //Lazy
			}
		}

		void RenderSystem::LoadPrimitives()
		{
			ASSERT(s_meshes.size() != 0, "s_meshes is not empty!");

			s_meshes.resize(static_cast<size_t>(PrimitivesIndex::COUNT));

			//Quad
			s_meshes[0] = s_resourceManager.Register("primitive:Quad", Mesh::Quad::Create());
			s_meshes[1] = s_resourceManager.Register("debug:Quad", Mesh::DebugQuad::Create(1.0f));

			// Cube
			s_meshes[2] = s_resourceManager.Register("primitive:Cube", Mesh::Cube::Create());
			s_meshes[3] = s_resourceManager.Register("debug:Cube", Mesh::DebugCube::Create(1.0f));

			// Sphere
			s_meshes[4] = s_resourceManager.Register("primitive:UVSphere", Mesh::Sphere<>::CreateUVSphere());
			s_meshes[5] = s_resourceManager.Register("debug:Sphere", Mesh::DebugSphere::Create(0.5f));

			// Capsule
			s_meshes[6] = s_resourceManager.Register("primitive:Capsule", Mesh::Capsule<>::Create());
			s_meshes[7] = s_resourceManager.Register("debug:Capsule", Mesh::DebugCapsule::Create(0.5f, 1.0f));

			// Cylinder
			s_meshes[8] = s_resourceManager.Register("primitive:Cylinder", Mesh::Cylinder<>::Create());

			// Cone
			s_meshes[9] = s_resourceManager.Register("primitive:Cone", Mesh::Cone<>::Create(0.5f, 2.0f));

			// Hemisphere
			s_meshes[10] = s_resourceManager.Register("primitive:Hemiphere", Mesh::Hemisphere<>::Create(100.0f));

			// Skybox
			s_meshes[11] = s_resourceManager.Load<OBJLoader>("assets/cube/cube.obj", &aw::VertexP::Layout);

			// Line
			s_meshes[12] = s_resourceManager.Register("primitive:Line", Mesh::Line::Create());

			// Torus
			s_meshes[13] = s_resourceManager.Register("primitive:Torus", Mesh::Torus<16, 32>::Create(0.03f, 0.5f));

			for (uint32 i = 0; i < s_meshes.size(); i++)
				s_meshes[i]->Flush();
		}

		void RenderSystem::LoadDefaultTextures()
		{
			ASSERT(s_textures.size() != 0, "s_textures is not empty!");

			s_textures.resize(static_cast<size_t>(DefaultTextureIndex::COUNT));

			TexelBuffer texelBuffer;

			vk::SamplerCreateInfo samplerCreateInfo{
				vk::SamplerCreateFlags(),
				vk::Filter::eNearest,
				vk::Filter::eNearest,
				vk::SamplerMipmapMode::eNearest,
				vk::SamplerAddressMode::eRepeat,
				vk::SamplerAddressMode::eRepeat,
				vk::SamplerAddressMode::eRepeat,
				0.0f,
				true,
				8.0f,
				false,
				vk::CompareOp::eAlways,
				-1000,
				1000,
				vk::BorderColor::eFloatTransparentBlack,
				false
			};

			{
				texelBuffer.Clear();
				texelBuffer.width = 2;
				texelBuffer.height = 2;
				texelBuffer.channels = 4;
				texelBuffer.texels = {
					0, 0, 0, 255,		255, 0, 255, 255,
					255, 0, 255, 255,		0, 0, 0, 255
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::NoTexture)] = s_resourceManager.Register("defaultTexture:NoTexture", std::move(textureUnique));
			}

			texelBuffer.Clear();
			texelBuffer.width = 1;
			texelBuffer.height = 1;
			texelBuffer.channels = 4;

			// Black
			{
				texelBuffer.texels = {
					0, 0, 0, 255
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::Black)] = s_resourceManager.Register("defaultTexture:Black", std::move(textureUnique));
			}

			// White
			{
				texelBuffer.texels = {
					255, 255, 255, 255
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::White)] = s_resourceManager.Register("defaultTexture:White", std::move(textureUnique));
			}

			// Red
			{
				texelBuffer.texels = {
					255, 0, 0, 255
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::Red)] = s_resourceManager.Register("defaultTexture:Red", std::move(textureUnique));
			}

			// Green
			{
				texelBuffer.texels = {
					0, 255, 0, 255
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::Green)] = s_resourceManager.Register("defaultTexture:Green", std::move(textureUnique));
			}

			// Blue
			{
				texelBuffer.texels = {
					0, 0, 255, 255
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::Blue)] = s_resourceManager.Register("defaultTexture:Blue", std::move(textureUnique));
			}

			// BlackTransparent
			{
				texelBuffer.texels = {
					0, 0, 0, 0
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::BlackTransparent)] = s_resourceManager.Register("defaultTexture:BlackTransparent", std::move(textureUnique));
			}

			// WhiteTransparent
			{
				texelBuffer.texels = {
					255, 255, 255, 0
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::WhiteTransparent)] = s_resourceManager.Register("defaultTexture:WhiteTransparent", std::move(textureUnique));
			}

			// YellowHalfTransparent
			{
				texelBuffer.texels = {
					255, 255, 0, 127
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::YellowHalfTransparent)] = s_resourceManager.Register("defaultTexture:YellowHalfTransparent", std::move(textureUnique));
			}

			// CyanHalfTransparent
			{
				texelBuffer.texels = {
					0, 255, 255, 127
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::CyanHalfTransparent)] = s_resourceManager.Register("defaultTexture:CyanHalfTransparent", std::move(textureUnique));
			}

			// MagentaHalfTransparent
			{
				texelBuffer.texels = {
					255, 0, 255, 127
				};

				SampledTexture textureUnique(samplerCreateInfo);
				textureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				textureUnique.Flush();
				s_textures[static_cast<size_t>(DefaultTextureIndex::MagentaHalfTransparent)] = s_resourceManager.Register("defaultTexture:MagentaHalfTransparent", std::move(textureUnique));
			}
		}

		ShaderHandle RenderSystem::GetOrCreateInstanceMaterialShader(InstanceMaterialIndices indices)
		{
			auto it = s_instanceMaterials.find(indices);

			ShaderHandle shader;

			if (it == s_instanceMaterials.end())
			{
				Material& material = s_materials[indices.material];
				Pass& pass = s_passes[indices.pass];

				ASSERT(s_meshes[indices.mesh] == nullptr, "mesh is nullptr");
				ASSERT(s_meshes[indices.mesh]->vertexLayout == nullptr, "mesh's vertex layout is nullptr");

				const VertexLayout& vLayout = *s_meshes[indices.mesh]->vertexLayout;

				shader = s_resourceManager.materialManager.CreateShader(material, pass, vLayout);
				s_instanceMaterials.insert({ indices, shader });
			}
			else
			{
				shader = it->second;
			}

			return shader;
		}

		void RenderSystem::UpdateCopyCommandBuffer()
		{
			vk::CommandBufferInheritanceInfo inheritInfo;

			vk::CommandBufferBeginInfo beginInfo(
				vk::CommandBufferUsageFlagBits::eSimultaneousUse,
				&inheritInfo
			);

			static std::mutex mtx;

			mtx.lock();

			Context::GetDevice().WaitIdle();
			s_copyCommandBuffer->begin(beginInfo);
			s_viewUBO.Copy(s_copyCommandBuffer.get());
			s_lightsUBO.Copy(s_copyCommandBuffer.get());
			s_lightScatteringUBO.Copy(s_copyCommandBuffer.get());
			CopyModelUBD(s_copyCommandBuffer.get());
			s_copyCommandBuffer->end();

			mtx.unlock();
		}

		bool RenderSystem::Register(Component* component)
		{
			if (component == nullptr)
				return false;

			//if (InheritsOf(component->typeId, TypeInfo<MeshRenderer>::id))
			//	return Register(reinterpret_cast<MeshRenderer*>(component));

			//if (InheritsOf(component->typeId, TypeInfo<Skybox>::id))
			//	return Register(reinterpret_cast<Skybox*>(component));

			//if (InheritsOf(component->typeId, TypeInfo<Camera>::id))
			//	return Register(reinterpret_cast<Camera*>(component));

			//if (InheritsOf(component->typeId, TypeInfo<Light>::id))
			//	return Register(reinterpret_cast<Light*>(component));

			//if (InheritsOf(component->typeId, TypeInfo<Text2D>::id))
			//	return Register(reinterpret_cast<Text2D*>(component));

			//if (InheritsOf(component->typeId, TypeInfo<Billboard>::id))
			//	return Register(reinterpret_cast<Billboard*>(component));

			if (InheritsOf(component->typeId, TypeInfo<MeshRenderer>::id)
				|| InheritsOf(component->typeId, TypeInfo<Skybox>::id)
				|| InheritsOf(component->typeId, TypeInfo<Camera>::id)
				|| InheritsOf(component->typeId, TypeInfo<Light>::id)
				|| InheritsOf(component->typeId, TypeInfo<Text2D>::id)
				|| InheritsOf(component->typeId, TypeInfo<Billboard>::id))
			{
				component->m_isUsed = true;

				s_instMutex.lock();
				s_compsToInstantiate.push_back(component);
				s_instMutex.unlock();

				return true;
			}

			return false;
		}

		bool RenderSystem::Register(MeshRenderer* meshRenderer)
		{
			if (meshRenderer == nullptr)
				return false;

			ReserveModelUBD(meshRenderer);

			meshRenderer->m_isUsed = true;
			s_meshRenderers.push_back(meshRenderer);

			return true;
		}

		bool RenderSystem::Register(Skybox* skybox)
		{
			if (skybox == nullptr)
				return false;

			if (s_skybox != nullptr)
			{
				LOG_ERROR("There already is a skybox in the system");
				return false;
			}

			s_skybox = skybox;

			ReserveModelUBD(s_skybox);

			return true;
		}

		bool RenderSystem::Register(Camera* camera)
		{
			if (camera == nullptr)
				return false;

			camera->m_isUsed = true;
			s_cameras.push_back(camera);

			//if (s_mainCamera == nullptr)
			//	s_mainCamera = camera;

			return true;
		}

		bool RenderSystem::Register(Light* light)
		{
			if (light == nullptr)
				return false;

			if (light->typeId == TypeInfo<PointLight>::id)
				return Register(reinterpret_cast<PointLight*>(light));

			if (light->typeId == TypeInfo<DirectionalLight>::id)
				return Register(reinterpret_cast<DirectionalLight*>(light));

			return false;
		}

		bool RenderSystem::Register(PointLight* pointLight)
		{
			if (pointLight == nullptr)
				return false;

			if (s_pointLights.size() >= s_maxPointLights)
			{
				LOG_ERROR("Max PointLight components reached");
				return false;
			}

			pointLight->m_isUsed = true;
			s_pointLights.push_back(pointLight);

			return true;
		}

		bool RenderSystem::Register(DirectionalLight* directionalLight)
		{
			if (directionalLight == nullptr)
				return false;

			if (s_directionalLights.size() >= s_maxDirectionalLights)
			{
				LOG_ERROR("Max DirectionnalLight components reached");
				return false;
			}

			directionalLight->m_isUsed = true;
			s_directionalLights.push_back(directionalLight);

			return true;
		}

		bool RenderSystem::Register(Text2D* text2D)
		{
			if (text2D == nullptr)
				return false;

			s_text2Ds.push_back(text2D);
			text2D->m_meshIndex = RegisterMesh(&text2D->m_mesh);
			text2D->m_materialIndex = s_textMaterialIndex;
			text2D->m_fontAtlas = &s_fontAtlas;

			ReserveModelUBD(text2D);

			text2D->m_isUsed = true;

			return true;
		}

		bool RenderSystem::Register(Billboard* billboard)
		{
			if (billboard == nullptr)
				return false;

			ReserveModelUBD(billboard);

			billboard->m_isUsed = true;
			s_billboards.push_back(billboard);

			return true;
		}

		//void RenderSystem::Unregister(Component* component)
		//{
		//	if (component == nullptr)
		//		return;

		//	if (InheritsOf(component->typeId, TypeInfo<MeshRenderer>::id))
		//		Unregister(reinterpret_cast<MeshRenderer*>(component));

		//	if (InheritsOf(component->typeId, TypeInfo<Skybox>::id))
		//		Unregister(reinterpret_cast<Skybox*>(component));

		//	if (InheritsOf(component->typeId, TypeInfo<Camera>::id))
		//		Unregister(reinterpret_cast<Camera*>(component));

		//	if (InheritsOf(component->typeId, TypeInfo<Light>::id))
		//		Unregister(reinterpret_cast<Light*>(component));

		//	if (InheritsOf(component->typeId, TypeInfo<Text2D>::id))
		//		Unregister(reinterpret_cast<Text2D*>(component));

		//	if (InheritsOf(component->typeId, TypeInfo<Billboard>::id))
		//		Unregister(reinterpret_cast<Billboard*>(component));
		//}

		//void RenderSystem::Unregister(MeshRenderer* meshRenderer)
		//{
		//	if (meshRenderer == nullptr)
		//		return;

		//	//for (uint64 i{ 0 }; i < s_meshRenderers.size(); i++)
		//	//{
		//	//	if (s_meshRenderers[i] == meshRenderer)
		//	//	{
		//	//		meshRenderer->m_dynamicBlock.Free();
		//	//		s_meshRenderers.erase(s_meshRenderers.begin() + i);
		//	//		break;
		//	//	}
		//	//}

		//	s_gcMutex.lock();
		//	s_meshRenderersToDestroy.push_back(meshRenderer);
		//	s_gcMutex.unlock();
		//}

		//void RenderSystem::Unregister(Skybox* skybox)
		//{
		//	if (!skybox || !s_skybox || skybox != s_skybox)
		//		return;

		//	s_skybox->m_dynamicBlock.Free();
		//	s_skybox = nullptr;
		//}

		//void RenderSystem::Unregister(Camera* camera)
		//{
		//	if (camera == nullptr)
		//		return;

		//	//auto it = std::find(s_cameras.begin(), s_cameras.end(), camera);
		//	//if (it != s_cameras.end())
		//	//	s_cameras.erase(it);

		//	s_gcMutex.lock();
		//	s_camerasToDestroy.push_back(camera);
		//	s_gcMutex.unlock();

		//	//if (s_mainCamera == camera)
		//	//	s_mainCamera = s_cameras.empty() ? nullptr : s_cameras.front();
		//}

		//void RenderSystem::Unregister(Light* light)
		//{
		//	if (light == nullptr)
		//		return;

		//	if (light->typeId == TypeInfo<PointLight>::id)
		//		Unregister(reinterpret_cast<PointLight*>(light));

		//	if (light->typeId == TypeInfo<DirectionalLight>::id)
		//		Unregister(reinterpret_cast<DirectionalLight*>(light));
		//}

		//void RenderSystem::Unregister(PointLight* pointLight)
		//{
		//	if (pointLight == nullptr)
		//		return;

		//	//auto it = std::find(s_pointLights.begin(), s_pointLights.end(), pointLight);
		//	//if (it != s_pointLights.end())
		//	//	s_pointLights.erase(it);

		//	s_gcMutex.lock();
		//	s_pointLightsToDestroy.push_back(pointLight);
		//	s_gcMutex.unlock();
		//}

		//void RenderSystem::Unregister(DirectionalLight* directionalLight)
		//{
		//	if (directionalLight == nullptr)
		//		return;

		//	//auto it = std::find(s_directionalLights.begin(), s_directionalLights.end(), directionalLight);
		//	//if (it != s_directionalLights.end())
		//	//	s_directionalLights.erase(it);

		//	s_gcMutex.lock();
		//	s_directionalLightsToDestroy.push_back(directionalLight);
		//	s_gcMutex.unlock();
		//}

		//void RenderSystem::Unregister(Text2D* text2D)
		//{
		//	if (text2D == nullptr)
		//		return;

		//	//auto it = std::find(s_text2Ds.begin(), s_text2Ds.end(), text2D);
		//	//if (it != s_text2Ds.end())
		//	//	s_text2Ds.erase(it);

		//	s_gcMutex.lock();
		//	s_text2DsToDestroy.push_back(text2D);
		//	s_gcMutex.unlock();
		//}

		//void RenderSystem::Unregister(Billboard* billboard)
		//{
		//	if (billboard == nullptr)
		//		return;

		//	//auto it = std::find(s_billboards.begin(), s_billboards.end(), billboard);
		//	//if (it != s_billboards.end())
		//	//{
		//	//	billboard->m_dynamicBlock.Free();
		//	//	s_billboards.erase(it);
		//	//}

		//	s_gcMutex.lock();
		//	s_billboardsToDestroy.push_back(billboard);
		//	s_gcMutex.unlock();
		//}

		void RenderSystem::Update()
		{
			InstantiatePendingComponents();
			GarbageCollect();

			#ifdef WITH_EDITOR
			s_xLandmark.isActive = false;
			s_yLandmark.isActive = false;
			s_zLandmark.isActive = false;
			s_cLandmark.isActive = false;
			s_xyLandmark.isActive = false;
			s_yzLandmark.isActive = false;
			s_zxLandmark.isActive = false;
			#endif

			CameraViewInfo camInfo;

			s_opaqueRQ.Clear();
			s_transparentRQ.Clear();
			s_lightScatteringEmitterRQ.Clear();
			s_lightScatteringObstacleRQ.Clear();

			if (s_cameras.empty() == false)
			{
				auto camera = s_cameras.front();
				if (camera != nullptr)
				{
					camInfo.viewTr = camera->GlobalTransform();

					ViewUBO viewUBO;
					viewUBO.viewPos = camInfo.viewTr.GetPosition();
					viewUBO.viewMatrix = camInfo.viewTr.Inverse().ToMatrix();

					s_viewUBO.Write(&viewUBO);

					camInfo.projMat = camera->m_projection;
					camInfo.viewMat = viewUBO.viewMatrix;
					camInfo.viewProjMat = camera->m_projection * viewUBO.viewMatrix;
					camInfo.textViewProjMat = camera->m_textOrtho;

					Matrix4 billboardView = {
						viewUBO.viewMatrix.m00, viewUBO.viewMatrix.m01, viewUBO.viewMatrix.m02, viewUBO.viewPos.x,
						viewUBO.viewMatrix.m10, viewUBO.viewMatrix.m11, viewUBO.viewMatrix.m12, viewUBO.viewPos.y,
						viewUBO.viewMatrix.m20, viewUBO.viewMatrix.m21, viewUBO.viewMatrix.m22, viewUBO.viewPos.z,
						0, 0, 0, 1
					};

					camInfo.billboardViewRotationMat = {
						camInfo.viewMat.m00, camInfo.viewMat.m10, camInfo.viewMat.m20, 0,
						camInfo.viewMat.m01, camInfo.viewMat.m11, camInfo.viewMat.m21, 0,
						camInfo.viewMat.m02, camInfo.viewMat.m12, camInfo.viewMat.m22, 0,
						0, 0, 0, 1
					};

					camInfo.billboardViewProjMat = camera->m_projection * billboardView;

					if (s_skybox != nullptr)
					{
						ModelUBO ubo;
						ubo.mvp = camera->m_skyboxProjection * Matrix4(Matrix3(viewUBO.viewMatrix));

						s_skybox->m_dynamicBlock.Write(&ubo);
					}
				}
			}

			for (uint64 i{ 0 }; i < s_meshRenderers.size(); i++)
			{
				if (!s_meshRenderers[i] || !s_meshRenderers[i]->IsActive())
					continue;

				RecordMeshRenderer(*s_meshRenderers[i], camInfo);
			}

			for (uint64 i{ 0 }; i < s_debug3DLines.size(); i++)
			{
				if (!s_debug3DLines[i])
					continue;

				RecordDebug3DLine(*s_debug3DLines[i], camInfo);
			}

			for (uint64 i{ 0 }; i < s_text2Ds.size(); i++)
			{
				if (!s_text2Ds[i] || !s_text2Ds[i]->IsActive())
					continue;

				if(s_text2Ds[i]->UpdateContent())
					RecordText2D(*s_text2Ds[i], camInfo);
			}

			for (uint64 i{ 0 }; i < s_billboards.size(); i++)
			{
				if (!s_billboards[i] || !s_billboards[i]->IsActive())
					continue;

				RecordBillboard(*s_billboards[i], camInfo);
			}

			{
				auto pointLightCount = uint32(s_pointLights.size());

				s_lightsUBO.Write(&pointLightCount, sizeof(pointLightCount), sizeof(PointLightUBO) * s_maxPointLights);

				for (uint32 i{ 0 }; i < pointLightCount; i++)
				{
					PointLightUBO pointLight;
					pointLight.position = s_pointLights[i]->GlobalPosition();
					pointLight.color = s_pointLights[i]->color;
					pointLight.intensity = s_pointLights[i]->intensity;

					s_lightsUBO.Write(&pointLight, sizeof(PointLightUBO), sizeof(PointLightUBO) * i);
				}

				auto directionalLightCount = uint32(s_directionalLights.size());

				s_lightsUBO.Write(&directionalLightCount, sizeof(directionalLightCount), sizeof(PointLightUBO) * s_maxPointLights + sizeof(pointLightCount) + sizeof(DirectionalLightUBO) * s_maxDirectionalLights);

				for (uint32 i{ 0 }; i < directionalLightCount; i++)
				{
					DirectionalLightUBO directionalLight;
					directionalLight.direction = s_directionalLights[i]->GlobalRotation() * Vector3::Forward();
					directionalLight.color = s_directionalLights[i]->color;
					directionalLight.intensity = s_directionalLights[i]->intensity;

					s_lightsUBO.Write(&directionalLight, sizeof(DirectionalLightUBO), sizeof(PointLightUBO) * s_maxPointLights + sizeof(pointLightCount) + sizeof(PointLightUBO) * i);
				}
			}
		}

		#ifdef WITH_EDITOR
		void UpdateLandmark(RenderSystem::Gizmo3D& landmark, uint32 actorId, PrimitivesIndex meshIndex, uint32 materialIndex, const Transform& meshTransform, RenderSystem::CameraViewInfo& camInfo, Component& selectedComponent, bool landmarkControlAbsolute, float scale)
		{
			landmark.componentId = selectedComponent.id;
			landmark.actorId = actorId;
			landmark.meshRenderer->isLightScatteringObstacle = false;
			landmark.meshRenderer->SetMesh(static_cast<uint32>(meshIndex));
			landmark.meshRenderer->SetMaterial(materialIndex);
			landmark.meshRenderer->isImplicit = true;

			landmark.meshRenderer->SetScale(scale);

			landmark.meshRenderer->SetGlobalPosition(selectedComponent.GlobalPosition());

			if (InputManager::GetKeyDown(KeyCode::Shift) && selectedComponent.typeId == TypeInfo<MeshRenderer>::id)
			{
				auto& meshRenderer = reinterpret_cast<MeshRenderer&>(selectedComponent);

				landmark.meshRenderer->SetGlobalRotation(selectedComponent.GlobalRotation() * meshRenderer.meshTransform.GetRotation());
				// landmark.meshRenderer->SetGlobalRotation(Quaternion::Identity());
			}
			else
			{
				if (landmarkControlAbsolute == false)
					landmark.meshRenderer->SetGlobalRotation(selectedComponent.GlobalRotation());
				else
					landmark.meshRenderer->SetGlobalRotation(Quaternion::Identity());
			}

			landmark.meshRenderer->meshTransform = meshTransform;

			ModelUBO ubo;
			ubo.model = landmark.meshRenderer->GlobalTransform().ToMatrix() * landmark.meshRenderer->meshTransform.ToMatrix();
			ubo.mvp = camInfo.viewProjMat * ubo.model;
			ubo.componentId = landmark.componentId;
			ubo.actorId = landmark.actorId;

			landmark.isActive = true;

			landmark.meshRenderer->GetDynamicBlock().Write(&ubo);
		}

		void RenderSystem::UpdateInEditor(Component* selectedComponent, bool landmarkControlAbsolute, int currentTool) // 0 = translation, 1 = rotation, 2 = scale
		{
			InstantiatePendingComponents();
			GarbageCollect();

			CameraViewInfo camInfo;

			s_gizmos2D.clear();
			s_gizmos3D.clear();

			s_opaqueRQ.Clear();
			s_transparentRQ.Clear();
			s_lightScatteringEmitterRQ.Clear();
			s_lightScatteringObstacleRQ.Clear();

			MeshRenderer* selectedToDrawOverAll{ nullptr };

			if (editorCamera != nullptr)
			{
				camInfo.viewTr = editorCamera->GlobalTransform();

				ViewUBO viewUBO;
				viewUBO.viewPos = camInfo.viewTr.GetPosition();
				viewUBO.viewMatrix = camInfo.viewTr.Inverse().ToMatrix();

				s_viewUBO.Write(&viewUBO);

				camInfo.projMat = editorCamera->m_projection;
				camInfo.viewMat = viewUBO.viewMatrix;
				camInfo.viewProjMat = editorCamera->m_projection * viewUBO.viewMatrix;
				camInfo.textViewProjMat = editorCamera->m_textOrtho;

				Matrix4 billboardView = {
					camInfo.viewMat.m00, camInfo.viewMat.m01, camInfo.viewMat.m02, viewUBO.viewPos.x,
					camInfo.viewMat.m10, camInfo.viewMat.m11, camInfo.viewMat.m12, viewUBO.viewPos.y,
					camInfo.viewMat.m20, camInfo.viewMat.m21, camInfo.viewMat.m22, viewUBO.viewPos.z,
					0, 0, 0, 1
				};

				camInfo.billboardViewRotationMat = {
					camInfo.viewMat.m00, camInfo.viewMat.m10, camInfo.viewMat.m20, 0,
					camInfo.viewMat.m01, camInfo.viewMat.m11, camInfo.viewMat.m21, 0,
					camInfo.viewMat.m02, camInfo.viewMat.m12, camInfo.viewMat.m22, 0,
					0, 0, 0, 1
				};

				camInfo.billboardViewProjMat = editorCamera->m_projection * billboardView;

				if (s_skybox != nullptr)
				{
					ModelUBO ubo;
					ubo.mvp = editorCamera->m_skyboxProjection * Matrix4(Matrix3(viewUBO.viewMatrix));

					s_skybox->m_dynamicBlock.Write(&ubo);
				}
			}

			for (uint64 i{ 0 }; i < s_meshRenderers.size(); i++)
			{
				if (!s_meshRenderers[i] || !s_meshRenderers[i]->IsActive())
					continue;

				/*if (s_meshRenderers[i] == selectedComponent)
				{
					selectedToDrawOverAll = reinterpret_cast<MeshRenderer*>(selectedComponent);

					uint32 tempMatId = selectedToDrawOverAll->GetMaterial();
					Vector3 tempScale = selectedToDrawOverAll->meshTransform.scale;

					selectedToDrawOverAll->SetMaterial(s_selectedOutlineMaterialIndex);
					selectedToDrawOverAll->meshTransform.scale = tempScale * 1.05f;
					RecordMeshRenderer(*selectedToDrawOverAll, camInfo);

					selectedToDrawOverAll->SetMaterial(tempMatId);
					selectedToDrawOverAll->meshTransform.scale = tempScale;
					RecordMeshRenderer(*selectedToDrawOverAll, camInfo);
				}
				else
				{*/
					RecordMeshRenderer(*s_meshRenderers[i], camInfo);
				//}
			}

			for (uint64 i{ 0 }; i < s_debug3DLines.size(); i++)
			{
				if (!s_debug3DLines[i])
					continue;

				RecordDebug3DLine(*s_debug3DLines[i], camInfo);
			}

			for (uint64 i{ 0 }; i < s_text2Ds.size(); i++)
			{
				if (!s_text2Ds[i] || !s_text2Ds[i]->IsActive())
					continue;

				if(s_text2Ds[i]->UpdateContent())
					RecordText2D(*s_text2Ds[i], camInfo);
			}

			for (uint64 i{ 0 }; i < s_billboards.size(); i++)
			{
				if (!s_billboards[i] || !s_billboards[i]->IsActive())
					continue;

				RecordBillboard(*s_billboards[i], camInfo);
			}

			{
				auto pointLightCount = uint32(s_pointLights.size());

				s_lightsUBO.Write(&pointLightCount, sizeof(pointLightCount), sizeof(PointLightUBO) * s_maxPointLights);

				for (uint32 i{ 0 }; i < pointLightCount; i++)
				{
					PointLightUBO pointLight;
					pointLight.position = s_pointLights[i]->GlobalPosition();
					pointLight.color = s_pointLights[i]->color;
					pointLight.intensity = s_pointLights[i]->intensity;

					s_lightsUBO.Write(&pointLight, sizeof(PointLightUBO), sizeof(PointLightUBO) * i);

					Gizmo2D gizmo;
					gizmo.billboard = std::make_unique<Billboard>(s_pointLights[i]->GetActor());
					gizmo.billboard->SetGlobalRotation(aw::Quaternion(3.14159f, aw::Vector3{ 1.0f, 0.0f, 0.0f }));
					gizmo.billboard->SetGlobalPosition(pointLight.position);
					gizmo.billboard->SetScale(0.5f);
					gizmo.billboard->SetMesh(s_quadMeshIndex);
					gizmo.billboard->SetMaterial(s_lightGizmoMaterialIndex);
					gizmo.billboard->isImplicit = true;
					gizmo.componentId = s_pointLights[i]->id;
					gizmo.actorId = s_pointLights[i]->GetActor()->id;
					RegisterImmediate(std::move(gizmo));
				}

				auto directionalLightCount = uint32(s_directionalLights.size());

				s_lightsUBO.Write(&directionalLightCount, sizeof(directionalLightCount), sizeof(PointLightUBO) * s_maxPointLights + sizeof(pointLightCount) + sizeof(DirectionalLightUBO) * s_maxDirectionalLights);

				for (uint32 i{ 0 }; i < directionalLightCount; i++)
				{
					DirectionalLightUBO directionalLight;
					directionalLight.direction = s_directionalLights[i]->GlobalRotation() * Vector3::Forward();
					directionalLight.color = s_directionalLights[i]->color;
					directionalLight.intensity = s_directionalLights[i]->intensity;

					s_lightsUBO.Write(&directionalLight, sizeof(DirectionalLightUBO), sizeof(PointLightUBO) * s_maxPointLights + sizeof(pointLightCount) + sizeof(PointLightUBO) * i);

					{
						Gizmo2D gizmo;
						gizmo.billboard = std::make_unique<Billboard>(s_directionalLights[i]->GetActor());
						gizmo.billboard->SetGlobalRotation(aw::Quaternion(3.14159f, aw::Vector3{ 1.0f, 0.0f, 0.0f }));
						gizmo.billboard->SetGlobalPosition(s_directionalLights[i]->GlobalPosition());
						gizmo.billboard->SetScale(0.5f);
						gizmo.billboard->SetMesh(s_quadMeshIndex);
						gizmo.billboard->SetMaterial(s_sunGizmoMaterialIndex);
						gizmo.billboard->isImplicit = true;
						gizmo.componentId = s_directionalLights[i]->id;
						gizmo.actorId = s_directionalLights[i]->GetActor()->id;
						RegisterImmediate(std::move(gizmo));
					}

					{
						Gizmo3D gizmo;
						gizmo.meshRenderer = std::make_unique<MeshRenderer>(s_directionalLights[i]->GetActor());
						gizmo.meshRenderer->SetGlobalRotation(s_directionalLights[i]->GlobalRotation());
						gizmo.meshRenderer->SetGlobalPosition(s_directionalLights[i]->GlobalPosition());
						gizmo.meshRenderer->SetScale(-1.0f);
						gizmo.meshRenderer->SetMesh(static_cast<uint32>(PrimitivesIndex::Line));
						gizmo.meshRenderer->SetMaterial(s_debugMaterialIndex);
						gizmo.meshRenderer->isImplicit = true;
						RegisterImmediate(std::move(gizmo));

						gizmo.meshRenderer = std::make_unique<MeshRenderer>(s_directionalLights[i]->GetActor());
						gizmo.meshRenderer->SetGlobalRotation(s_directionalLights[i]->GlobalRotation());
						gizmo.meshRenderer->SetGlobalPosition(s_directionalLights[i]->GlobalPosition());
						gizmo.meshRenderer->Translate_SelfSpace(Vector3::Right() * 0.1f);
						gizmo.meshRenderer->SetScale(-0.8f);
						gizmo.meshRenderer->SetMesh(static_cast<uint32>(PrimitivesIndex::Line));
						gizmo.meshRenderer->SetMaterial(s_debugMaterialIndex);
						gizmo.meshRenderer->isImplicit = true;
						RegisterImmediate(std::move(gizmo));

						gizmo.meshRenderer = std::make_unique<MeshRenderer>(s_directionalLights[i]->GetActor());
						gizmo.meshRenderer->SetGlobalRotation(s_directionalLights[i]->GlobalRotation());
						gizmo.meshRenderer->SetGlobalPosition(s_directionalLights[i]->GlobalPosition());
						gizmo.meshRenderer->Translate_SelfSpace(-Vector3::Right() * 0.1f);
						gizmo.meshRenderer->SetScale(-0.8f);
						gizmo.meshRenderer->SetMesh(static_cast<uint32>(PrimitivesIndex::Line));
						gizmo.meshRenderer->SetMaterial(s_debugMaterialIndex);
						gizmo.meshRenderer->isImplicit = true;
						RegisterImmediate(std::move(gizmo));

						gizmo.meshRenderer = std::make_unique<MeshRenderer>(s_directionalLights[i]->GetActor());
						gizmo.meshRenderer->SetGlobalRotation(s_directionalLights[i]->GlobalRotation());
						gizmo.meshRenderer->SetGlobalPosition(s_directionalLights[i]->GlobalPosition());
						gizmo.meshRenderer->Translate_SelfSpace(Vector3::Up() * 0.1f);
						gizmo.meshRenderer->SetScale(-0.8f);
						gizmo.meshRenderer->SetMesh(static_cast<uint32>(PrimitivesIndex::Line));
						gizmo.meshRenderer->SetMaterial(s_debugMaterialIndex);
						gizmo.meshRenderer->isImplicit = true;
						RegisterImmediate(std::move(gizmo));

						gizmo.meshRenderer = std::make_unique<MeshRenderer>(s_directionalLights[i]->GetActor());
						gizmo.meshRenderer->SetGlobalRotation(s_directionalLights[i]->GlobalRotation());
						gizmo.meshRenderer->SetGlobalPosition(s_directionalLights[i]->GlobalPosition());
						gizmo.meshRenderer->Translate_SelfSpace(-Vector3::Up() * 0.1f);
						gizmo.meshRenderer->SetScale(-0.8f);
						gizmo.meshRenderer->SetMesh(static_cast<uint32>(PrimitivesIndex::Line));
						gizmo.meshRenderer->SetMaterial(s_debugMaterialIndex);
						gizmo.meshRenderer->isImplicit = true;
						RegisterImmediate(std::move(gizmo));
					}
				}
			}

			for (uint64 i{ 0 }; i < s_cameras.size(); i++)
			{
				Gizmo2D gizmo;
				gizmo.billboard = std::make_unique<Billboard>(s_cameras[i]->GetActor());
				gizmo.billboard->SetGlobalRotation(aw::Quaternion(3.14159f, aw::Vector3{ 1.0f, 0.0f, 0.0f }));
				gizmo.billboard->SetGlobalPosition(s_cameras[i]->GlobalPosition());
				gizmo.billboard->SetScale(0.5f);
				gizmo.billboard->SetMesh(s_quadMeshIndex);
				gizmo.billboard->SetMaterial(s_cameraGizmoMaterialIndex);
				gizmo.billboard->isImplicit = true;
				gizmo.componentId = s_cameras[i]->id;
				gizmo.actorId = s_cameras[i]->GetActor()->id;
				RegisterImmediate(std::move(gizmo));
			}

			if (s_xLandmark.meshRenderer == nullptr)
			{
				s_xLandmark.meshRenderer = std::make_unique<MeshRenderer>(nullptr);

				ReserveModelUBD(s_xLandmark.meshRenderer.get());
			}
			if (s_yLandmark.meshRenderer == nullptr)
			{
				s_yLandmark.meshRenderer = std::make_unique<MeshRenderer>(nullptr);

				ReserveModelUBD(s_yLandmark.meshRenderer.get());
			}
			if (s_zLandmark.meshRenderer == nullptr)
			{
				s_zLandmark.meshRenderer = std::make_unique<MeshRenderer>(nullptr);

				ReserveModelUBD(s_zLandmark.meshRenderer.get());
			}
			if (s_cLandmark.meshRenderer == nullptr)
			{
				s_cLandmark.meshRenderer = std::make_unique<MeshRenderer>(nullptr);

				ReserveModelUBD(s_cLandmark.meshRenderer.get());
			}
			if (s_xyLandmark.meshRenderer == nullptr)
			{
				s_xyLandmark.meshRenderer = std::make_unique<MeshRenderer>(nullptr);

				ReserveModelUBD(s_xyLandmark.meshRenderer.get());
			}
			if (s_yzLandmark.meshRenderer == nullptr)
			{
				s_yzLandmark.meshRenderer = std::make_unique<MeshRenderer>(nullptr);

				ReserveModelUBD(s_yzLandmark.meshRenderer.get());
			}
			if (s_zxLandmark.meshRenderer == nullptr)
			{
				s_zxLandmark.meshRenderer = std::make_unique<MeshRenderer>(nullptr);

				ReserveModelUBD(s_zxLandmark.meshRenderer.get());
			}

			s_xLandmark.isActive = false;
			s_yLandmark.isActive = false;
			s_zLandmark.isActive = false;
			s_cLandmark.isActive = false;
			s_xyLandmark.isActive = false;
			s_yzLandmark.isActive = false;
			s_zxLandmark.isActive = false;

			if (selectedComponent != nullptr && editorCamera != nullptr)
			{
				float halfFOV = editorCamera->GetFOV();
				float D = (selectedComponent->GlobalPosition() - editorCamera->GlobalPosition()).Norm();
				float halfScale = tanf(halfFOV) * D;

				float scale = (halfScale * 2.0f) / 10.0f;

				{
					//auto debug = std::make_unique<MeshRenderer>(nullptr);
					Gizmo3D debug;
					debug.componentId = selectedComponent->id;
					debug.actorId = selectedComponent->GetActor()->id;
					debug.meshRenderer = std::make_unique<MeshRenderer>(nullptr);
					debug.meshRenderer->SetMaterial(s_debugMaterialIndex);
					debug.meshRenderer->isImplicit = true;
					debug.meshRenderer->SetGlobalTransform(selectedComponent->GlobalTransform());

					uint32 typeId = selectedComponent->typeId;
					Rigidbody* body = dynamic_cast<Rigidbody*>(selectedComponent);

					if (typeId == TypeInfo<BoxCollider>::id || typeId == TypeInfo<Rigidbody>::id && body->geometryType == ph::GeometryType::Box)
					{
						Vector3 scale = body ? body->halfScale : dynamic_cast<BoxCollider*>(selectedComponent)->halfScale;
						scale *= 2.0f; // Because we get HalfScale.

						// meshTransform already apply  * selectedComponent->GlobalTransform().scale;
						debug.meshRenderer->meshTransform.scale = scale;
						debug.meshRenderer->SetMesh(static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::DebugCube));
						RegisterImmediate(std::move(debug));
					}
					else if (typeId == TypeInfo<CapsuleCollider>::id || typeId == TypeInfo<Rigidbody>::id && body->geometryType == ph::GeometryType::Capsule)
					{
						float radius = body ? body->radius : dynamic_cast<CapsuleCollider*>(selectedComponent)->radius;
						radius *= 2.0f; // Because we get HalfScale.

						float height = body ? body->halfHeight : dynamic_cast<CapsuleCollider*>(selectedComponent)->halfHeight;
						height *= 2.0f; // Because we get HalfScale.

						// meshTransform already apply  * selectedComponent->GlobalTransform().scale.
						debug.meshRenderer->meshTransform.scale = {radius, height, radius};
						debug.meshRenderer->SetMesh(static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::DebugCapsule));
						RegisterImmediate(std::move(debug));
					}
					else if (typeId == TypeInfo<SphereCollider>::id || typeId == TypeInfo<Rigidbody>::id && body->geometryType == ph::GeometryType::Sphere)
					{
						float scale = body ? body->radius : dynamic_cast<SphereCollider*>(selectedComponent)->radius;
						scale *= 2.0f; // Because we get HalfScale;

						// meshTransform already apply  * selectedComponent->GlobalTransform().scale.
						debug.meshRenderer->meshTransform.scale = Vector3(scale);
						debug.meshRenderer->SetMesh(static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::DebugSphere));
						RegisterImmediate(std::move(debug));
					}
					else if (typeId == TypeInfo<PlaneCollider>::id)
					{
						debug.meshRenderer->SetMesh(static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::DebugQuad));
						RegisterImmediate(std::move(debug));
					}
				}

				if (currentTool == 0) // Translation
				{
					static Transform xTransform(Vector3(0.5f, 0.0f, 0.0f), Quaternion(-M_PI2, Vector3(0.0f, 0.0f, 1.0f)), Vector3(0.1f, 1.0f, 0.1f));
					static Transform yTransform(Vector3(0.0f, 0.5f, 0.0f), Quaternion::Identity(), Vector3(0.1f, 1.0f, 0.1f));
					static Transform zTransform(Vector3(0.0f, 0.0f, 0.5f), Quaternion(M_PI2, Vector3(1.0f, 0.0f, 0.0f)), Vector3(0.1f, 1.0f, 0.1f));
					static Transform cTransform(Vector3::Zero(), Quaternion::Identity(), Vector3(0.2f, 0.2f, 0.2f));
					static Transform xyTransform(Vector3(0.15f, 0.15f, 0.0f), Quaternion(M_PI2, Vector3(1.0f, 0.0f, 0.0f)), Vector3(0.03f, 0.03f, 0.03f));
					static Transform yzTransform(Vector3(0.0f, 0.15f, 0.15f), Quaternion(-M_PI2, Vector3(0.0f, 0.0f, 1.0f)), Vector3(0.03f, 0.03f, 0.03f));
					static Transform zxTransform(Vector3(0.15f, 0.0f, 0.15f), Quaternion::Identity(), Vector3(0.03f, 0.03f, 0.03f));

					UpdateLandmark(s_xLandmark, ~0 - 1, PrimitivesIndex::Cylinder, s_redLandmarkMaterialIndex, xTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
					UpdateLandmark(s_yLandmark, ~0 - 2, PrimitivesIndex::Cylinder, s_greenLandmarkMaterialIndex, yTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
					UpdateLandmark(s_zLandmark, ~0 - 3, PrimitivesIndex::Cylinder, s_blueLandmarkMaterialIndex, zTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
					UpdateLandmark(s_cLandmark, ~0 - 4, PrimitivesIndex::Sphere, s_whiteLandmarkMaterialIndex, cTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);

					if (InputManager::GetKeyUp(KeyCode::Shift))
					{
						UpdateLandmark(s_xyLandmark, ~0 - 5, PrimitivesIndex::Quad, s_yellowLandmarkMaterialIndex, xyTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
						UpdateLandmark(s_yzLandmark, ~0 - 6, PrimitivesIndex::Quad, s_cyanLandmarkMaterialIndex, yzTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
						UpdateLandmark(s_zxLandmark, ~0 - 7, PrimitivesIndex::Quad, s_magentaLandmarkMaterialIndex, zxTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
					}
				}
				else if (currentTool == 1) // Rotation
				{
					static Transform xTransform(Vector3::Zero(), Quaternion(M_PI2, Vector3(0.0f, 1.0f, 0.0f)));
					static Transform yTransform(Vector3::Zero(), Quaternion(M_PI2, Vector3(1.0f, 0.0f, 0.0f)));
					static Transform zTransform(Vector3::Zero(), Quaternion::Identity());
					static Transform cTransform(Vector3::Zero(), Quaternion::Identity(), Vector3(0.2f, 0.2f, 0.2f));

					UpdateLandmark(s_xLandmark, ~0 - 1, PrimitivesIndex::Torus, s_redLandmarkMaterialIndex, xTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
					UpdateLandmark(s_yLandmark, ~0 - 2, PrimitivesIndex::Torus, s_greenLandmarkMaterialIndex, yTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
					UpdateLandmark(s_zLandmark, ~0 - 3, PrimitivesIndex::Torus, s_blueLandmarkMaterialIndex, zTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
					UpdateLandmark(s_cLandmark, ~0 - 4, PrimitivesIndex::Sphere, s_whiteLandmarkMaterialIndex, cTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
				}
				else if (currentTool == 2) // Scale
				{
					if (InputManager::GetKeyDown(KeyCode::Shift))
					{
						static Transform xTransform(Vector3(0.5f, 0.0f, 0.0f), Quaternion(-M_PI2, Vector3(0.0f, 0.0f, 1.0f)), Vector3(0.1f, 1.0f, 0.1f));
						static Transform yTransform(Vector3(0.0f, 0.5f, 0.0f), Quaternion::Identity(), Vector3(0.1f, 1.0f, 0.1f));
						static Transform zTransform(Vector3(0.0f, 0.0f, 0.5f), Quaternion(M_PI2, Vector3(1.0f, 0.0f, 0.0f)), Vector3(0.1f, 1.0f, 0.1f));
						static Transform cTransform(Vector3::Zero(), Quaternion::Identity(), Vector3(0.2f, 0.2f, 0.2f));

						UpdateLandmark(s_xLandmark, ~0 - 1, PrimitivesIndex::Cube, s_redLandmarkMaterialIndex, xTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
						UpdateLandmark(s_yLandmark, ~0 - 2, PrimitivesIndex::Cube, s_greenLandmarkMaterialIndex, yTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
						UpdateLandmark(s_zLandmark, ~0 - 3, PrimitivesIndex::Cube, s_blueLandmarkMaterialIndex, zTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
						UpdateLandmark(s_cLandmark, ~0 - 4, PrimitivesIndex::Cube, s_whiteLandmarkMaterialIndex, cTransform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
					}
					else
					{
						static Transform transform(Vector3::Zero(), Quaternion::Identity(), Vector3(0.2f, 0.2f, 0.2f));

						UpdateLandmark(s_cLandmark, ~0 - 4, PrimitivesIndex::Cube, s_whiteLandmarkMaterialIndex, transform, camInfo, *selectedComponent, landmarkControlAbsolute, scale);
					}
				}
			}

			for (uint64 i{ 0 }; i < s_gizmos3D.size(); i++)
			{
				if (!s_gizmos3D[i].meshRenderer)
					continue;

				RecordMeshRenderer(*s_gizmos3D[i].meshRenderer, camInfo);

				ModelUBO ubo;
				ubo.model = s_gizmos3D[i].meshRenderer->GlobalTransform().ToMatrix() * s_gizmos3D[i].meshRenderer->meshTransform.ToMatrix();
				ubo.mvp = camInfo.viewProjMat * ubo.model;
				ubo.componentId = s_gizmos3D[i].componentId;
				ubo.actorId = s_gizmos3D[i].actorId;

				s_gizmos3D[i].meshRenderer->m_dynamicBlock.Write(&ubo);
			}

			for (uint64 i{ 0 }; i < s_gizmos2D.size(); i++)
			{
				if (!s_gizmos2D[i].billboard)
					continue;

				RecordBillboard(*s_gizmos2D[i].billboard, camInfo);

				ModelUBO ubo;
				UniformTransform tr = s_gizmos2D[i].billboard->GlobalTransform();

				tr.SetPosition(camInfo.viewTr.GetRotation() * tr.GetPosition() + camInfo.viewTr.GetPosition());

				ubo.model = tr.ToMatrix();
				ubo.mvp = camInfo.projMat * ubo.model;
				ubo.componentId = s_gizmos2D[i].componentId;
				ubo.actorId = s_gizmos2D[i].actorId;

				s_gizmos2D[i].billboard->m_dynamicBlock.Write(&ubo);
			}
		}

		std::pair<uint32, uint32> RenderSystem::GetIdsAtPixel(const MousePos& pos)
		{
			std::pair<uint32, uint32> ids{};

			s_componentIdSubBuffer.Read(&ids, sizeof(ids), pos.x * sizeof(ids) + pos.y * s_width * sizeof(ids));

			return ids;
		}

		void RenderSystem::DrawInEditor(vk::CommandBuffer commandBuffer, Camera& editorCamera)
		{
			vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
			commandBuffer.begin(beginInfo);

			commandBuffer.executeCommands(s_copyCommandBuffer.get());

			commandBuffer.setViewport(0, editorCamera.renderArea.viewport);
			commandBuffer.setScissor(0, editorCamera.renderArea.scissor);
			s_framegraph.Draw(commandBuffer);

			//m_componentIdSubBuffer = BufferManager::Reserve(m_width * m_height * sizeof(uint32), false, BufferType::View);
			vk::BufferImageCopy copy(
				s_componentIdSubBuffer.offset,
				0, 0,
				//m_width, m_height,
				vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
				vk::Offset3D(), vk::Extent3D(s_width, s_height, 1)
			);

			//commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlagBits::eByRegion, )

			commandBuffer.copyImageToBuffer(
				s_frameResourcesManager.GetImage(s_componentIdImageIndex).GetImage(),
				vk::ImageLayout::eTransferSrcOptimal,
				s_componentIdSubBuffer.buffer->Get(),
				copy
			);

			commandBuffer.end();
		}

		bool RenderSystem::RegisterImmediate(Gizmo2D gizmo2D)
		{
			ReserveModelUBD(gizmo2D.billboard.get());

			s_gizmos2D.push_back(std::move(gizmo2D));

			return true;
		}

		bool RenderSystem::RegisterImmediate(Gizmo3D gizmo3D)
		{
			ReserveModelUBD(gizmo3D.meshRenderer.get());

			s_gizmos3D.push_back(std::move(gizmo3D));

			return true;
		}
		#endif

		void RenderSystem::RecordMeshRenderer(MeshRenderer& meshRenderer, const CameraViewInfo& camInfo)
		{
			ModelUBO ubo;
			ubo.model = meshRenderer.GlobalTransform().ToMatrix() * meshRenderer.meshTransform.ToMatrix();
			ubo.mvp = camInfo.viewProjMat * ubo.model;
			ubo.componentId = meshRenderer.id;
			if (meshRenderer.GetActor())
				ubo.actorId = meshRenderer.GetActor()->id;

			Material& material = s_materials[meshRenderer.m_materialIndex];

			if (material.IsModified() == true)
			{
				if (material.materialInfo.hasDiffuseMap && material.GetDiffuse() != ~0)
					material.SetSampledTexture(s_textures[material.GetDiffuse()], material.GetDiffuseBinding());
				if (material.materialInfo.hasHeightMap && material.GetHeight() != ~0)
					material.SetSampledTexture(s_textures[material.GetHeight()], material.GetHeightBinding());
				if (material.materialInfo.hasNormalMap && material.GetNormal() != ~0)
					material.SetSampledTexture(s_textures[material.GetNormal()], material.GetNormalBinding());
				if (material.materialInfo.hasSpecularMap && material.GetSpecular() != ~0)
					material.SetSampledTexture(s_textures[material.GetSpecular()], material.GetSpecularBinding());
				if (material.materialInfo.hasEnvironmentMap && material.GetEnvironment() != ~0)
					material.SetSampledTexture(s_textures[material.GetEnvironment()], material.GetEnvironmentBinding());
				if (material.materialInfo.hasReflectionMap && material.GetReflection() != ~0)
					material.SetSampledTexture(s_textures[material.GetReflection()], material.GetReflectionBinding());
				if (material.materialInfo.hasRefractionMap && material.GetRefraction() != ~0)
					material.SetSampledTexture(s_textures[material.GetRefraction()], material.GetRefractionBinding());
				if (material.materialInfo.hasEmissiveMap && material.GetEmissive() != ~0)
					material.SetSampledTexture(s_textures[material.GetEmissive()], material.GetEmissiveBinding());
				if (material.materialInfo.hasOcclusionMap && material.GetOcclusion() != ~0)
					material.SetSampledTexture(s_textures[material.GetOcclusion()], material.GetOcclusionBinding());
				if (material.materialInfo.hasMetalicRoughnessMap && material.GetMetalicRoughness() != ~0)
					material.SetSampledTexture(s_textures[material.GetMetalicRoughness()], material.GetMetalicRoughnessBinding());

				material.MarkClean();
			}

			ubo.baseColorFactor = material.GetBaseColorFactor();
			ubo.emissiveFactor = material.GetEmissiveFactor();
			// LOG_LOG("ubo.emissiveFactor: %f, %f, %f", ubo.emissiveFactor.red, ubo.emissiveFactor.green, ubo.emissiveFactor.blue);
			ubo.metallicFactor = material.GetMetallicFactor();
			ubo.roughnessFactor = material.GetRoughnessFactor();
			ubo.scaleUV = material.GetScaleUV();
			ubo.scaleUV.x *= meshRenderer.scaleUV.x;
			ubo.scaleUV.y *= meshRenderer.scaleUV.y;
			ubo.offsetUV = material.GetOffsetUV() + meshRenderer.offsetUV;

			Mesh* mesh = s_meshes[meshRenderer.m_meshIndex];

			meshRenderer.m_dynamicBlock.Write(&ubo);


			if (material.materialInfo.isTransparent)
			{
				ShaderHandle shader = GetOrCreateInstanceMaterialShader({ meshRenderer.m_materialIndex, meshRenderer.m_meshIndex, s_forwardPassIndex });
				s_transparentRQ.AddModel(shader, material, mesh, meshRenderer.m_dynamicBlock.GetOffset(), meshRenderer.m_geometryDset);
			}
			else
			{
				ShaderHandle shader = GetOrCreateInstanceMaterialShader({ meshRenderer.m_materialIndex, meshRenderer.m_meshIndex, s_transparencyPassIndex });
				s_opaqueRQ.AddModel(shader, material, mesh, meshRenderer.m_dynamicBlock.GetOffset(), meshRenderer.m_geometryDset);
			}

			if (meshRenderer.isLightScatteringEmitter)
			{
				Material& lightScatteringMaterial = s_materials[s_firstLightScatteringWhiteLightMaterialIndex];
				ShaderHandle lightScatteringShader = GetOrCreateInstanceMaterialShader({ s_firstLightScatteringWhiteLightMaterialIndex, meshRenderer.m_meshIndex, s_firstLightScatteringPassIndex });
				s_lightScatteringEmitterRQ.AddModel(lightScatteringShader, lightScatteringMaterial, mesh, meshRenderer.m_dynamicBlock.GetOffset(), meshRenderer.m_geometryDset);

				Vector3 projected = ubo.mvp.Project({});
				const float rhw = 1.0f / projected.z;

				LightScatteringUBO.screenLightPosition.x = (projected.x * rhw) / 4.0f + 0.5f;
				LightScatteringUBO.screenLightPosition.y = (projected.y * rhw) / 4.0f + 0.5f;

				s_lightScatteringUBO.Write(&LightScatteringUBO);
			}
			else if (meshRenderer.isLightScatteringObstacle)
			{
				Material& lightScatteringMaterial = s_materials[s_firstLightScatteringObstacleMaterialIndex];
				ShaderHandle lightScatteringShader = GetOrCreateInstanceMaterialShader({ s_firstLightScatteringObstacleMaterialIndex, meshRenderer.m_meshIndex, s_firstLightScatteringPassIndex });
				s_lightScatteringObstacleRQ.AddModel(lightScatteringShader, lightScatteringMaterial, mesh, meshRenderer.m_dynamicBlock.GetOffset(), meshRenderer.m_geometryDset);
			}
		}

		void RenderSystem::RecordDebug3DLine(MeshRenderer& debug3dLine, const CameraViewInfo& camInfo)
		{
			ModelUBO ubo;
			ubo.model = debug3dLine.GlobalTransform().ToMatrix();
			ubo.mvp = camInfo.viewProjMat * ubo.model;
			//ubo.componentId = static_cast<float>(debug3dLine.id) / 100.0f;
			ubo.componentId = debug3dLine.id;
			if (debug3dLine.GetActor())
				ubo.actorId = debug3dLine.GetActor()->id;

			debug3dLine.m_dynamicBlock.Write(&ubo);
			//s_modelUBD.Write(i, &ubo);

			Material& material = s_materials[debug3dLine.m_materialIndex];
			Mesh* mesh = s_meshes[debug3dLine.m_meshIndex];


			if (material.materialInfo.isTransparent)
			{
				ShaderHandle shader = GetOrCreateInstanceMaterialShader({ debug3dLine.m_materialIndex, debug3dLine.m_meshIndex, s_forwardPassIndex });
				s_transparentRQ.AddModel(shader, material, mesh, debug3dLine.m_dynamicBlock.GetOffset(), debug3dLine.m_geometryDset);
			}
			else
			{
				ShaderHandle shader = GetOrCreateInstanceMaterialShader({ debug3dLine.m_materialIndex, debug3dLine.m_meshIndex, s_transparencyPassIndex });
				s_opaqueRQ.AddModel(shader, material, mesh, debug3dLine.m_dynamicBlock.GetOffset(), debug3dLine.m_geometryDset);
			}
		}

		void RenderSystem::RecordText2D(Text2D& text2D, const CameraViewInfo& camInfo)
		{
			ModelUBO ubo;
			ubo.model = text2D.GlobalTransform().ToMatrix();
			ubo.mvp = camInfo.textViewProjMat * ubo.model;
			//ubo.componentId = static_cast<float>(text2D.id) / 100.0f;
			ubo.componentId = text2D.id;
			if (text2D.GetActor())
				ubo.actorId = text2D.GetActor()->id;

			text2D.m_dynamicBlock.Write(&ubo);

			Material& material = s_materials[text2D.m_materialIndex];
			Mesh* mesh = s_meshes[text2D.m_meshIndex];

			ShaderHandle shader = GetOrCreateInstanceMaterialShader({ text2D.m_materialIndex, text2D.m_meshIndex, s_transparencyPassIndex });

			s_transparentRQ.AddModel(shader, material, mesh, text2D.m_dynamicBlock.GetOffset(), text2D.m_geometryDset);
		}

		void RenderSystem::RecordBillboard(Billboard& billboard, const CameraViewInfo& camInfo)
		{
			ModelUBO ubo;
			UniformTransform tr = billboard.GlobalTransform();

			tr.SetPosition(camInfo.viewTr.GetRotation() * tr.GetPosition() + camInfo.viewTr.GetPosition());

			ubo.model = tr.ToMatrix();
			ubo.mvp = camInfo.projMat * ubo.model;
			//ubo.componentId = static_cast<float>(billboard.id) / 100.0f;
			ubo.componentId = billboard.id;
			if (billboard.isImplicit == true && billboard.GetParent() != nullptr && billboard.GetParent()->GetActor() != nullptr)
				ubo.actorId = billboard.GetParent()->GetActor()->id;
			else if (billboard.GetActor())
				ubo.actorId = billboard.GetActor()->id;

			billboard.m_dynamicBlock.Write(&ubo);

			Material& material = s_materials[billboard.m_materialIndex];
			Mesh* mesh = s_meshes[billboard.m_meshIndex];

			ShaderHandle shader = GetOrCreateInstanceMaterialShader({ billboard.m_materialIndex, billboard.m_meshIndex, s_transparencyPassIndex });

			s_transparentRQ.AddModel(shader, material, mesh, billboard.m_dynamicBlock.GetOffset(), billboard.m_geometryDset);
		}

		void RenderSystem::Draw(vk::CommandBuffer commandBuffer)
		{
			vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
			commandBuffer.begin(beginInfo);

			commandBuffer.executeCommands(s_copyCommandBuffer.get());

			if (s_cameras.empty() == false)
			{
				auto camera = s_cameras.front();

				commandBuffer.setViewport(0, camera->renderArea.viewport);
				commandBuffer.setScissor(0, camera->renderArea.scissor);
			}
				s_framegraph.Draw(commandBuffer);

			//m_componentIdSubBuffer = BufferManager::Reserve(m_width * m_height * sizeof(uint32), false, BufferType::View);
			vk::BufferImageCopy copy(
				s_componentIdSubBuffer.offset,
				0, 0,
				//m_width, m_height,
				vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
				vk::Offset3D(), vk::Extent3D(s_width, s_height, 1)
			);

			//commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlagBits::eByRegion, )

			commandBuffer.copyImageToBuffer(
				s_frameResourcesManager.GetImage(s_componentIdImageIndex).GetImage(),
				vk::ImageLayout::eTransferSrcOptimal,
				s_componentIdSubBuffer.buffer->Get(),
				copy
			);

			commandBuffer.end();
		}

		void RenderSystem::DrawDebug3DLine(const Vector3& begin, const Vector3& end)
		{
			auto line = std::make_unique<MeshRenderer>(nullptr);

			Mesh* lineMesh = s_resourceManager.GetMesh("primitive:Line " + begin.ToString() + end.ToString());
			if (lineMesh == nullptr)
			{
				lineMesh = s_resourceManager.Register("primitive:Line " + begin.ToString() + end.ToString(), Mesh::Line::Create(begin, end));
				lineMesh->Flush();
			}

			uint32 lineMeshIndex = RegisterMesh(lineMesh);
			line->SetMesh(lineMeshIndex);
			//uint32 lineMaterial = RegisterMaterial(s_resourceManager.materialManager.CreateMaterial())

			uint32 lineMaterial;
			{
				Material debugMaterial;
				//debugMaterial.materialPipelineInfo.SetDepthTest(false);
				debugMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eLine);
				debugMaterial.materialPipelineInfo.SetPrimitiveTopology(vk::PrimitiveTopology::eLineList);
				debugMaterial.materialInfo.receiveLights = false;
				debugMaterial.materialInfo.isTransparent = true;
				s_resourceManager.materialManager.CreateMaterial(debugMaterial, sb::ShaderBuilder::MakeShaderProgram);
				lineMaterial = RegisterMaterial(std::move(debugMaterial));
			}

			line->SetMaterial(lineMaterial);

			s_debug3DLines.emplace_back(std::move(line));

			ReserveModelUBD(s_debug3DLines.back().get());

			//Register(s_debug3DLines.back().get());
		}

		void RenderSystem::Resize(uint32 width, uint32 height)
		{
			s_width = width;
			s_height = height;

			s_componentIdSubBuffer = BufferManager::Reserve(width * height * sizeof(uint32) * 2, false, BufferType::View);

			s_frameResourcesManager.Resize(width, height);
			s_framegraph.Resize(s_frameResourcesManager, width, height);

			const Material& lastLightScatteringMaterial = s_materials[s_lastLightScatteringMaterialIndex];
			lastLightScatteringMaterial.SetSampledTexture(s_frameResourcesManager.GetImage(s_firstLightScatterImageIndex), 0);
			//s_lastLightScatteringMaterial.SetSampledTexture(s_frameResourcesManager.GetImage(s_firstLightScatterImageIndex), 0);
		}

		uint32 RenderSystem::RegisterMaterial(Material&& mat)
		{
			if (mat.materialInfo.hasViewUBO)
				mat.SetViewUBO(s_viewUBO);

			if (mat.materialInfo.hasLightUBO)
				mat.SetLightUBO(s_lightsUBO);

			//mat.FlushTextures();

			if (mat.materialInfo.hasDiffuseMap && mat.GetDiffuse() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetDiffuse()], mat.GetDiffuseBinding());
			if (mat.materialInfo.hasHeightMap && mat.GetHeight() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetHeight()], mat.GetHeightBinding());
			if (mat.materialInfo.hasNormalMap && mat.GetNormal() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetNormal()], mat.GetNormalBinding());
			if (mat.materialInfo.hasSpecularMap && mat.GetSpecular() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetSpecular()], mat.GetSpecularBinding());
			if (mat.materialInfo.hasEnvironmentMap && mat.GetEnvironment() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetEnvironment()], mat.GetEnvironmentBinding());
			if (mat.materialInfo.hasReflectionMap && mat.GetReflection() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetReflection()], mat.GetReflectionBinding());
			if (mat.materialInfo.hasRefractionMap && mat.GetRefraction() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetRefraction()], mat.GetRefractionBinding());
			if (mat.materialInfo.hasEmissiveMap && mat.GetEmissive() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetEmissive()], mat.GetEmissiveBinding());
			if (mat.materialInfo.hasOcclusionMap && mat.GetOcclusion() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetOcclusion()], mat.GetOcclusionBinding());
			if (mat.materialInfo.hasMetalicRoughnessMap && mat.GetMetalicRoughness() != ~0)
				mat.SetSampledTexture(s_textures[mat.GetMetalicRoughness()], mat.GetMetalicRoughnessBinding());

			s_materials.emplace_back(std::move(mat));

			return s_materials.size() - 1;
		}

		void RenderSystem::CopyModelUBD(vk::CommandBuffer commandBuffer)
		{
			for (const ModelUBDGroup& group : s_modelUBDGroups)
			{
				group.dynamicPool->UniformBufferDynamic().Copy(commandBuffer);
			}
		}

		RenderSystem::ModelUBDGroup& RenderSystem::FindOrCreateSuitableGroupModelUBD()
		{
			for (ModelUBDGroup& group : s_modelUBDGroups)
			{
				if (group.dynamicPool->AvailableBlockCount() > 0)
					return group;
			}

			s_modelUBDGroups.emplace_back();
			s_modelUBDGroups.back().dynamicPool = std::make_unique<UniformBufferDynamicPool>();
			s_modelUBDGroups.back().dynamicPool->Init(s_modelUBDSize, s_modelUBDCountPerPool);

			s_modelUBDGroups.back().descriptorPool.Create({ s_modelUBDLayoutInfo });

			auto descriptorSets = Context::GetDevice().Get().allocateDescriptorSetsUnique(
				vk::DescriptorSetAllocateInfo(
					s_modelUBDGroups.back().descriptorPool.Get(),
					1,
					&s_modelUBDLayout.get()
				)
			);

			s_modelUBDGroups.back().descriptorSet.swap(descriptorSets[0]);

			DescriptorSet dset(s_modelUBDGroups.back().descriptorSet.get());
			//dset.SetBufferToBinding(0, s_modelUBDDescriptorSetLayoutInfo, *s_modelUBD.GetDestination().buffer, s_modelUBD.GetDestination().elementSize, s_modelUBD.GetDestination().offset);
			dset.SetBufferToBinding(
				0,
				s_modelUBDLayoutInfo,
				*s_modelUBDGroups.back().dynamicPool->UniformBufferDynamic().GetDestination().buffer,
				s_modelUBDGroups.back().dynamicPool->UniformBufferDynamic().GetDestination().elementSize,
				s_modelUBDGroups.back().dynamicPool->UniformBufferDynamic().GetDestination().offset
			);

			UpdateCopyCommandBuffer();

			return s_modelUBDGroups.back();
		}
	}
}
