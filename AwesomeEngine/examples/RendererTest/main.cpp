#include <Awesome/SDK.hpp>

using namespace aw;

// TODO: Fix later
//struct Conf
//{
//public:
//	struct
//	{
//		uint8 monitorId = 0;
//		uint16 width = 800;
//		uint16 height = 600;
//
//		struct
//		{
//			uint16 x = 0;
//			uint16 y = 0;
//		} pos;
//
//		std::string name = "Window";
//
//	} window;
//
//	Conf()
//	{
//		sdk::LuaScript conf("config.lua");
//		conf.Run();
//
//		const luwra::StateWrapper& state = static_cast<luwra::StateWrapper>(conf);
//
//		if (state.has("window"))
//		{
//			luwra::Table windowTable = state["window"];
//
//			if(windowTable.has("monitorId"))
//				window.monitorId = windowTable["monitorId"];
//
//			if(windowTable.has("width"))
//				window.width = windowTable["width"];
//			if (windowTable.has("height"))
//				window.height = windowTable["height"];
//
//			if (windowTable.has("pos"))
//			{
//				luwra::Table posTable = windowTable["pos"];
//
//				if (posTable.has("x"))
//					window.pos.x = posTable["x"];
//				if (posTable.has("y"))
//					window.pos.y = posTable["y"];
//			}
//
//			if(windowTable.has("name"))
//				window.name = std::string(static_cast<const char*>(windowTable["name"]));
//		}
//
//		if (state.has("logVerbosity"))
//			SET_LOG_MIN_SEVERITY(static_cast<Severity>(state["logVerbosity"]));
//	}
//};
//
//static Conf config;

class MainApplication : public sdk::WindowApplication
{
	//vk::UniqueCommandBuffer copyCommandBuffer;

	//FontAtlas fontAtlas;

	sdk::Text2D* textMeshRenderer1{ nullptr };
	sdk::Text2D* textMeshRenderer2{ nullptr };
	sdk::Text2D* textMeshRenderer3{ nullptr };

	const VertexLayout& skyboxVertexLayout{ VertexP::Layout };
	const VertexLayout& vertexLayout{ VertexPTNTaH::Layout };

	// sdk::ResourceManager resourceManager;
	RenderingResources renderingResources{};

	std::unique_ptr<sdk::Scene> m_scene = nullptr;

	sdk::MeshRenderer* hemisphereMeshRenderer = nullptr;
	sdk::MeshRenderer* coneMeshRenderer = nullptr;
	sdk::MeshRenderer* cylindreMeshRenderer = nullptr;
	sdk::MeshRenderer* capsuleMeshRenderer = nullptr;
	sdk::Camera* cameraComponent = nullptr;
	//sdk::SoundEmitter* testEmitter;

	//GUIBenchmark guiBenchmark{ &frameTime };

	float rotationSpeed = 0.5f;
	float translationSpeed = 3.0;
	float mouseSpeed = M_PI2 + 1.0f / 3.0f;

	float rotatorTest = 0.5f;

public:
	//Use it when Conf class fixed
	//MainApplication() : WindowApplication(config.window.width, config.window.height, config.window.name.c_str(), config.window.pos.x, config.window.pos.y, config.window.monitorId)
	//{
	//}

	void Start() override
	{
		Context::LogVulkan = false;

		WindowApplication::Start();

		auto& window = *m_mainWindow.get();

		if (!sound::LoadBank("Music.bnk"))
			THROW("Failed to load Music soundbank");

		m_scene = std::make_unique<sdk::Scene>();

		sdk::ResourceManager& resourceManager = m_scene->GetRenderSystem().GetResourceManager();

		//m_scene = std::make_unique<sdk::Scene>();
		//resourceManager.Load("assets/sphere.obj");

		//resourceManager.Load("assets/glTF/DamagedHelmet/DamagedHelmet.gltf");

		//GLTFImporter::Import("assets/glTF/MetalRoughSpheres/MetalRoughSpheres.gltf", resourceManager);

		//sdk::Scene scene;

		Mesh* skyboxMesh = resourceManager.Load<sdk::OBJLoader>("assets/cube/cube.obj", &skyboxVertexLayout);
		skyboxMesh->Flush();
		uint32 skyboxMeshIndex = m_scene->GetRenderSystem().RegisterMesh(skyboxMesh);

		Mesh* squareMesh = resourceManager.Register("primitive:Square", Mesh::Quad::Create());
		squareMesh->Flush();

		Mesh* debugSquareMesh = resourceManager.Register("debug:Square", Mesh::DebugQuad::Create(1.0f));
		debugSquareMesh->Flush();

		Mesh* cubeMesh = resourceManager.Register("primitive:Cube", Mesh::Cube::Create());
		cubeMesh->Flush();
		uint32 cubeMeshIndex = m_scene->GetRenderSystem().RegisterMesh(cubeMesh);

		Mesh* debugCubeMesh = resourceManager.Register("debug:Cube", Mesh::DebugCube::Create(1.0f));
		debugCubeMesh->Flush();
		uint32 debugCubeMeshIndex = m_scene->GetRenderSystem().RegisterMesh(debugCubeMesh);

		Mesh* sphereMesh = resourceManager.Register("primitive:UVSphere", Mesh::Sphere<>::CreateUVSphere());
		sphereMesh->Flush();
		uint32 sphereMeshIndex = m_scene->GetRenderSystem().RegisterMesh(sphereMesh);

		Mesh* debugSphereMesh = resourceManager.Register("debug:Sphere", Mesh::DebugSphere::Create(0.5f));
		debugSphereMesh->Flush();
		uint32 debugSphereMeshIndex = m_scene->GetRenderSystem().RegisterMesh(debugSphereMesh);

		Mesh* hemisphereMesh = resourceManager.Register("primitive:Hemiphere", Mesh::Hemisphere<>::Create(100.0f));
		//hemisphereMesh->ReverseFaces();
		hemisphereMesh->Flush();
		uint32 hemisphereMeshIndex = m_scene->GetRenderSystem().RegisterMesh(hemisphereMesh);

		uint32 quadMeshIndex;
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

			Mesh* quadMeshPtr = resourceManager.Register("primitive:Quad", std::move(quadMesh));
			quadMeshIndex = m_scene->GetRenderSystem().RegisterMesh(quadMeshPtr);
		}

		Mesh* coneMesh = resourceManager.Register("primitive:Cone", Mesh::Cone<>::Create(0.5f, 2.0f));
		coneMesh->Flush();
		uint32 coneMeshIndex = m_scene->GetRenderSystem().RegisterMesh(coneMesh);

		Mesh* cylinderMesh = resourceManager.Register("primitive:Cylinder", Mesh::Cylinder<>::Create());
		cylinderMesh->Flush();
		uint32 cylinderMeshIndex = m_scene->GetRenderSystem().RegisterMesh(cylinderMesh);

		Mesh* capsuleMesh = resourceManager.Register("primitive:Capsule", Mesh::Capsule<>::Create());
		capsuleMesh->Flush();
		uint32 capsuleMeshIndex = m_scene->GetRenderSystem().RegisterMesh(capsuleMesh);

		Mesh* debugCapsuleMesh = resourceManager.Register("debug:Capsule", Mesh::DebugCapsule::Create(0.5f, 1.0f));
		debugCapsuleMesh->Flush();
		uint32 debugCapsuleMeshIndex = m_scene->GetRenderSystem().RegisterMesh(debugCapsuleMesh);

		//FontAtlas fontAtlas = sdk::FontLoader::Load(resourceManager, "c:/windows/fonts/times.ttf", 96.0f);
		//fontAtlas = sdk::FontLoader::Load(resourceManager, "assets/fonts/DroidSans.ttf", 64.0f);
		//fontAtlas = sdk::FontLoader::Load(resourceManager, "c:/windows/fonts/arial.ttf", 14.0f);

		Mesh* plane;
		{
			Mesh _plane;
			std::vector<uint8> vertices(4 * VertexPT::Size);
			std::vector<uint8> indices(6 * sizeof(uint16));

			_plane.indexType = vk::IndexType::eUint16;
			_plane.vertexLayout = &VertexPT::Layout;

			VectorView<VertexPT> verticesView(vertices);
			VectorView<uint16> indicesView(indices);

			verticesView[0].Get<PositionXYZ,	Vector3>() = Vector3(-0.5f, -0.5f, 0.0f);
			verticesView[0].Get<TextureUV,		Vector2>() = Vector2(0.0f, 1.0f);
			verticesView[1].Get<PositionXYZ,	Vector3>() = Vector3(0.5f, -0.5f, 0.0f);
			verticesView[1].Get<TextureUV,		Vector2>() = Vector2(1.0f, 1.0f);
			verticesView[2].Get<PositionXYZ,	Vector3>() = Vector3(0.5f, 0.5f, 0.0f);
			verticesView[2].Get<TextureUV,		Vector2>() = Vector2(1.0f, 0.0f);
			verticesView[3].Get<PositionXYZ,	Vector3>() = Vector3(-0.5f, 0.5f, 0.0f);
			verticesView[3].Get<TextureUV,		Vector2>() = Vector2(0.0f, 0.0f);

			indicesView[0] = 0;
			indicesView[1] = 1;
			indicesView[2] = 3;
			indicesView[3] = 1;
			indicesView[4] = 2;
			indicesView[5] = 3;

			_plane.SetVertices(std::move(vertices));
			_plane.SetIndices(std::move(indices));

			_plane.Flush();

			plane = resourceManager.Register("primitive:Plane", std::move(_plane));
		}

		//text = resourceManager.Register("text", Mesh());
		//text->indexType = vk::IndexType::eUint16;
		//text->vertexLayout = &VertexPT::Layout;

		SampledTextureHandle diffuseTexture;
		SampledTextureHandle normalTexture;
		SampledTextureHandle specularTexture;
		SampledTextureHandle heightTexture;
		SampledTextureHandle skyboxTexture;
		SampledTextureHandle sphereEnvironmentMap;
		SampledTextureHandle whiteTexture;
		SampledTextureHandle blueTexture;
		SampledTextureHandle redTexture;
		//SampledTextureHandle atlasTexture;
		{
			vk::CommandBufferAllocateInfo cpyCmdBufAllocInfo(Context::GetCommandPool(), vk::CommandBufferLevel::ePrimary, 1);
			vk::UniqueCommandBuffer copyCmdBuffer(
				std::move(Context::GetDevice().Get().allocateCommandBuffersUnique(cpyCmdBufAllocInfo)[0])
			);

			TexelBuffer* texelBuffer{ nullptr };

			//texelBuffer = resourceManager.Load<sdk::TextureLoader>("assets/textures/debug_texture.png");
			texelBuffer = resourceManager.Load<sdk::TextureLoader>("assets/textures/piso_patio_exterior.png");

			SampledTexture diffuseTextureUnique;
			diffuseTextureUnique.Create(
				vk::ImageType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				*texelBuffer,
				copyCmdBuffer.get(),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			diffuseTexture = resourceManager.Register("diffuseTexture", std::move(diffuseTextureUnique));

			texelBuffer = resourceManager.Load<sdk::TextureLoader>("assets/textures/N_piso_patio_exterior_displace_inv.png", sdk::TexelBufferLoadComponent::RGBAlpha, sdk::TexelBufferLoadParamsBit::InvertR);
			SampledTexture normalTextureUnique;
			normalTextureUnique.Create(
				vk::ImageType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				*texelBuffer,
				copyCmdBuffer.get(),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			normalTexture = resourceManager.Register("normalTexture", std::move(normalTextureUnique));

			texelBuffer = resourceManager.Load<sdk::TextureLoader>("assets/textures/piso_patio_exterior_displace_inv2.png", sdk::TexelBufferLoadComponent::RGBAlpha, sdk::TexelBufferLoadParamsBit::InvertR);
			SampledTexture specularTextureUnique;
			specularTextureUnique.Create(
				vk::ImageType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				*texelBuffer,
				copyCmdBuffer.get(),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			specularTexture = resourceManager.Register("specularTexture", std::move(specularTextureUnique));

			texelBuffer = resourceManager.Load<sdk::TextureLoader>("assets/textures/piso_patio_exterior_displace_inv.png");
			SampledTexture heightTextureUnique;
			heightTextureUnique.Create(
				vk::ImageType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				*texelBuffer,
				copyCmdBuffer.get(),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			heightTexture = resourceManager.Register("heightTexture", std::move(heightTextureUnique));

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

			//SampledTexture atlasTextureUnique(samplerCreateInfo);
			//atlasTextureUnique.Create(
			//	vk::ImageType::e2D,
			//	vk::Format::eR8G8B8A8Unorm,
			//	fontAtlas.atlas,
			//	copyCmdBuffer.get(),
			//	vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
			//	vk::ImageAspectFlagBits::eColor
			//);
			//atlasTexture = resourceManager.Register("atlasTexture", std::move(atlasTextureUnique));

			{
				TexelBuffer texelBuffer;
				texelBuffer.width = 1;
				texelBuffer.height = 1;
				texelBuffer.channels = 4;
				texelBuffer.texels = { 255, 255, 255, 255 };
				SampledTexture whiteTextureUnique;
				whiteTextureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					copyCmdBuffer.get(),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				whiteTexture = resourceManager.Register("whiteTexture", std::move(whiteTextureUnique));

				texelBuffer.texels = { 255, 0, 0, 255 };
				SampledTexture redTextureUnique;
				redTextureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					copyCmdBuffer.get(),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				redTexture = resourceManager.Register("redTexture", std::move(redTextureUnique));

				texelBuffer.texels = { 0, 0, 255, 255 };
				SampledTexture blueTextureUnique;
				blueTextureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					copyCmdBuffer.get(),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				blueTexture = resourceManager.Register("blueTexture", std::move(blueTextureUnique));
			}

			TexelBuffer yokohamaSkybox = sdk::TextureLoader::LoadSkybox("assets/skybox/Yokohama3");
			SampledTexture skyboxTextureUnique;
			skyboxTextureUnique.Create(
				vk::ImageType::e3D,
				vk::Format::eR8G8B8A8Unorm,
				yokohamaSkybox,
				copyCmdBuffer.get(),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			resourceManager.Register("YokohamaSkyboxTexels", std::move(yokohamaSkybox));
			skyboxTexture = resourceManager.Register("skyboxTexture", std::move(skyboxTextureUnique));

			vk::Extent3D envExtent{
				512,
				512,
				1
			};
			SampledTexture sphereEnvironmentMapUnique;
			sphereEnvironmentMapUnique.Create(
				vk::ImageType::e3D,
				vk::Format::eR8G8B8A8Unorm,
				envExtent,
				vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			sphereEnvironmentMap = resourceManager.Register("sphereEnvironmentMap", std::move(sphereEnvironmentMapUnique));
		}

		m_scene->GetRenderSystem().Resize(window->GetWidth(), window->GetHeight());

		uint32 litMaterialIndex;
		{
			Material litMaterial;
			litMaterial.SetDiffuse(diffuseTexture);
			litMaterial.SetNormal(normalTexture);
			litMaterial.SetSpecular(specularTexture);
			litMaterial.SetHeight(heightTexture);
			litMaterial.materialInfo.receiveLights = true;
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(litMaterial, sb::ShaderBuilder::MakeShaderProgram);
			litMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(litMaterial));
		}

		uint32 debugMaterialIndex;
		{
			Material debugMaterial;
			//debugMaterial.materialPipelineInfo.SetDepthTest(false);
			debugMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eLine);
			debugMaterial.materialPipelineInfo.SetPrimitiveTopology(vk::PrimitiveTopology::eLineList);
			debugMaterial.materialInfo.receiveLights = false;
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(debugMaterial, sb::ShaderBuilder::MakeShaderProgram);
			debugMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(debugMaterial));
		}

		uint32 unlitMaterialIndex;
		{
			Material unlitMaterial;
			unlitMaterial.SetDiffuse(whiteTexture);
			unlitMaterial.SetSpecular(whiteTexture);
			unlitMaterial.materialInfo.receiveLights = false;
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(unlitMaterial, sb::ShaderBuilder::MakeShaderProgram);
			unlitMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(unlitMaterial));
		}

		uint32 unlitMaterialBlueIndex;
		{
			Material unlitMaterialBlue;
			unlitMaterialBlue.SetDiffuse(whiteTexture);
			unlitMaterialBlue.SetSpecular(blueTexture);
			unlitMaterialBlue.materialInfo.receiveLights = false;
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(unlitMaterialBlue, sb::ShaderBuilder::MakeShaderProgram);
			unlitMaterialBlueIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(unlitMaterialBlue));
		}

		uint32 unlitMaterialRedIndex;
		{
			Material unlitMaterialRed;
			unlitMaterialRed.SetDiffuse(whiteTexture);
			unlitMaterialRed.SetSpecular(redTexture);
			unlitMaterialRed.materialInfo.receiveLights = false;
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(unlitMaterialRed, sb::ShaderBuilder::MakeShaderProgram);
			unlitMaterialRedIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(unlitMaterialRed));
		}

		uint32 awesomeMaterialIndex;
		{
			Material awesomeMaterial;
			awesomeMaterial.SetDiffuse(diffuseTexture);
			awesomeMaterial.SetSpecular(specularTexture);
			awesomeMaterial.SetNormal(normalTexture);
			awesomeMaterial.SetHeight(heightTexture);
			//awesomeMaterial.SetReflection(whiteTexture);
			awesomeMaterial.SetRefraction(specularTexture);
			awesomeMaterial.SetEnvironment(skyboxTexture);
			awesomeMaterial.materialInfo.receiveLights = true;
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(awesomeMaterial, sb::ShaderBuilder::MakeShaderProgram);
			awesomeMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(awesomeMaterial));
		}

		uint32 reflectMaterialIndex;
		{
			Material reflectMaterial;
			//      reflectMaterial.SetDiffuse(whiteTexture);
			//      reflectMaterial.SetSpecular(whiteTexture);
			reflectMaterial.SetReflection(whiteTexture);
			reflectMaterial.SetEnvironment(skyboxTexture);
			reflectMaterial.materialInfo.receiveLights = false;
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(reflectMaterial, sb::ShaderBuilder::MakeShaderProgram);
			reflectMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(reflectMaterial));
		}

		uint32 refractMaterialIndex;
		{
			Material refractMaterial;
			refractMaterial.SetRefraction(whiteTexture);
			refractMaterial.SetEnvironment(skyboxTexture);
			refractMaterial.materialInfo.receiveLights = false;
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(refractMaterial, sb::ShaderBuilder::MakeShaderProgram);
			refractMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(refractMaterial));
		}

		//uint32 textMaterialIndex;
		//{
		//	Material textMaterial;
		//	textMaterial.SetDiffuse(atlasTexture);
		//	textMaterial.materialInfo.receiveLights = false;
		//	textMaterial.materialInfo.correctGamma = false;
		//	textMaterial.materialInfo.isTransparent = true;
		//	textMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
		//	textMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eFill);
		//	m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(textMaterial, sb::ShaderBuilder::MakeShaderProgram);
		//	textMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(textMaterial));
		//}

		uint32 skyboxMaterialIndex;
		{
			Material skyboxMaterial;
			skyboxMaterial.materialInfo.isSkybox = true;
			skyboxMaterial.materialPipelineInfo.SetDepthCompareOp(vk::CompareOp::eLessOrEqual);
			skyboxMaterial.materialPipelineInfo.SetDepthWriteEnabled(false);
			skyboxMaterial.SetDiffuse(skyboxTexture);
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(skyboxMaterial, sb::ShaderBuilder::MakeSkyboxShaderProgram);
			skyboxMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(skyboxMaterial));
		}

		uint32 wireframeMaterialIndex;
		{
			Material wireframeMaterial;
			wireframeMaterial.SetDiffuse(whiteTexture);
			wireframeMaterial.SetSpecular(whiteTexture);
			wireframeMaterial.materialInfo.receiveLights = false;
			wireframeMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
			wireframeMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eLine);
			wireframeMaterial.materialPipelineInfo.SetDepthWriteEnabled(false);
			m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(wireframeMaterial, sb::ShaderBuilder::MakeShaderProgram);
			wireframeMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(wireframeMaterial));
		}

		// === TEXT ===

		//auto actorText =  m_scene->CreateActor();
		//ASSERT(!actorText, "actorText is nullptr");

		//textMeshRenderer1 = actorText->AddComponent<sdk::Text2D>();
		//ASSERT(!textMeshRenderer1, "textMeshRenderer1 is nullptr");
		//textMeshRenderer1->SetGlobalPosition({ 5.0f, 50.0f, 0.0f });
		//textMeshRenderer1->SetContent("hop");

		//textMeshRenderer2 = actorText->AddComponent<sdk::Text2D>();
		//ASSERT(!textMeshRenderer2, "textMeshRenderer2 is nullptr");
		//textMeshRenderer2->SetGlobalPosition({ 5.0f, 110.0f, 0.0f });
		//textMeshRenderer2->SetContent("hop");

		//textMeshRenderer3 = actorText->AddComponent<sdk::Text2D>();
		//ASSERT(!textMeshRenderer3, "textMeshRenderer3 is nullptr");
		//textMeshRenderer3->SetGlobalPosition({ 5.0f, 170.0f, 0.0f });
		//textMeshRenderer3->SetContent("hop");

		//textMeshRenderer2->SetGlobalRotation(Quaternion::Identity());

		//textMeshRenderer2->SetScale(5.0f);
		// === Lights ===

		auto actorMoveLight = m_scene->CreateActor();
		ASSERT(!actorMoveLight, "actorMoveLight is nullptr");

		auto lightScript = actorMoveLight->AddComponent<sdk::Script>(nullptr, "assets/scripts/light.lua");
		ASSERT(!lightScript, "lightScript is nullptr");

		auto lightMeshRenderer = actorMoveLight->AddComponent<sdk::MeshRenderer>();
		ASSERT(!lightMeshRenderer, "lightMeshRenderer is nullptr");
		lightMeshRenderer->SetScale(0.2f);
		lightMeshRenderer->SetMesh(sphereMeshIndex);
		lightMeshRenderer->SetMaterial(unlitMaterialIndex);

		/*auto pointLightComponent = actorMoveLight->AddComponent<sdk::PointLight>();
		ASSERT(!pointLightComponent, "pointLightComponent 1 is nullptr");
		pointLightComponent->color = Color4f(300.0f, 300.0f, 300.0f, 1.0f);*/

		sdk::Actor* actorStaticLight = m_scene->CreateActor();
		ASSERT(!actorStaticLight, "actorStaticLight is nullptr");

		auto directionalLightComponent = actorStaticLight->AddComponent<sdk::DirectionalLight>();
		ASSERT(!directionalLightComponent, "directionalLightComponent is nullptr");
		directionalLightComponent->color = Color4f::Blue();
		directionalLightComponent->intensity = 300;
		directionalLightComponent->SetGlobalRotation(Quaternion::FromEulerAngles(Vector3(0, 0, 0)));

		/*pointLightComponent = actorStaticLight->AddComponent<sdk::PointLight>();
		ASSERT(!pointLightComponent, "pointLightComponent 2 is nullptr");
		pointLightComponent->color = Color4f::Blue();
		pointLightComponent->SetGlobalPosition({ 0.0f, -5.0f, -5.0f });


		lightMeshRenderer = actorStaticLight->AddComponent<sdk::MeshRenderer>(pointLightComponent);
		ASSERT(!lightMeshRenderer, "lightMeshRenderer is nullptr");
		lightMeshRenderer->SetScale(0.2f);
		lightMeshRenderer->SetMesh(sphereMeshIndex);
		lightMeshRenderer->SetMaterial(unlitMaterialBlueIndex);*/

		/*pointLightComponent = actorStaticLight->AddComponent<sdk::PointLight>();
		ASSERT(!pointLightComponent, "pointLightComponent 3 is nullptr");
		pointLightComponent->color = Color4f::Red();
		pointLightComponent->SetGlobalPosition({ 0.0f, 5.0f, 0.0f });*/

		/*lightMeshRenderer = actorStaticLight->AddComponent<sdk::MeshRenderer>(pointLightComponent);
		ASSERT(!lightMeshRenderer, "lightMeshRenderer is nullptr");
		lightMeshRenderer->SetScale(0.2f);
		lightMeshRenderer->SetScale(0.2f);
		lightMeshRenderer->SetMesh(sphereMeshIndex);
		lightMeshRenderer->SetMaterial(unlitMaterialRedIndex);*/

		/*for (uint64 i{ 0 }; i < 50; i++)
		{
			lightMeshRenderer = actorStaticLight->AddComponent<sdk::MeshRenderer>(lightMeshRenderer);
			ASSERT(!lightMeshRenderer, "lightMeshRenderer is nullptr");
			lightMeshRenderer->SetScale(0.2f);
			lightMeshRenderer->SetScale(0.2f);
			lightMeshRenderer->SetMesh(sphereMeshIndex);
			lightMeshRenderer->SetMaterial(unlitMaterialRedIndex);
			lightMeshRenderer->SetLocalPosition({ 0.0f, 1.0f, 0.0f });
		}*/

		// === Cube ===

		sdk::Actor* cubeActor = m_scene->CreateActor();
		ASSERT(!cubeActor, "cubeActor is nullptr");

		auto cubeMeshRenderer = cubeActor->AddComponent<sdk::MeshRenderer>();
		ASSERT(!cubeMeshRenderer, "cubeMeshRenderer is nullptr");
		cubeMeshRenderer->SetLocalPosition({ 0.0f, -3.0f, -3.0f });
		cubeMeshRenderer->SetMesh(cubeMeshIndex);
		cubeMeshRenderer->SetMaterial(litMaterialIndex);
		cubeActor->SetLocalPosition({ -1.5f, 0.0f, -5.0f });

		// === Spheres ===

		sdk::Actor* actor1 = m_scene->CreateActor();
		ASSERT(!actor1, "actor1 is nullptr");

		auto meshRenderer1 = actor1->AddComponent<sdk::MeshRenderer>();
		ASSERT(!meshRenderer1, "meshRenderer 1 is nullptr");
		meshRenderer1->SetMesh(sphereMeshIndex);
		meshRenderer1->SetMaterial(litMaterialIndex);

		auto meshRenderer1Behaviour = actor1->AddComponent<sdk::Script>(meshRenderer1, "assets/scripts/mesh1_behaviour.lua");
		ASSERT(!meshRenderer1Behaviour, "meshRenderer1Behaviour is nullptr");

		//meshRenderer1->SetScale(0.5f);
		meshRenderer1->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		//meshRenderer1->SetLocalRotation(Quaternion(M_PI2, { 0.0f, 1.0f, 0.0f }));

		auto meshRenderer2 = actor1->AddComponent<sdk::MeshRenderer>(meshRenderer1);
		ASSERT(!meshRenderer2, "meshRenderer 2 is nullptr");
		meshRenderer2->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		meshRenderer2->SetMesh(sphereMeshIndex);
		meshRenderer2->SetMaterial(awesomeMaterialIndex);

		auto meshRenderer2Behaviour = actor1->AddComponent<sdk::Script>(meshRenderer2, "assets/scripts/mesh2_behaviour.lua");
		ASSERT(!meshRenderer2Behaviour, "meshRenderer2Behaviour is nullptr");

		auto meshRenderer3 = actor1->AddComponent<sdk::MeshRenderer>(meshRenderer2);
		ASSERT(!meshRenderer3, "meshRenderer 3 is nullptr");
		meshRenderer3->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		meshRenderer3->SetMesh(sphereMeshIndex);
		meshRenderer3->SetMaterial(reflectMaterialIndex);

		auto meshRenderer3Behaviour = actor1->AddComponent<sdk::Script>(meshRenderer3, "assets/scripts/mesh3_behaviour.lua");
		ASSERT(!meshRenderer3Behaviour, "meshRenderer3Behaviour is nullptr");

		auto meshRenderer4 = actor1->AddComponent<sdk::MeshRenderer>(meshRenderer3);
		ASSERT(!meshRenderer4, "meshRenderer 4 is nullptr");
		meshRenderer4->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		meshRenderer4->SetMesh(sphereMeshIndex);
		meshRenderer4->SetMaterial(refractMaterialIndex);

		auto meshRenderer4Behaviour = actor1->AddComponent<sdk::Script>(meshRenderer4, "assets/scripts/mesh4_behaviour.lua");
		ASSERT(!meshRenderer4Behaviour, "meshRenderer4Behaviour is nullptr");

		auto soundEmitter = actor1->AddComponent<sdk::SoundEmitter>(meshRenderer4);
		ASSERT(!soundEmitter, "soundEmitter is nullptr");

		sdk::Actor* actor2 =  m_scene->CreateActor();
		ASSERT(!actor2, "actor2 is nullptr");

		//sdk::GLTFImporter::Import("assets/glTF/MetalRoughSpheres/MetalRoughSpheres.gltf", resourceManager, actor2);
		sdk::GLTFImporter::Import("assets/glTF/DamagedHelmet/DamagedHelmet.gltf", *m_scene, actor2);
		//sdk::GLTFImporter::Import("assets/glTF/NormalTangentTest/NormalTangentTest.gltf", resourceManager, actor2);
		//sdk::GLTFImporter::Import("assets/glTF/NormalTangentMirrorTest/NormalTangentMirrorTest.gltf", resourceManager, actor2);

		//actor2->Translate_WorldSpace(Vector3(0, 0, 10));
		actor2->Rotate_WorldSpace(Quaternion(0.7, 0, 0.7, 0));

		//sdk::Actor* actor3 = m_scene->CreateActor();
		//ASSERT(!actor3, "actor3 is nullptr");

		//sdk::GLTFImporter::Import("assets/glTF/MetalRoughSpheres/MetalRoughSpheres.gltf", *m_scene, actor3);
		//actor3->Translate_WorldSpace(Vector3(0, 0, 10));

		// === Hemisphere ===

		sdk::Actor* hemisphereActor = m_scene->CreateActor();
		ASSERT(!hemisphereActor, "hemisphereActor is nullptr");

		hemisphereMeshRenderer = hemisphereActor->AddComponent<sdk::MeshRenderer>();
		ASSERT(!hemisphereMeshRenderer, "hemisphereMeshRenderer is nullptr");
		hemisphereMeshRenderer->SetLocalPosition({ 0.0f, 0.0f, -10.0f });
		hemisphereMeshRenderer->SetMesh(hemisphereMeshIndex);
		hemisphereMeshRenderer->SetMaterial(litMaterialIndex);
		hemisphereMeshRenderer->isLightScatteringEmitter = true;

		// === Cone ===

		sdk::Actor* coneActor = m_scene->CreateActor();
		ASSERT(!coneActor, "coneActor is nullptr");

		coneMeshRenderer = coneActor->AddComponent<sdk::MeshRenderer>();
		ASSERT(!coneMeshRenderer, "coneMeshRenderer is nullptr");
		coneActor->SetLocalPosition({ 0.0f, -1.2f, -5.0f });
		coneMeshRenderer->SetLocalRotation({ 0.0f, 0.0f, 0.0f, 1.0f });
		coneMeshRenderer->SetMesh(coneMeshIndex);
		coneMeshRenderer->SetMaterial(litMaterialIndex);

		// === Cylindre ===

		sdk::Actor* cylindreActor = m_scene->CreateActor();
		ASSERT(!cylindreActor, "cylindreActor is nullptr");

		cylindreMeshRenderer = cylindreActor->AddComponent<sdk::MeshRenderer>();
		ASSERT(!cylindreMeshRenderer, "cylindreMeshRenderer is nullptr");
		cylindreMeshRenderer->SetLocalPosition({ 1.5f, 0.0f, -5.0f });
		cylindreMeshRenderer->SetMesh(cylinderMeshIndex);
		cylindreMeshRenderer->SetMaterial(litMaterialIndex);

		// === Capsule ===

		sdk::Actor* capsuleActor = m_scene->CreateActor();
		ASSERT(!capsuleActor, "capsuleActor is nullptr");

		capsuleMeshRenderer = capsuleActor->AddComponent<sdk::MeshRenderer>();
		ASSERT(!capsuleMeshRenderer, "capsuleMeshRenderer is nullptr");
		capsuleMeshRenderer->SetLocalPosition({ 0.0f, 2.0f, -5.0f });
		capsuleMeshRenderer->SetMesh(capsuleMeshIndex);
		capsuleMeshRenderer->SetMaterial(litMaterialIndex);

		// === Camera & Skybox ===

		auto skyboxComponent = actor1->AddComponent<sdk::Skybox>();
		ASSERT(!skyboxComponent, "skyboxComponent is nullptr");
		skyboxComponent->SetMesh(skyboxMeshIndex);
		skyboxComponent->SetMaterial(skyboxMaterialIndex);

		sdk::Actor* mainCamera = m_scene->CreateActor(nullptr, "MainCamera");
		ASSERT(!mainCamera, "mainCamera is nullptr");

		cameraComponent = mainCamera->AddComponent<sdk::Camera>();
		ASSERT(!cameraComponent, "cameraComponent is nullptr");
		{
			cameraComponent->renderArea.viewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f);
			cameraComponent->renderArea.scissor = vk::Rect2D({}, { window->GetWidth(), window->GetHeight() });
			cameraComponent->SetWidth(cameraComponent->renderArea.viewport.width);
			cameraComponent->SetHeight(cameraComponent->renderArea.viewport.height);
			cameraComponent->SetNear(0.001f);
		}

		auto camPointLightComponent = mainCamera->AddComponent<sdk::PointLight>();
		ASSERT(!camPointLightComponent, "camPointLightComponent is nullptr");

		auto listenerComponent = mainCamera->AddComponent<sdk::SoundListener>();
		ASSERT(!listenerComponent, "listenerComponent is nullptr");

		/*
		auto textMeshRenderer1 = mainCamera->AddComponent<sdk::MeshRenderer>(nullptr, &textModel);
		ASSERT(!textMeshRenderer1, "textMeshRenderer1 is nullptr");
		//textMeshRenderer1->SetGlobalPosition({ -0.25f, 0.25f, -1.01f });
		textMeshRenderer1->SetGlobalPosition({ 5.0f, 20.0f, 0.01f });
		textMeshRenderer1->SetGlobalRotation({ 1, 0, 0, 0 });
		textMeshRenderer1->useTextOrtho = true;
		//*/
		//textMeshRenderer1->SetLocalRotation(Quaternion(3.14159f, Vector3(1, 0, 0)));
		//textMeshRenderer1->SetScale(1.0f / 1280.0f);


		auto cameraScriptComponent = mainCamera->AddComponent<sdk::Script>(nullptr, "assets/scripts/camera.lua");
		ASSERT(!cameraScriptComponent, "cameraScriptComponent is nullptr");

		//auto camteraTestRenderer = mainCamera->AddComponent<MeshRenderer>(cameraComponent, &box);
		//ASSERT(!camteraTestRenderer, "camteraTestRenderer is nullptr");
		//camteraTestRenderer->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		//camteraTestRenderer->SetLocalRotation(Quaternion(M_PI2, Vector3::Right()));

		// === Physics ===

		sdk::Actor* dynamicActor = m_scene->CreateActor();
		ASSERT(!dynamicActor, "dynamicActor is nullptr");

		auto dynamicBody = dynamicActor->AddComponent<sdk::Rigidbody>(nullptr, ph::CapsuleGeometry(), ph::Material(0.5f, 0.5f, 1.0f));
		ASSERT(!dynamicBody, "dynamicBody is nullptr");

		auto dynamicActorRenderer = dynamicActor->AddComponent<sdk::MeshRenderer>(dynamicBody);
		ASSERT(!dynamicActorRenderer, "dynamicActorRenderer is nullptr");
		//dynamicActorRenderer->SetMesh(sphereMeshIndex);
		//dynamicActorRenderer->SetMaterial(litMaterialIndex);

		//auto dynamicActorRenderer = dynamicActor->AddComponent<sdk::MeshRenderer>(dynamicBody, &litCapsule);
		//ASSERT(!dynamicActorRenderer, "dynamicActorRenderer is nullptr");

		//dynamicActor->AddComponent<sdk::MeshRenderer>(dynamicBody);
		dynamicActor->Translate_WorldSpace({ 10.0f, 20.0f, -5.0f });
		dynamicActorRenderer->SetMesh(debugCapsuleMeshIndex);
		dynamicActorRenderer->SetMaterial(debugMaterialIndex);

		 ///___________________________________________________________________

		sdk::Actor* staticActor = m_scene->CreateActor();
		ASSERT(!staticActor, "staticActor is nullptr");

		auto staticBody = staticActor->AddComponent<sdk::SphereCollider>(nullptr);
		ASSERT(!staticBody, "staticBody is nullptr");

		//auto staticActorRenderer = staticActor->AddComponent<sdk::MeshRenderer>(staticBody);
		//ASSERT(!staticActorRenderer, "staticActorRenderer is nullptr");
		//staticActorRenderer->SetMesh(debugSphereMeshIndex);
		//staticActorRenderer->SetMaterial(debugMaterialIndex);

		//auto staticActorRenderer = staticActor->AddComponent<sdk::MeshRenderer>(staticBody, &litSphere);
		//ASSERT(!staticActorRenderer, "staticActorRenderer is nullptr");

		staticActor->SetGlobalPosition({ 10.0f, 0.0f, -5.0f });

		///___________________________________________________________________

		sdk::Actor* triggerActor = m_scene->CreateActor();
		ASSERT(!triggerActor, "triggerActor is nullptr");

		auto triggerBody = triggerActor->AddComponent<sdk::BoxCollider>(nullptr, Vector3(0.5f, 0.5f, 0.5f));
		ASSERT(!triggerBody, "triggerBody is nullptr");
		triggerBody->SetTrigger(true);

		//auto triggerActorRenderer = triggerActor->AddComponent<sdk::MeshRenderer>(triggerBody);
		//ASSERT(!triggerActorRenderer, "triggerActorRenderer is nullptr");
		//triggerActorRenderer->SetMesh(debugCubeMeshIndex);
		//triggerActorRenderer->SetMaterial(debugMaterialIndex);

		//auto triggerActorRenderer = triggerActor->AddComponent<sdk::MeshRenderer>(triggerBody, &litCube);
		//ASSERT(!triggerActorRenderer, "triggerActorRenderer is nullptr");

		//triggerActor->AddComponent<sdk::MeshRenderer>(triggerBody, &debugCube);
		triggerActor->SetGlobalPosition({ 10.0f, 5.0f, -5.0f });

		// === GUI ===

		sdk::Actor* guiActor = m_scene->CreateActor();
		ASSERT(!guiActor, "guiActor is nullptr");

		sdk::Actor* physicsInteraction = m_scene->CreateActor();
		ASSERT(!physicsInteraction, "physicsInteraction is nullptr");

		auto physicsBehaviour = physicsInteraction->AddComponent<sdk::Script>(nullptr, "assets/scripts/physics.lua");
		ASSERT(!physicsBehaviour, "physicsBehaviour is nullptr");

		//

		sdk::Actor* soundActor = m_scene->CreateActor();
		ASSERT(!soundActor, "soundActor is nullptr");

		auto soundActorBehaviour = soundActor->AddComponent<sdk::Script>(nullptr, "assets/scripts/sound.lua");
		ASSERT(!soundActorBehaviour, "soundActorBehaviour is nullptr");

		auto soundActorEmitter = soundActor->AddComponent<sdk::SoundEmitter>();
		ASSERT(!soundActorEmitter, "soundActorEmitter is nullptr");

		window.SetOnOutOfDateCallback(
			[&](uint32 width, uint32 height)
			{
				cameraComponent->renderArea.viewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f);
				cameraComponent->renderArea.scissor = vk::Rect2D({}, { window->GetWidth(), window->GetHeight() });
				cameraComponent->SetWidth(cameraComponent->renderArea.viewport.width);
				cameraComponent->SetHeight(cameraComponent->renderArea.viewport.height);

				m_scene->GetRenderSystem().Resize(window->GetWidth(), window->GetHeight());
			}
		);

		sdk::SceneManager::AddScene(m_scene.get(), "Main Scene");
		sdk::SceneManager::Start();

		std::function<void()> func = [this]()
		{
			std::cout << "Sound thread id: " << std::this_thread::get_id() << std::endl;
			while (!ShouldQuit())
			{
				if (!m_soundUpdated)
				{
					sdk::SceneManager::GetSoundSystem().Update(sdk::Time::DeltaTime());
					m_soundUpdated = true;
				}
				else
					std::this_thread::yield();
			}
			LOG_LOG("Exitting Sound thread")
		};
		//sdk::ThreadManager::Push(func);
		{
			std::thread thr(func);
			thr.detach();
		}

		func = [this]()
		{
			std::cout << "Script thread id: " << std::this_thread::get_id() << std::endl;
			while (!ShouldQuit())
			{
				if (!m_scriptsUpdated)
				{
					sdk::SceneManager::GetScriptSystem().Update(sdk::Time::DeltaTime());
					m_scriptsUpdated = true;
				}
				else
					std::this_thread::yield();
			}
			LOG_LOG("Exitting Script thread")
		};
		//sdk::ThreadManager::Push(func);
		{
			std::thread thr(func);
			thr.detach();
		}

		func = [this]()
		{
			std::cout << "Render thread id: " << std::this_thread::get_id() << std::endl;
			while (!ShouldQuit())
			{
				//if (!drawn)
				//{
				sdk::RenderSystem& renderSystem = sdk::SceneManager::GetRenderSystem();
				renderSystem.Update(sdk::Time::DeltaTime());

				Context::GetDevice().WaitGraphicsIdle();
				renderingResources = m_mainWindow->GetNewRenderingResources();

				//if (textMeshRenderer1 && textMeshRenderer1->IsActive())
				//{
				//	textMeshRenderer1->SetContent("frameTime: " + std::to_string(sdk::Time::DeltaTime()) + "\t       FPS: " + std::to_string(1.0f / sdk::Time::DeltaTime()));
				//}
				//if (textMeshRenderer2 && textMeshRenderer2->IsActive())
				//{
				//	textMeshRenderer2->SetContent("Avg frameTime: " + std::to_string(m_totalFrameTimePerSec / m_frameCountPerSec) + "\t   Avg FPS: " + std::to_string(m_frameCountPerSec / m_totalFrameTimePerSec));
				//}
				//if (textMeshRenderer3 && textMeshRenderer3->IsActive())
				//{
				//	textMeshRenderer3->SetContent("Frame count: " + std::to_string(m_frameCount));
				//}
				
				renderSystem.Draw(renderingResources.commandBuffer);
				m_mainWindow->Draw(renderSystem.GetPresentImage().GetImage());

				//sdk::SceneManager::WaitForRenderSystem();
				//renderSystem.Draw(m_renderingResources.commandBuffer);
				//window.Draw(renderSystem.GetPresentImage().GetImage());

				//Context::GetDevice().WaitGraphicsIdle();
				//drawn = true;
				//}
				//else
				//	std::this_thread::yield();
			}
			LOG_LOG("Exitting Render thread")
		};
		//sdk::ThreadManager::Push(func);
		{
			std::thread thr(func);
			thr.detach();
		}

		func = [this]()
		{
			std::cout << "Physics thread id: " << std::this_thread::get_id() << std::endl;
			while (!ShouldQuit())
			{
				//if (!physicsUpdated)
				//{
				//m_physicalSystem.Update(Time::DeltaTime());
				//physicsUpdated = true;
				//}
				//else
				//{
				static float elapsedTime = FLT_EPSILON;
				//LOG_LOG("elapsedTime: %lf", elapsedTime);

				float start = sdk::Time::GlobalTime();
				//std::this_thread::yield();
				sdk::PhysicalSystem& physicalSystem = sdk::SceneManager::GetPhysicalSystem();
				physicalSystem.Update(elapsedTime);

				//do
				//{
				//std::this_thread::yield();
				while (((elapsedTime = sdk::Time::GlobalTime()) - start < physicalSystem.fixedInterval) && !ShouldQuit())
					std::this_thread::yield();

				//}
			}
			LOG_LOG("Exitting Physics thread")
		};
		//sdk::ThreadManager::Push(func);
		{
			std::thread thr(func);
			thr.detach();
		}

		Context::LogVulkan = false;
	}

	double m_totalFrameTimePerSec{ 0.0 };
	uint64 m_frameCountPerSec{ 0 };
	uint64 m_frameCount{ 0 };

	void Update() override
	{
		if ((*m_mainWindow)->IsMinimized() == false)
		{
			m_frameCountPerSec++;
			m_frameCount++;

			while (!m_soundUpdated)
				std::this_thread::yield();
			while (!m_scriptsUpdated)
				std::this_thread::yield();

			WindowApplication::Update();

			m_soundUpdated = false;
			m_scriptsUpdated = false;

			float frameTime = sdk::Time::DeltaTime();
			
			m_totalFrameTimePerSec += frameTime;
			if (m_totalFrameTimePerSec >= 1000.0f)
			{
				m_totalFrameTimePerSec -= 1000.0f;
				m_frameCountPerSec = 1;
			}

			//script.Update();

			if (InputManager::GetKeyDown(KeyCode::Space) == true)
			{
				if (InputManager::GetKeyDown(KeyCode::R))
					m_scene->GetRenderSystem().lightScatteringUBO.exposure += 0.01f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::F))
					m_scene->GetRenderSystem().lightScatteringUBO.exposure -= 0.01f * frameTime;

				if (InputManager::GetKeyDown(KeyCode::T))
					m_scene->GetRenderSystem().lightScatteringUBO.decay += 0.01f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::G))
					m_scene->GetRenderSystem().lightScatteringUBO.decay -= 0.01f * frameTime;

				if (InputManager::GetKeyDown(KeyCode::Y))
					m_scene->GetRenderSystem().lightScatteringUBO.density += 0.5f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::H))
					m_scene->GetRenderSystem().lightScatteringUBO.density -= 0.5f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::U))
					m_scene->GetRenderSystem().lightScatteringUBO.weight += 0.5f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::J))
					m_scene->GetRenderSystem().lightScatteringUBO.weight -= 0.5f * frameTime;
			}

			//renderingResources = m_mainWindow->GetNewRenderingResources();

			//sdk::RenderSystem& renderSystem = sdk::SceneManager::GetRenderSystem();
			//renderSystem.Draw(renderingResources.commandBuffer);
			//m_mainWindow->Draw(renderSystem.GetPresentImage().GetImage());
		}
		else
			(*m_mainWindow)->Update();
	}

	void End() override
	{
		Context::LogVulkan = false;

		sound::End();
		WindowApplication::End();

		m_scene.reset();
	}
};

int main()
{
	try
	{
		sdk::EngineRRID engine;

		MainApplication app;
		app.Launch();
		//app.End();
	}
	catch (const std::function<void()>& catchFunc)
	{
		catchFunc();
		system("pause");

		return -1;
	}

	//system("pause");

	return 0;
}