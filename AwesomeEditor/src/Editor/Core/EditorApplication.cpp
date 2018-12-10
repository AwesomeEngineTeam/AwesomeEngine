#include <Editor/Core/EditorApplication.hpp>
#include <Awesome/SDK/Components/Render/MeshRenderer.hpp>
#include <Awesome/Maths/Ray.hpp>
#include <Awesome/Maths/Plane.hpp>

using namespace aw;

namespace ed
{
	EditorApplication::EditorApplication(int argc, char** argv) :
		QApplication(argc, argv),
		m_splash{ QPixmap("assetsEditor/icons/AwesomeLogo.png") },
		m_scene{ sdk::SceneManager::CurrentScene }
	{
		setApplicationName("Awesome Engine");
		QLocale::setDefault(QLocale("us"));

		m_splash.show();

		QFile File("assetsEditor/style/AwesomeQStyle.qss");
		File.open(QFile::ReadOnly);
		QString StyleSheet = QLatin1String(File.readAll());

		setStyleSheet(StyleSheet);
	}

	void EditorApplication::Play()
	{
		if (m_isPlaying)
			return;

		m_editWin->SetOnGame(true);
		sdk::Time::Reset();

		try
		{
			//for(int i = 0; i<250; ++i)
			//{
			//	sdk::Actor* errorText = sdk::SceneManager::CurrentScene->CreateActor(nullptr, "ErrorText");
			//	sdk::Actor* child = sdk::SceneManager::CurrentScene->CreateActor(errorText, "Child");
			//	sdk::Actor* subChild = sdk::SceneManager::CurrentScene->CreateActor(child, "SubChild");

			//	auto text = errorText->AddComponent<sdk::Text2D>();
			//	text->SetGlobalPosition({ 5.0f, m_editWin->GetGameWindow()->GetHeight() / 2.f, 0.0f });
			//	text->SetContent("Please fix all errors before entering play mode");

			//	auto soundListener = subChild->AddComponent<sdk::SoundListener>();
			//	auto script = subChild->AddComponent<sdk::Script>(soundListener, "assets/scripts/dummy.lua");
			//	auto rb = subChild->AddComponent<sdk::Rigidbody>(script, ph::CapsuleGeometry(), ph::Material(0.5f, 0.5f, 1.0f));

			//	std::thread thr([subChild, errorText, text, script, rb, soundListener]()
			//	{
			//		using namespace std::chrono_literals;
			//		std::this_thread::sleep_for(3s);

			//		sdk::SceneManager::CurrentScene->RemoveActor(errorText);
			//		//errorText->RemoveComponent(text);
			//		//subChild->RemoveComponent(script);
			//		//subChild->RemoveComponent(rb);
			//		//subChild->RemoveComponent(soundListener);
			//	});

			//	thr.detach();
			//}

			sdk::ScriptSystem::OnSceneStart();
			m_isPlaying = true;
		}
		catch(...)
		{
			//TODO catch errors not thrown
			LOG_ERROR("Please fix all errors before entering play mode");
			m_editWin->SetOnGame(false);

			sdk::Actor* errorText = sdk::SceneManager::CurrentScene->CreateActor(nullptr, "ErrorText");

			auto text = errorText->AddComponent<sdk::Text2D>();
			text->SetGlobalPosition({ 5.0f, m_editWin->GetGameWindow()->GetHeight() / 2.f, 0.0f });
			text->SetContent("Please fix all errors before entering play mode");

			auto soundListener = errorText->AddComponent<sdk::SoundListener>(text);
			auto script = errorText->AddComponent<sdk::Script>(soundListener, "assets/scripts/dummy.lua");
			auto rb = errorText->AddComponent<sdk::Rigidbody>(script, ph::CapsuleGeometry(), ph::Material(0.5f, 0.5f, 1.0f));

			std::thread thr([errorText]()
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(3s);

				sdk::SceneManager::CurrentScene->RemoveActor(errorText);
			});

			thr.detach();
		}
	}

	void EditorApplication::Stop()
	{
		if (!m_isPlaying)
			return;

		sound::End();

		m_editWin->SetOnGame(false);

		sdk::Time::SetTimeScale(0);
		m_isPlaying = false;
	}

	void EditorApplication::QuitApp()
	{
		Quit();
	}

	sdk::Actor* EditorApplication::InstantiateActor(const std::string& name, sdk::Actor* parent)
	{
		return sdk::SceneManager::CurrentScene->CreateActor(parent, name);
	}

	sdk::Actor* EditorApplication::InstantiateQuad(sdk::Actor* parent)
	{
		sdk::Actor* act = sdk::SceneManager::CurrentScene->CreateActor(parent, "Quad");
		auto comp = act->AddComponent<sdk::MeshRenderer>();
		comp->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Quad));

		return act;
	}

	sdk::Actor* EditorApplication::InstantiateCube(sdk::Actor* parent)
	{
		sdk::Actor* act = sdk::SceneManager::CurrentScene->CreateActor(parent, "Cube");
		auto comp = act->AddComponent<sdk::MeshRenderer>();
		comp->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));

		return act;
	}

	sdk::Actor* EditorApplication::InstantiateSphere(sdk::Actor* parent)
	{
		sdk::Actor* act = sdk::SceneManager::CurrentScene->CreateActor(parent, "Sphere");
		auto comp = act->AddComponent<sdk::MeshRenderer>();
		comp->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Sphere));

		return act;
	}

	sdk::Actor* EditorApplication::InstantiateCapsule(sdk::Actor* parent)
	{
		sdk::Actor* act = sdk::SceneManager::CurrentScene->CreateActor(parent, "Capsule");
		auto comp = act->AddComponent<sdk::MeshRenderer>();
		comp->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Capsule));

		return act;
	}

	sdk::Actor* EditorApplication::InstantiateCylinder(sdk::Actor* parent)
	{
		sdk::Actor* act = sdk::SceneManager::CurrentScene->CreateActor(parent, "Cylinder");
		auto comp = act->AddComponent<sdk::MeshRenderer>();
		comp->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cylinder));

		return act;
	}

	sdk::Actor* EditorApplication::InstantiateCone(sdk::Actor* parent)
	{
		sdk::Actor* act = sdk::SceneManager::CurrentScene->CreateActor(parent, "Cone");
		auto comp = act->AddComponent<sdk::MeshRenderer>();
		comp->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cone));

		return act;
	}

	void EditorApplication::RenameActor(aw::sdk::Actor* actor, const std::string& name)
	{
		actor->name = name;
		m_editWin->RenameActor(actor, name);
	}

	void EditorApplication::RenameComponent(aw::sdk::Component* comp, const std::string& name)
	{
		comp->name = name;
		m_editWin->RenameComponent(comp, name);
	}

	void EditorApplication::Destroy(sdk::Actor* actor)
	{
		if (actor)
		{
			if (m_selectedComponent && m_selectedComponent->GetActor() == actor)
				m_selectedComponent = nullptr;
			sdk::SceneManager::CurrentScene->RemoveActor(actor);
		}
	}

	void EditorApplication::Destroy(sdk::Actor* actor, sdk::Component* comp)
	{
		if (m_selectedComponent == comp)
			m_selectedComponent = nullptr;
		if (actor && comp)
			actor->RemoveComponent(comp);
	}

	aw::sdk::Actor* EditorApplication::Import(const std::string& path, aw::sdk::Actor* parent)
	{
		std::string name;

		int32 index1 = path.rfind('/') + 1; // +1 to remove /
		int32 index2 = path.rfind('.');

		if(index1 > 0  && index1 < index2)
			name = path.substr(index1, index2 - index1);

		sdk::Actor* actor = m_scene->CreateActor(parent, name);

		if(path.substr(index2) == ".gltf")
			sdk::GLTFImporter::Import(path, *m_scene, actor);
		else if (path.substr(index2) == ".obj")
			/*Todo: Import OBJ*/;

		return actor;
	}

	void EditorApplication::NewScene()
	{
		QInputDialog dialog;
		dialog.setLabelText("Enter a name:");

		if (!dialog.exec())
			return;

		sdk::SceneManager::CreateScene(dialog.textValue().toStdString());
	}

	void EditorApplication::SaveScene()
	{
		sdk::SceneManager::SaveScene();
	}

	double m_totalFrameTimePerSec{ 0.0 };
	uint64 m_frameCountPerSec{ 0 };
	uint64 m_frameCount{ 0 };

	void EditorApplication::Start()
	{
		BaseApplication::Start();

		sdk::Time::Pause();

		m_editWin = new Window;
		m_editWin->SetOnInspectComponentCallback([this](sdk::Component* selectedComponent){
			m_selectedComponent = selectedComponent;
		});
		m_editWin->SetOnClearInspectorCallback([this](){
			m_selectedComponent = nullptr;
		});

		sdk::SceneManager::CreateScene("Main Scene");

		sdk::PhysicalSystem::Init();
		sdk::RenderSystem::Init();

		//sdk::Actor* actor2 = m_scene->CreateActor(nullptr, "GLTF Helmet");
		//ASSERT(!actor2, "actor2 is nullptr");

		//sdk::Actor* actor3 = m_scene->CreateActor(nullptr, "Rough Spheres");
		//ASSERT(!actor3, "actor3 is nullptr");

		//sdk::GLTFImporter::Import("assets/glTF/DamagedHelmet/DamagedHelmet.gltf", *m_scene, actor2);
		//sdk::GLTFImporter::Import("assets/glTF/MetalRoughSpheres/MetalRoughSpheres.gltf", *m_scene, actor3);

		//actor2->Rotate_WorldSpace(Quaternion(0.7, 0, 0.7, 0));
		//actor3->Translate_WorldSpace(Vector3(0, 0, 10));

		auto& window = m_editWin->GetGameWindow();

		if (!sound::LoadBank("Demo/Init.bnk"))
			THROW("Failed to load init bank");

		if (!sound::LoadBank("Demo/Demo.bnk"))
			THROW("Failed to load Demo soundbank");

		sdk::ResourceManager& resourceManager = sdk::RenderSystem::GetResourceManager();

		// SampledTextureHandle diffuseTexture;
		// SampledTextureHandle normalTexture;
		// SampledTextureHandle specularTexture;
		// SampledTextureHandle heightTexture;
		// SampledTextureHandle skyboxTexture;
		// SampledTextureHandle whiteTexture;
		// SampledTextureHandle blueTexture;
		// SampledTextureHandle redTexture;
		uint32 diffuseTextureIndex;
		uint32 normalTextureIndex;
		uint32 specularTextureIndex;
		uint32 heightTextureIndex;
		uint32 yokohamaSkyboxTextureIndex;
		uint32 midnightSkyboxTextureIndex;
		uint32 skySkyboxTextureIndex;
		uint32 spiresSkyboxTextureIndex;
		uint32 sorcwdSkyboxTextureIndex;
		uint32 areskutanSkyboxTextureIndex;
		uint32 whiteTextureIndex;
		uint32 blueTextureIndex;
		uint32 redTextureIndex;
		{
			//TexelBuffer* texelBuffer{ nullptr };

			//texelBuffer = resourceManager.Load<sdk::TextureLoader>("assets/textures/debug_texture.png");
			//texelBuffer = resourceManager.Load<sdk::TextureLoader>("assets/textures/piso_patio_exterior.png");
			TexelBuffer texelBuffer = sdk::TextureLoader::Load("assets/textures/piso_patio_exterior.png");

			SampledTexture diffuseTextureUnique;
			diffuseTextureUnique.Create(
				vk::ImageType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				std::move(texelBuffer),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			SampledTextureHandle diffuseTexture = resourceManager.Register("diffuseTexture", std::move(diffuseTextureUnique));
			diffuseTextureIndex = sdk::RenderSystem::RegisterTexture(diffuseTexture);

			texelBuffer = sdk::TextureLoader::Load("assets/textures/N_piso_patio_exterior_displace.png");
			SampledTexture normalTextureUnique;
			normalTextureUnique.Create(
				vk::ImageType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				std::move(texelBuffer),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			SampledTextureHandle normalTexture = resourceManager.Register("normalTexture", std::move(normalTextureUnique));
			normalTextureIndex = sdk::RenderSystem::RegisterTexture(normalTexture);

			texelBuffer = sdk::TextureLoader::Load("assets/textures/piso_patio_exterior_displace_inv2.png");
			SampledTexture specularTextureUnique;
			specularTextureUnique.Create(
				vk::ImageType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				std::move(texelBuffer),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			SampledTextureHandle specularTexture = resourceManager.Register("specularTexture", std::move(specularTextureUnique));
			specularTextureIndex = sdk::RenderSystem::RegisterTexture(specularTexture);

			texelBuffer = sdk::TextureLoader::Load("assets/textures/piso_patio_exterior_displace_inv.png");
			SampledTexture heightTextureUnique;
			heightTextureUnique.Create(
				vk::ImageType::e2D,
				vk::Format::eR8G8B8A8Unorm,
				std::move(texelBuffer),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			SampledTextureHandle heightTexture = resourceManager.Register("heightTexture", std::move(heightTextureUnique));
			heightTextureIndex = sdk::RenderSystem::RegisterTexture(heightTexture);

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
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				SampledTextureHandle whiteTexture = resourceManager.Register("whiteTexture", std::move(whiteTextureUnique));
				whiteTextureIndex = sdk::RenderSystem::RegisterTexture(whiteTexture);

				texelBuffer.texels = { 255, 0, 0, 255 };
				SampledTexture redTextureUnique;
				redTextureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				SampledTextureHandle redTexture = resourceManager.Register("redTexture", std::move(redTextureUnique));
				redTextureIndex = sdk::RenderSystem::RegisterTexture(redTexture);

				texelBuffer.texels = { 0, 0, 255, 255 };
				SampledTexture blueTextureUnique;
				blueTextureUnique.Create(
					vk::ImageType::e2D,
					vk::Format::eR8G8B8A8Unorm,
					texelBuffer,
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				SampledTextureHandle blueTexture = resourceManager.Register("blueTexture", std::move(blueTextureUnique));
				blueTextureIndex = sdk::RenderSystem::RegisterTexture(blueTexture);
			}

			{
				TexelBuffer yokohamaSkybox = sdk::TextureLoader::LoadSkybox("assets/skybox/Yokohama3");
				SampledTexture skyboxTextureUnique;
				skyboxTextureUnique.Create(
					vk::ImageType::e3D,
					vk::Format::eR8G8B8A8Unorm,
					std::move(yokohamaSkybox),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				SampledTextureHandle skyboxTexture = resourceManager.Register("skyboxTexture", std::move(skyboxTextureUnique));
				yokohamaSkyboxTextureIndex = sdk::RenderSystem::RegisterTexture(skyboxTexture);
			}

			{
				TexelBuffer skybox = sdk::TextureLoader::LoadSkybox("assets/skybox/Spires", ".tga");
				SampledTexture skyboxTextureUnique;
				skyboxTextureUnique.Create(
					vk::ImageType::e3D,
					vk::Format::eR8G8B8A8Unorm,
					std::move(skybox),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				SampledTextureHandle skyboxTexture = resourceManager.Register("spiresSkyboxTexture", std::move(skyboxTextureUnique));
				spiresSkyboxTextureIndex = sdk::RenderSystem::RegisterTexture(skyboxTexture);
			}

			{
				TexelBuffer skybox = sdk::TextureLoader::LoadSkybox("assets/skybox/mp_midnight", ".tga");
				SampledTexture skyboxTextureUnique;
				skyboxTextureUnique.Create(
					vk::ImageType::e3D,
					vk::Format::eR8G8B8A8Unorm,
					std::move(skybox),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				SampledTextureHandle skyboxTexture = resourceManager.Register("midnightSkyboxTexture", std::move(skyboxTextureUnique));
				midnightSkyboxTextureIndex = sdk::RenderSystem::RegisterTexture(skyboxTexture);
			}

			{
				TexelBuffer skybox = sdk::TextureLoader::LoadSkybox("assets/skybox/sor_cwd");
				SampledTexture skyboxTextureUnique;
				skyboxTextureUnique.Create(
					vk::ImageType::e3D,
					vk::Format::eR8G8B8A8Unorm,
					std::move(skybox),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				SampledTextureHandle skyboxTexture = resourceManager.Register("sorcwdSkyboxTexture", std::move(skyboxTextureUnique));
				sorcwdSkyboxTextureIndex = sdk::RenderSystem::RegisterTexture(skyboxTexture);
			}

			{
				TexelBuffer skybox = sdk::TextureLoader::LoadSkybox("assets/skybox/Areskutan");
				SampledTexture skyboxTextureUnique;
				skyboxTextureUnique.Create(
					vk::ImageType::e3D,
					vk::Format::eR8G8B8A8Unorm,
					std::move(skybox),
					vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
					vk::ImageAspectFlagBits::eColor
				);
				SampledTextureHandle skyboxTexture = resourceManager.Register("areskutanSkyboxTexture", std::move(skyboxTextureUnique));
				areskutanSkyboxTextureIndex = sdk::RenderSystem::RegisterTexture(skyboxTexture);
			}

			/*// TexelBuffer yokohamaSkybox = sdk::TextureLoader::LoadSkybox("assets/skybox/Yokohama3");
			TexelBuffer yokohamaSkybox = sdk::TextureLoader::LoadSkybox("assets/skybox/Spires", ".tga");
			// TexelBuffer yokohamaSkybox = sdk::TextureLoader::LoadSkybox("assets/skybox/mp_midnight", ".tga");
			// TexelBuffer yokohamaSkybox = sdk::TextureLoader::LoadSkybox("assets/skybox/sor_cwd");
			// TexelBuffer yokohamaSkybox = sdk::TextureLoader::LoadSkybox("assets/skybox/Areskutan");
			SampledTexture skyboxTextureUnique;
			skyboxTextureUnique.Create(
				vk::ImageType::e3D,
				vk::Format::eR8G8B8A8Unorm,
				std::move(yokohamaSkybox),
				vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
				vk::ImageAspectFlagBits::eColor
			);
			SampledTextureHandle skyboxTexture = resourceManager.Register("spiresSkyboxTexture", std::move(skyboxTextureUnique));
			spiresSkyboxTextureIndex = sdk::RenderSystem::RegisterTexture(skyboxTexture);*/
		}

		sdk::RenderSystem::Resize(window->GetWidth(), window->GetHeight());

		uint32 litMaterialIndex;
		{
			aw::Material litMaterial;
			litMaterial.SetDiffuse(diffuseTextureIndex);
			litMaterial.SetNormal(normalTextureIndex);
			litMaterial.SetSpecular(specularTextureIndex);
			litMaterial.SetHeight(heightTextureIndex);
			litMaterial.materialInfo.receiveLights = true;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(litMaterial, sb::ShaderBuilder::MakeShaderProgram);
			litMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(litMaterial));
		}

		//{
		//	Material debugMaterial;
		//	debugMaterial.materialPipelineInfo.SetDepthTest(false);
		//	debugMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eLine);
		//	debugMaterial.materialPipelineInfo.SetPrimitiveTopology(vk::PrimitiveTopology::eLineList);
		//	debugMaterial.materialInfo.receiveLights = false;
		//	debugMaterial.materialInfo.isTransparent = true;
		//	sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(debugMaterial, sb::ShaderBuilder::MakeShaderProgram);
		//	m_debugMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(debugMaterial));
		//}

		uint32 unlitMaterialIndex;
		{
			aw::Material unlitMaterial;
			unlitMaterial.SetDiffuse(whiteTextureIndex);
			unlitMaterial.SetSpecular(whiteTextureIndex);
			unlitMaterial.materialInfo.receiveLights = false;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(unlitMaterial, sb::ShaderBuilder::MakeShaderProgram);
			unlitMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(unlitMaterial));
		}

		uint32 unlitMaterialBlueIndex;
		{
			aw::Material unlitMaterialBlue;
			unlitMaterialBlue.SetDiffuse(whiteTextureIndex);
			unlitMaterialBlue.SetSpecular(blueTextureIndex);
			unlitMaterialBlue.materialInfo.receiveLights = false;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(unlitMaterialBlue, sb::ShaderBuilder::MakeShaderProgram);
			unlitMaterialBlueIndex = sdk::RenderSystem::RegisterMaterial(std::move(unlitMaterialBlue));
		}

		uint32 unlitMaterialRedIndex;
		{
			aw::Material unlitMaterialRed;
			unlitMaterialRed.SetDiffuse(whiteTextureIndex);
			unlitMaterialRed.SetSpecular(redTextureIndex);
			unlitMaterialRed.materialInfo.receiveLights = false;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(unlitMaterialRed, sb::ShaderBuilder::MakeShaderProgram);
			unlitMaterialRedIndex = sdk::RenderSystem::RegisterMaterial(std::move(unlitMaterialRed));
		}

		uint32 awesomeMaterialIndex;
		{
			aw::Material awesomeMaterial;
			awesomeMaterial.SetDiffuse(diffuseTextureIndex);
			awesomeMaterial.SetSpecular(specularTextureIndex);
			awesomeMaterial.SetNormal(normalTextureIndex);
			awesomeMaterial.SetHeight(heightTextureIndex);
			//awesomeMaterial.SetReflection(whiteTexture);
			awesomeMaterial.SetRefraction(specularTextureIndex);
			awesomeMaterial.SetEnvironment(yokohamaSkyboxTextureIndex);
			awesomeMaterial.materialInfo.receiveLights = true;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(awesomeMaterial, sb::ShaderBuilder::MakeShaderProgram);
			awesomeMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(awesomeMaterial));
		}

		uint32 reflectMaterialIndex;
		{
			aw::Material reflectMaterial;
			//      reflectMaterial.SetDiffuse(whiteTexture);
			//      reflectMaterial.SetSpecular(whiteTexture);
			reflectMaterial.SetReflection(whiteTextureIndex);
			reflectMaterial.SetEnvironment(yokohamaSkyboxTextureIndex);
			reflectMaterial.materialInfo.receiveLights = false;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(reflectMaterial, sb::ShaderBuilder::MakeShaderProgram);
			reflectMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(reflectMaterial));
		}

		uint32 yokohamaRefractMaterialIndex;
		{
			aw::Material refractMaterial;
			refractMaterial.SetRefraction(whiteTextureIndex);
			refractMaterial.SetEnvironment(yokohamaSkyboxTextureIndex);
			refractMaterial.materialInfo.receiveLights = false;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(refractMaterial, sb::ShaderBuilder::MakeShaderProgram);
			yokohamaRefractMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(refractMaterial));
		}

		uint32 spiresRefractMaterialIndex;
		{
			aw::Material refractMaterial;
			refractMaterial.SetRefraction(whiteTextureIndex);
			refractMaterial.SetEnvironment(spiresSkyboxTextureIndex);
			refractMaterial.materialInfo.receiveLights = false;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(refractMaterial, sb::ShaderBuilder::MakeShaderProgram);
			spiresRefractMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(refractMaterial));
		}

		uint32 midnightRefractMaterialIndex;
		{
			aw::Material refractMaterial;
			refractMaterial.SetRefraction(whiteTextureIndex);
			refractMaterial.SetEnvironment(midnightSkyboxTextureIndex);
			refractMaterial.materialInfo.receiveLights = false;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(refractMaterial, sb::ShaderBuilder::MakeShaderProgram);
			midnightRefractMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(refractMaterial));
		}

		uint32 sorcwdRefractMaterialIndex;
		{
			aw::Material refractMaterial;
			refractMaterial.SetRefraction(whiteTextureIndex);
			refractMaterial.SetEnvironment(sorcwdSkyboxTextureIndex);
			refractMaterial.materialInfo.receiveLights = false;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(refractMaterial, sb::ShaderBuilder::MakeShaderProgram);
			sorcwdRefractMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(refractMaterial));
		}

		uint32 areskutanRefractMaterialIndex;
		{
			aw::Material refractMaterial;
			refractMaterial.SetRefraction(whiteTextureIndex);
			refractMaterial.SetEnvironment(areskutanSkyboxTextureIndex);
			refractMaterial.materialInfo.receiveLights = false;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(refractMaterial, sb::ShaderBuilder::MakeShaderProgram);
			areskutanRefractMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(refractMaterial));
		}

		uint32 skyboxMaterialIndex;
		{
			aw::Material skyboxMaterial;
			skyboxMaterial.materialInfo.isSkybox = true;
			skyboxMaterial.materialPipelineInfo.SetDepthCompareOp(vk::CompareOp::eLessOrEqual);
			skyboxMaterial.materialPipelineInfo.SetDepthWriteEnabled(false);
			skyboxMaterial.SetDiffuse(yokohamaSkyboxTextureIndex);
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(skyboxMaterial, sb::ShaderBuilder::MakeSkyboxShaderProgram);
			skyboxMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(skyboxMaterial));
		}

		uint32 rimMaterialIndex;
		{
			aw::Material rimMaterial;
			sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(rimMaterial, sb::ShaderBuilder::MakeRimShaderProgram);
			rimMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(rimMaterial));
		}

		//uint32 wireframeMaterialIndex;
		//{
		//	Material wireframeMaterial;
		//	wireframeMaterial.SetDiffuse(whiteTexture);
		//	wireframeMaterial.SetSpecular(whiteTexture);
		//	wireframeMaterial.materialInfo.receiveLights = false;
		//	wireframeMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
		//	wireframeMaterial.materialPipelineInfo.SetPolygonMode(vk::PolygonMode::eLine);
		//	wireframeMaterial.materialPipelineInfo.SetDepthWriteEnabled(false);
		//	sdk::RenderSystem::GetResourceManager().materialManager.CreateMaterial(wireframeMaterial, sb::ShaderBuilder::MakeShaderProgram);
		//	wireframeMaterialIndex = sdk::RenderSystem::RegisterMaterial(std::move(wireframeMaterial));
		//}

		//{
		//	Material lightGizmoMaterial;
		//	lightGizmoMaterial.SetDiffuse(lightGizmoTexture);
		//	lightGizmoMaterial.materialInfo.receiveLights = false;
		//	lightGizmoMaterial.materialInfo.isTransparent = true;
		//	lightGizmoMaterial.materialPipelineInfo.SetCullMode(vk::CullModeFlagBits::eNone);
		//	lightGizmoMaterial.materialPipelineInfo.SetDepthWriteEnabled(false);
		//	lightGizmoMaterial.materialPipelineInfo.SetDepthTest(false);
		//	m_scene->GetRenderSystem().GetResourceManager().materialManager.CreateMaterial(lightGizmoMaterial, sb::ShaderBuilder::MakeShaderProgram);
		//	m_lightGizmoMaterialIndex = m_scene->GetRenderSystem().RegisterMaterial(std::move(lightGizmoMaterial));
		//}

		// === Level ===

		auto skyboxActor = m_scene->CreateActor(nullptr, "SkyboxActor");
		ASSERT(!skyboxActor, "skyboxActor is nullptr");

		auto skyboxComponent = skyboxActor->AddComponent<sdk::Skybox>();
		ASSERT(!skyboxComponent, "skyboxComponent is nullptr");
		skyboxComponent->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Skybox));
		skyboxComponent->SetMaterial(skyboxMaterialIndex);

		auto light = skyboxActor->AddComponent<sdk::DirectionalLight>(skyboxComponent);
		light->SetLocalRotation(Quaternion(M_PI, Vector3(0.0f, 1.0f, 0.0f)));
		light->color = Color3f(100, 100, 100);

		// === Player ===

		sdk::Actor* player = m_scene->CreateActor(nullptr, "Player");
		ASSERT(!player, "player is nullptr");

		auto playerBody = player->AddComponent<sdk::Rigidbody>(nullptr, ph::CapsuleGeometry(0.75f, 0.75f), ph::Material(10.0f, 10.0f, 0.0f), 80.0f);
		ASSERT(!playerBody, "playerBody is nullptr");
		playerBody->name = "PlayerBody";
		playerBody->lockAxisMask = static_cast<uint8>(ph::LockAxis::LockAllAngular) | static_cast<uint8>(aw::ph::LockAxis::LockLinearY);

		playerBody->SetGlobalPosition(Vector3(0.0f, 2.0f, 165.0f));
		playerBody->collisionDetectionMode = ph::CollisionDetection::Continuous;

		m_cameraComponent = player->AddComponent<sdk::Camera>();
		ASSERT(!m_cameraComponent, "m_cameraComponent is nullptr");
		m_cameraComponent->name = "MainCamera";
		m_cameraComponent->SetLocalPosition(Vector3(0.0f, 1.2f, 0.0f));
		{
			m_cameraComponent->renderArea.viewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f);
			m_cameraComponent->renderArea.scissor = vk::Rect2D({}, { window->GetWidth(), window->GetHeight() });
			m_cameraComponent->SetWidth(m_cameraComponent->renderArea.viewport.width);
			m_cameraComponent->SetHeight(m_cameraComponent->renderArea.viewport.height);
			m_cameraComponent->SetNear(0.001f);
		}

		auto playerListener = player->AddComponent<sdk::SoundListener>(m_cameraComponent);

		auto playerBehaviour = player->AddComponent<sdk::Script>(nullptr, "assets/scripts/player.lua");
		ASSERT(!playerBehaviour, "playerBehaviour is nullptr");

		playerBehaviour->AddOrSetBinding("reach", 15);
		playerBehaviour->AddOrSetBinding("force", 300.f);
		playerBehaviour->AddOrSetBinding("mouseSpeed", 1);
		playerBehaviour->AddOrSetBinding("maxSpeed", 15.0f);
		playerBehaviour->AddOrSetBinding("sprintSpeed", 2.5f);
		playerBehaviour->AddOrSetBinding("velocityDrag", 5.0f);
		playerBehaviour->AddOrSetBinding("cameraRotationSpeed", 0.9f);
		playerBehaviour->AddOrSetBinding("camera", static_cast<sdk::Component*>(m_cameraComponent));

		// === Root level actor ===

		sdk::Actor* level = m_scene->CreateActor(nullptr, "Level");
		auto levelComp = level->AddComponent<sdk::Component>();
		levelComp->name = "Level";

		// === Landscape ===

		// Enter Corridor

		sdk::Actor* landscape = m_scene->CreateActor(level, "Landscape");

		auto landscapeCollider = landscape->AddComponent<sdk::PlaneCollider>();
		landscapeCollider->SetGlobalPosition(Vector3(0.0f, -1.0f, 65.0f));

		auto roofCollider = landscape->AddComponent<sdk::PlaneCollider>();
		roofCollider->SetGlobalPosition(Vector3(0.0f, 11.0f, 0.0f));
		roofCollider->SetGlobalRotation(Quaternion(0.0f, 1.0f, 0.0f, 0.0f));

		// Corridors

		const int size1 = 2;
		const Vector3 pos1[size1]{ Vector3(-5.5f, 5.5f, 0.0f), Vector3(5.5f, 5.5f, 0.0f) };
		const Vector3 groundPos1[4]{ Vector3(0.0f, 0.0f, 95.0f), Vector3(0.0f, 11.0f, 0.0f),
			Vector3(0.0f, 0.0f, 75.0f), Vector3(0.0f, 11.0f, 0.0f) };

		for (int k = 0; k < 3; k += 2)
		{
			for (int j = -1; j < 2; j += 2)
			{
				sdk::MeshRenderer* parent = nullptr;

				for (int i = 0; i < 2; i++)
				{
					auto ground = landscape->AddComponent<sdk::MeshRenderer>(parent);
					ground->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Quad));
					ground->meshTransform.scale = Vector3(1.0f, 1.0f, 2.0f);
					ground->SetMaterial(litMaterialIndex);
					ground->name = "Corridor";

					Vector3 position = groundPos1[i + k];
					position.z = position.z * j;

					ground->SetLocalPosition(position);

					if (!parent)
						parent = ground;
				}

				for (int i = 0; i < size1; i++)
				{
					auto wall = landscape->AddComponent<sdk::MeshRenderer>(parent);
					wall->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
					wall->meshTransform.scale = Vector3(1.0f, 11.0f, 20.0f);
					wall->SetMaterial(litMaterialIndex);
					wall->name = "CorridorWall";

					Vector3 position = pos1[i];
					position.x = position.x * j;

					wall->SetLocalPosition(position);
					auto wallCollider = landscape->AddComponent<sdk::BoxCollider>(wall, Vector3(1.5f, 5.5f, 10.0f));
				}
			}
		}

		int num = 1;
		char c;
		for (int j = 1; j >= -1; j -= 2)
		{
			itoa(num++, &c, 10);

			auto corridorBackWall = landscape->AddComponent<sdk::MeshRenderer>();
			corridorBackWall->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			corridorBackWall->meshTransform.scale = Vector3(12.0f, 11.0f, 1.0f);
			// corridorBackWall->SetMaterial(litMaterialIndex);
			corridorBackWall->SetMaterial(unlitMaterialIndex);
			corridorBackWall->name = std::string("CorridorBackWall") + c;
			corridorBackWall->SetLocalPosition(Vector3(0.0f, 5.5f, j * 105.0f));
			corridorBackWall->isLightScatteringEmitter = true;

			if(j == 1)
				auto corridor1BackWallCollider = landscape->AddComponent<sdk::BoxCollider>(corridorBackWall, Vector3(6.0f, 5.5f, 0.5f));
			else
			{
				auto corridor1BackWallCollider = landscape->AddComponent<sdk::BoxCollider>(corridorBackWall, Vector3(2.0f, 3.6f, 0.4f));
				corridor1BackWallCollider->SetLocalPosition(Vector3(-4.01957f, -1.92089f, 0.f));

				corridor1BackWallCollider = landscape->AddComponent<sdk::BoxCollider>(corridorBackWall, Vector3(2.0f, 3.6f, 0.4f));
				corridor1BackWallCollider->SetLocalPosition(Vector3(4.04296, -1.92089f, 0.f));
			}
		}

		// MainRoom

		auto groundMainRoom = landscape->AddComponent<sdk::MeshRenderer>();
		groundMainRoom->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Quad));
		groundMainRoom->meshTransform.scale = Vector3(10.0f, 1.0f, 13.0f);
		groundMainRoom->scaleUV = { 11.0f, 11.0f };
		groundMainRoom->SetMaterial(litMaterialIndex);
		groundMainRoom->name = "MainRoom";
		groundMainRoom->SetLocalPosition(Vector3(0.0f, 0.0f, 0.0f));

		auto roofMainRoom = landscape->AddComponent<sdk::MeshRenderer>();
		roofMainRoom->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Quad));
		roofMainRoom->meshTransform.scale = Vector3(10.0f, 1.0f, 12.9f);
		roofMainRoom->scaleUV = { 11.0f, 11.0f };
		roofMainRoom->SetMaterial(litMaterialIndex);
		roofMainRoom->name = "MainRoomRoof";
		roofMainRoom->SetLocalPosition(Vector3(0.0f, 11.0f, 0.0f));
		roofMainRoom->SetLocalRotation(Quaternion(M_PI, Vector3(1.0f, 0.0f, 0.0f)));

		auto refractObjectParent = landscape->AddComponent<sdk::Component>();
		ASSERT(!refractObjectParent, "refractObject Parent is nullptr");
		refractObjectParent->SetLocalPosition({ -6.0f, 4.85f, 4.0f });
		refractObjectParent->SetScale(1.3f);
		refractObjectParent->name = "Refract object parent";

		auto refractObjectCol = landscape->AddComponent<sdk::BoxCollider>(refractObjectParent);
		ASSERT(!refractObjectCol, "refractObject collider 1 is nullptr");
		refractObjectCol->halfScale = Vector3(3.0f, 3.0f, 3.0f);
		refractObjectCol->name = "RefractObject";

		// refract object frame
		{
			auto refractObjectFrame = landscape->AddComponent<sdk::Component>(refractObjectParent);
			ASSERT(!refractObjectFrame, "refractObject frame is nullptr");

			auto refractObjectFrame1 = landscape->AddComponent<sdk::MeshRenderer>(refractObjectFrame);
			ASSERT(!refractObjectFrame1, "refractObject frame 1 is nullptr");
			refractObjectFrame1->SetLocalPosition({ 3.0f, 0.0f, 3.0f });
			refractObjectFrame1->meshTransform.scale = Vector3(0.5f, 6.0f, 0.5f);
			refractObjectFrame1->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObjectFrame1->SetMaterial(rimMaterialIndex);
			refractObjectFrame1->name = "Refract frame (1)";

			auto refractObjectFrame2 = landscape->AddComponent<sdk::MeshRenderer>(refractObjectFrame);
			ASSERT(!refractObjectFrame2, "refractObject frame 2 is nullptr");
			refractObjectFrame2->SetLocalPosition({ -3.0f, 0.0f, 3.0f });
			refractObjectFrame2->meshTransform.scale = Vector3(0.5f, 6.0f, 0.5f);
			refractObjectFrame2->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObjectFrame2->SetMaterial(rimMaterialIndex);
			refractObjectFrame2->name = "Refract frame (2)";

			auto refractObjectFrame3 = landscape->AddComponent<sdk::MeshRenderer>(refractObjectFrame);
			ASSERT(!refractObjectFrame3, "refractObject frame 3 is nullptr");
			refractObjectFrame3->SetLocalPosition({ 3.0f, 0.0f, -3.0f });
			refractObjectFrame3->meshTransform.scale = Vector3(0.5f, 6.0f, 0.5f);
			refractObjectFrame3->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObjectFrame3->SetMaterial(rimMaterialIndex);
			refractObjectFrame3->name = "Refract frame (3)";

			auto refractObjectFrame4 = landscape->AddComponent<sdk::MeshRenderer>(refractObjectFrame);
			ASSERT(!refractObjectFrame4, "refractObject frame 4 is nullptr");
			refractObjectFrame4->SetLocalPosition({ -3.0f, 0.0f, -3.0f });
			refractObjectFrame4->meshTransform.scale = Vector3(0.5f, 6.0f, 0.5f);
			refractObjectFrame4->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObjectFrame4->SetMaterial(rimMaterialIndex);
			refractObjectFrame4->name = "Refract frame (4)";

			auto refractObjectFrame5 = landscape->AddComponent<sdk::MeshRenderer>(refractObjectFrame);
			ASSERT(!refractObjectFrame5, "refractObject frame 5 is nullptr");
			refractObjectFrame5->SetLocalPosition({ 0.0f, 3.0f, 0.0f });
			refractObjectFrame5->meshTransform.scale = Vector3(6.5f, 0.5f, 6.5f);
			refractObjectFrame5->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObjectFrame5->SetMaterial(rimMaterialIndex);
			refractObjectFrame5->name = "Refract frame (5)";

			auto refractObjectFrame6 = landscape->AddComponent<sdk::MeshRenderer>(refractObjectFrame);
			ASSERT(!refractObjectFrame6, "refractObject frame 6 is nullptr");
			refractObjectFrame6->SetLocalPosition({ 0.0f, -3.0f, 0.0f });
			refractObjectFrame6->meshTransform.scale = Vector3(6.5f, 0.5f, 6.5f);
			refractObjectFrame6->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObjectFrame6->SetMaterial(rimMaterialIndex);
			refractObjectFrame6->name = "Refract frame (6)";
		}

		// refract object 2
		{
			auto refractObject2 = landscape->AddComponent<sdk::Component>(refractObjectParent);
			ASSERT(!refractObject2, "refractObject 2 is nullptr");
			refractObject2->SetLocalRotation(Quaternion(M_PI, Vector3(0.0f, 1.0f, 0.0f)));

			auto refractObject2Renderer = landscape->AddComponent<sdk::MeshRenderer>(refractObject2);
			ASSERT(!refractObject2Renderer, "refractObject 2 renderer is nullptr");
			refractObject2Renderer->SetLocalPosition({ 0.0f, 0.0f, 3.0f });
			refractObject2Renderer->meshTransform.scale = Vector3(6.0f, 6.0f, 0.1f);
			refractObject2Renderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObject2Renderer->SetMaterial(areskutanRefractMaterialIndex);
			refractObject2Renderer->name = "Refract (areskutan 3)";
		}

		// refract object 3
		{
			auto refractObject3 = landscape->AddComponent<sdk::Component>(refractObjectParent);
			ASSERT(!refractObject3, "refractObject 3 is nullptr");
			refractObject3->SetLocalRotation(Quaternion(M_PI / 2.0, Vector3(0.0f, 1.0f, 0.0f)));

			auto refractObject3Renderer = landscape->AddComponent<sdk::MeshRenderer>(refractObject3);
			ASSERT(!refractObject3Renderer, "refractObject 3 renderer is nullptr");
			refractObject3Renderer->SetLocalPosition({ 0.0f, 0.0f, 3.0f });
			refractObject3Renderer->meshTransform.scale = Vector3(6.0f, 6.0f, 0.1f);
			refractObject3Renderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObject3Renderer->SetMaterial(midnightRefractMaterialIndex);
			refractObject3Renderer->name = "Refract (midnight 3)";
		}

		// refract object 4
		{
			auto refractObject4 = landscape->AddComponent<sdk::Component>(refractObjectParent);
			ASSERT(!refractObject4, "refractObject 4 is nullptr");

			auto refractObject4Renderer = landscape->AddComponent<sdk::MeshRenderer>(refractObject4);
			ASSERT(!refractObject4Renderer, "refractObject renderer is nullptr");
			refractObject4Renderer->SetLocalPosition({ 0.0f, 0.0f, 3.0f });
			refractObject4Renderer->meshTransform.scale = Vector3(6.0f, 6.0f, 0.1f);
			refractObject4Renderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObject4Renderer->SetMaterial(sorcwdRefractMaterialIndex);
			refractObject4Renderer->name = "Refract (sorcwd 3)";
		}

		// refract object 5
		{
			auto refractObject5 = landscape->AddComponent<sdk::Component>(refractObjectParent);
			ASSERT(!refractObject5, "refractObject 5 is nullptr");
			refractObject5->SetLocalRotation(Quaternion(-M_PI / 2.0, Vector3(0.0f, 1.0f, 0.0f)));

			auto refractObject5Renderer = landscape->AddComponent<sdk::MeshRenderer>(refractObject5);
			ASSERT(!refractObject5Renderer, "refractObject renderer is nullptr");
			refractObject5Renderer->SetLocalPosition({ 0.0f, 0.0f, 3.0f });
			refractObject5Renderer->meshTransform.scale = Vector3(6.0f, 6.0f, 0.1f);
			refractObject5Renderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			refractObject5Renderer->SetMaterial(spiresRefractMaterialIndex);
			refractObject5Renderer->name = "Refract (spires 3)";
		}

		// auto refractObject5 = landscape->AddComponent<sdk::MeshRenderer>(refractObjectParent);
		// ASSERT(!refractObject5, "refractObject 5 is nullptr");
		// refractObject5->SetLocalPosition({ 6.0f, 3.0f, 4.0f });
		// refractObject5->SetScale(2.0f);
		// refractObject5->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
		// refractObject5->SetMaterial(spiresRefractMaterialIndex);
		// refractObject5->name = "Refract (spires)";

		auto boomboxPedestal = landscape->AddComponent<sdk::MeshRenderer>();
		boomboxPedestal->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cylinder));
		boomboxPedestal->meshTransform.scale = Vector3(1.2f, 1.5f, 1.2f);
		boomboxPedestal->SetMaterial(litMaterialIndex);
		boomboxPedestal->name = "BoomBox Pedestal";
		boomboxPedestal->SetScale(1.5f);
		boomboxPedestal->SetLocalPosition(Vector3(28.0f, 1.0f, 62.0f));
		landscape->AddComponent<sdk::BoxCollider>(boomboxPedestal, Vector3(0.5f, 0.75f, 0.5f));


		for (int j = -1; j < 2; j += 2)
		{
			auto wall = landscape->AddComponent<sdk::MeshRenderer>(groundMainRoom);
			wall->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			wall->meshTransform.scale = Vector3(2.0f, 11.0f, 130.0f);
			wall->name = "Wall";
			wall->scaleUV.x = 14.0f;
			wall->SetMaterial(litMaterialIndex);
			wall->isLightScatteringObstacle = false;

			wall->SetLocalPosition(Vector3(j * 51.0f, 5.5f, 0.0f));

			auto collider = landscape->AddComponent<sdk::PlaneCollider>(wall);
			collider->SetLocalRotation(Quaternion(j * SQRT2 / 2.0f, 0.0f, 0.0f, SQRT2 / 2.0f));
			collider->SetLocalPosition(Vector3(j * -1.0f, 0.0f, 0.0f));
		}

		for (int j = -1; j < 2; j += 2)
		{
			auto wall = landscape->AddComponent<sdk::MeshRenderer>(groundMainRoom);
			wall->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			wall->meshTransform.scale = Vector3(12.0f, 1.0f, 2.0f);
			wall->scaleUV.y = 0.1f;
			wall->SetMaterial(litMaterialIndex);
			wall->name = "Wall";
			wall->isLightScatteringObstacle = false;

			Vector3 position = Vector3(0.0f, 11.0f, 65.0f);
			position.z = position.z * j;

			wall->SetLocalPosition(position);
		}

		for (int j = -1; j < 2; j += 2)
		{
			for (int i = -1; i < 2; i += 2)
			{
				auto wall = landscape->AddComponent<sdk::MeshRenderer>(groundMainRoom);
				wall->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
				wall->meshTransform.scale = Vector3(45.0f, 11.0f, 1.0f);
				wall->scaleUV.x = 3.0f;
				wall->SetMaterial(litMaterialIndex);
				wall->name = "Wall";
				wall->isLightScatteringObstacle = false;

				wall->SetLocalPosition(Vector3(i * 27.5f, 5.5f, j * 64.5f));
				auto collider = landscape->AddComponent<sdk::BoxCollider>(wall, Vector3(22.5f, 5.5f, 1.5f));
			}
		}

		// End

		auto awesomeDoorCollider = landscape->AddComponent<sdk::BoxCollider>();
		awesomeDoorCollider->SetScale(4);
		awesomeDoorCollider->isTrigger = true;
		awesomeDoorCollider->halfScale = Vector3(0.5f, 0.9f, 0.125f);
		awesomeDoorCollider->name = "AwesomeDoorCollider";
		awesomeDoorCollider->SetLocalPosition(Vector3(0.0f, 3.58859f, -105.00524f));

		auto doorBehaviour = landscape->AddComponent<sdk::Script>(awesomeDoorCollider, "assets/scripts/door_behaviour.lua");
		doorBehaviour->AddOrSetBinding("level", levelComp);

		auto awesomeDoorMesh = landscape->AddComponent<sdk::MeshRenderer>(awesomeDoorCollider);
		awesomeDoorMesh->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
		//awesomeDoorMesh->SetMaterial(awesomeMaterialIndex);
		awesomeDoorMesh->SetMaterial(reflectMaterialIndex);
		awesomeDoorMesh->name = "AwesomeDoor";
		//awesomeDoorMesh->SetLocalPosition(Vector3(0.0f, 3.0f, -98.0f));
		awesomeDoorMesh->meshTransform.scale = Vector3(1, 1.80099f, 0.25344f);

		//auto awesomeDoorMeshCollider = landscape->AddComponent<sdk::BoxCollider>(awesomeDoorMesh);
		//awesomeDoorMeshCollider->isTrigger = true;
		//awesomeDoorMeshCollider->halfScale = Vector3(0.5f, 0.9f, 0.125f);
		//awesomeDoorMeshCollider->name = "awesomeDoorMeshCollider";

		auto awesomeLight = landscape->AddComponent<sdk::PointLight>(awesomeDoorMesh);
		awesomeLight->color = { 10000.0f, 10000.0f, 20000.0f };
		awesomeLight->SetLocalPosition(Vector3());
		awesomeLight->name = "awesomeLight";

		awesomeDoorMesh->SetActive(false);
		//awesomeDoorCollider->SetActive(false);
		//awesomeLight->SetActive(false);

		//auto awesomeBehaviour = landscape->AddComponent<sdk::Script>(awesomeSphere, "assets/scripts/awesomeTrigger.lua");

		// === Cube Heap ===

		ph::Material mat(10.f, 10.f, 0.0f);
		std::string name = "CubeHeap";

		const int size3 = 9;
		const Vector3 pos3[size3]{ Vector3(-3.4f, 0.0f, 2.6f), Vector3(-1.0f, 0.0f, 2.3f), Vector3(1.56f, 0.0f, 2.28f),
			Vector3(3.95f, 0.0f, 2.5f), Vector3(-2.1f, 2.0f, 2.55f), Vector3(0.5f, 2.0f, 2.50f),
			Vector3(3.0f, 2.0f, 2.6f), Vector3(-0.63f, 4.0f, 2.94f), Vector3(2.21f, 4.0f, 2.51f) };
		const Vector3 rot3[size3]{ Vector3(0.0f, -1.26f, 0.0f), Vector3(0.0f, 11.3f, 0.0f), Vector3(0.0f, -5.0f, 0.0f),
			Vector3(0.0f, -8.0f, 0.0f), Vector3(-1.47f, 0.0f, 0.0f), Vector3(0.0f, -15.54f, 0.0f),
			Vector3(0.0f, 9.35f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f) };

		for (int i = 0; i < size3; i++)
		{
			sdk::Actor* cubeHeap = m_scene->CreateActor(level, name);

			auto cubeHeapRenderer = cubeHeap->AddComponent<sdk::MeshRenderer>();
			cubeHeapRenderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			cubeHeapRenderer->SetScale(2.0f);
			//cubeHeapRenderer->SetMaterial(litMaterialIndex);
			cubeHeapRenderer->SetGlobalPosition(pos3[i]);
			cubeHeapRenderer->SetGlobalRotation(Quaternion::FromEulerAngles(rot3[i]));
			auto cubeHeapBody = cubeHeap->AddComponent<sdk::Rigidbody>(cubeHeapRenderer, ph::BoxGeometry(), mat);
			cubeHeapBody->SetKinematic(true);
			cubeHeapBody->name = name;

			cubeHeap->AddComponent<sdk::SoundEmitter>();

			auto cubeHeapCollScript = cubeHeap->AddComponent<sdk::Script>(nullptr, "assets/scripts/collision_sound.lua");
			cubeHeapCollScript->AddOrSetBinding("placement_impact", true);
		}

		// === Wrecking Ball ===

		sdk::Actor* wreckingBall = m_scene->CreateActor(level, "Wrecking Ball");

		auto wreckingBallbody = wreckingBall->AddComponent<sdk::Rigidbody>(nullptr, ph::SphereGeometry(), ph::Material(10.f, 10.f, 0.2f), 80.0f);
		wreckingBallbody->SetLocalPosition(Vector3(0.0f, 3.5f, -22.0f));
		wreckingBallbody->SetScale(7);
		wreckingBallbody->SetKinematic(true);

		auto wreckingBallRenderer = wreckingBall->AddComponent<sdk::MeshRenderer>();
		wreckingBallRenderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Sphere));

		wreckingBallbody->GlobalTransform();
		wreckingBall->SetActive(false);

		//wreckingBallRenderer->SetMaterial(litMaterialIndex);

		//// === Trigger ===

		//sdk::Actor* wreckingTriggerActor = m_scene->CreateActor(level, "Wrecking Trigger");

		//auto wreckingTriggerRenderer = wreckingTriggerActor->AddComponent<sdk::MeshRenderer>();
		//wreckingTriggerRenderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
		//wreckingTriggerRenderer->SetScale(1);
		////wreckingTriggerRenderer->SetMaterial(litMaterialIndex);
		//wreckingTriggerRenderer->SetLocalPosition(Vector3(-31.0f, 1.5f, 34.0f));
		//auto wreckingTriggerBox = wreckingTriggerActor->AddComponent<sdk::BoxCollider>();
		//wreckingTriggerBox->isTrigger = true;

		//auto wreckingTriggerBehaviour = wreckingTriggerActor->AddComponent<sdk::Script>(nullptr, "assets/scripts/wreckingTrigger.lua");
		//wreckingTriggerBehaviour->AddOrSetBinding("wreckingBall", static_cast<sdk::Component*>(wreckingBallbody));
		//wreckingTriggerBehaviour->AddOrSetBinding("impulseForce", 3000.0f);
		//wreckingTriggerBehaviour->AddOrSetBinding("portal", static_cast<sdk::Component*>(awesomeDoorMesh));

		//auto wreckingSE = wreckingTriggerActor->AddComponent<sdk::SoundEmitter>();

		const int size4 = 3;
		Vector3 pos4[size4]{ Vector3(-44.5f, 5.5f, 22.0f), Vector3(-44.5f, 5.5f, 60.0f), Vector3(-44.5f, 5.5f, 115.0f) };

		for (int j = -1; j < 2; j += 2)
		{
			for (int i = 0; i < size4; i++)
			{
				auto pLight = landscape->AddComponent<aw::sdk::PointLight>();
				pLight->name = "Lantern";

				Vector3 position = pos4[i];
				position.x = position.x * j;

				pLight->SetGlobalPosition(position);

				pLight->color.red = 50000;	pLight->color.green = 50000; pLight->color.blue = 20000;
			}
		}

		// === Radio ===
		sdk::Actor* radio = Import("assets/glTF/BoomBox/BoomBox.gltf", level);

		radio->AddComponent<sdk::Script>(nullptr, "assets/scripts/radio_behaviour.lua");

		auto collScript = radio->AddComponent<sdk::Script>(nullptr, "assets/scripts/collision_sound.lua");
		collScript->AddOrSetBinding("placement_impact", false);

		radio->AddComponent<sdk::SoundEmitter>();
		auto radioBody = radio->AddComponent<sdk::Rigidbody>(nullptr, ph::SphereGeometry(), ph::Material(10.0f, 10.0f, 0.2f));
		radioBody->SetScale(0.02f);
		radioBody->name = radio->name;

		radio->SetGlobalPosition({ 28.0f, 1.8f, 127.0 });
		radio->SetScale(70);

		// === Switch ===

		sdk::Actor* switchActor = m_scene->CreateActor(level, "Switch");
		auto switchComp = switchActor->AddComponent<sdk::Component>();
		switchComp->SetGlobalPosition(Vector3(-50.0f, 3.0f, 28.0f));

		auto switchScript = switchActor->AddComponent<sdk::Script>(switchComp, "assets/scripts/note_switch_root.lua");
		switchScript->AddOrSetBinding("wreckingBall", static_cast<sdk::Component*>(wreckingBallbody));
		switchScript->AddOrSetBinding("impulseForce", 3000.0f);
		switchScript->AddOrSetBinding("portal", static_cast<sdk::Component*>(awesomeDoorMesh));

		switchActor->AddComponent<sdk::SoundEmitter>(switchComp);

		const int size5 = 4;
		const std::string path5[size5]{"", "", "", ""};
		const std::string switchNotes[4] = { "C6JingleNote", "DS5JingleNote", "AS5JingleNote", "G5JingleNote" };
		const uint32 switchNotesMaterials[4] = { areskutanRefractMaterialIndex, yokohamaRefractMaterialIndex, sorcwdRefractMaterialIndex, midnightRefractMaterialIndex };

		for (int i = 0; i < size5; i++)
		{
			sdk::Actor* switchChild = m_scene->CreateActor(switchActor, "SwitchChild");

			auto renderer = switchChild->AddComponent<sdk::MeshRenderer>();
			renderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
			renderer->SetMaterial(switchNotesMaterials[i]);
			renderer->SetLocalPosition(Vector3(0.0f, 0.0f, i * 4.0f));
			auto triggerBox = switchChild->AddComponent<sdk::Rigidbody>(renderer, ph::BoxGeometry());
			triggerBox->SetKinematic(true);

			auto script = switchChild->AddComponent<sdk::Script>(renderer, "assets/scripts/note_switch.lua");
			script->AddOrSetBinding("noteToPlay", switchNotes[i]);

		//	auto triggerScript = switchActor->AddComponent<sdk::Script>(renderer, path5[i]);
		}

		// === Other ===

		sdk::RenderSystem::editorCamera = &m_editorCamera;
		{
			m_editorCamera.renderArea.viewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f);
			m_editorCamera.renderArea.scissor = vk::Rect2D({}, { window->GetWidth(), window->GetHeight() });
			m_editorCamera.SetWidth(m_editorCamera.renderArea.viewport.width);
			m_editorCamera.SetHeight(m_editorCamera.renderArea.viewport.height);
			m_editorCamera.SetNear(0.001f);
			m_editorCamera.isImplicit = true;
		}

		// === TEXT ===

		//auto actorText = m_scene->CreateActor(nullptr, "Debug Text");
		//ASSERT(!actorText, "actorText is nullptr");

		//auto textMeshRenderer1 = actorText->AddComponent<sdk::Text2D>();
		//ASSERT(!textMeshRenderer1, "textMeshRenderer1 is nullptr");
		//textMeshRenderer1->SetGlobalPosition({ 5.0f, 50.0f, 0.0f });
		//textMeshRenderer1->SetContent("frameTime:");

		//auto textScript1 = actorText->AddComponent<sdk::Script>(textMeshRenderer1, "assets/scripts/frameTime_txt_behaviour.lua");
		//ASSERT(!textScript1, "textScript1 is nullptr");

		//auto textMeshRenderer2 = actorText->AddComponent<sdk::Text2D>();
		//ASSERT(!textMeshRenderer2, "textMeshRenderer2 is nullptr");
		//textMeshRenderer2->SetGlobalPosition({ 5.0f, 120.0f, 0.0f });
		//textMeshRenderer2->SetContent("Avg frameTime:");

		//auto textScript2 = actorText->AddComponent<sdk::Script>(textMeshRenderer2, "assets/scripts/avg_frameTime_txt_behaviour.lua");
		//ASSERT(!textScript2, "textScript2 is nullptr");

		//auto textMeshRenderer3 = actorText->AddComponent<sdk::Text2D>();
		//ASSERT(!textMeshRenderer3, "textMeshRenderer3 is nullptr");
		//textMeshRenderer3->SetGlobalPosition({ 5.0f, 180.0f, 0.0f });
		//textMeshRenderer3->SetContent("Frame count:");

		//auto textScript3 = actorText->AddComponent<sdk::Script>(textMeshRenderer3, "assets/scripts/frameCount_txt_behaviour.lua");
		//ASSERT(!textScript3, "textScript3 is nullptr");

		//textMeshRenderer1->SetScale(0.35f);

		//// === Lights ===

		//auto actorMoveLight = m_scene->CreateActor(nullptr, "Moving Light");
		//ASSERT(!actorMoveLight, "actorMoveLight is nullptr");

		//auto lightScript = actorMoveLight->AddComponent<sdk::Script>(nullptr, "assets/scripts/light.lua");
		//ASSERT(!lightScript, "lightScript is nullptr");

		//auto pointLightComponent = actorMoveLight->AddComponent<sdk::PointLight>();
		//ASSERT(!pointLightComponent, "pointLightComponent 1 is nullptr");
		//pointLightComponent->color = Color4f(100.0f, 100.0f, 100.0f, 1.0f);

		//sdk::Actor* actorStaticLight = m_scene->CreateActor(nullptr, "Static Light");
		//ASSERT(!actorStaticLight, "actorStaticLight is nullptr");

		//pointLightComponent = actorStaticLight->AddComponent<sdk::PointLight>();
		//ASSERT(!pointLightComponent, "pointLightComponent 2 is nullptr");
		//pointLightComponent->color = Color4f::Blue();
		//pointLightComponent->color.red *= 100.0f;
		//pointLightComponent->color.green *= 100.0f;
		//pointLightComponent->color.blue *= 100.0f;
		//pointLightComponent->SetGlobalPosition({ 0.0f, -5.0f, -5.0f });

		//pointLightComponent = actorStaticLight->AddComponent<sdk::PointLight>();
		//ASSERT(!pointLightComponent, "pointLightComponent 3 is nullptr");
		//pointLightComponent->color = Color4f::Red();
		//pointLightComponent->color.red *= 100.0f;
		//pointLightComponent->color.green *= 100.0f;
		//pointLightComponent->color.blue *= 100.0f;
		//pointLightComponent->SetGlobalPosition({ 0.0f, 5.0f, 0.0f });

		//auto direcionalLightComponent = actorStaticLight->AddComponent<sdk::DirectionalLight>();
		//ASSERT(!direcionalLightComponent, "direcionalLightComponent is nullptr");
		//direcionalLightComponent->color = Color4f::White();
		//direcionalLightComponent->color.red *= 25.0f;
		//direcionalLightComponent->color.green *= 25.0f;
		//direcionalLightComponent->color.blue *= 25.0f;
		//direcionalLightComponent->SetGlobalPosition({ -2.0f, 4.0f, 0.0f });

		//// === Cube ===

		//sdk::Actor* cubeActor = m_scene->CreateActor(nullptr, "Cube 1");
		//ASSERT(!cubeActor, "cubeActor is nullptr");

		//auto cubeMeshRenderer = cubeActor->AddComponent<sdk::MeshRenderer>();
		//ASSERT(!cubeMeshRenderer, "cubeMeshRenderer is nullptr");
		//cubeMeshRenderer->SetLocalPosition({ 0.0f, -3.0f, -3.0f });
		//cubeMeshRenderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cube));
		//cubeMeshRenderer->SetMaterial(litMaterialIndex);
		//cubeMeshRenderer->meshTransform.scale.y = 2.0f;
		//cubeActor->SetLocalPosition({ -1.5f, 0.0f, -5.0f });

		//// === Spheres ===

		//sdk::Actor* actor1 = m_scene->CreateActor(nullptr, "Sphere 1");
		//ASSERT(!actor1, "actor1 is nullptr");

		//auto meshRenderer1 = actor1->AddComponent<sdk::MeshRenderer>();
		//ASSERT(!meshRenderer1, "meshRenderer 1 is nullptr");
		//meshRenderer1->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Sphere));
		//meshRenderer1->SetMaterial(litMaterialIndex);
		//meshRenderer1->name = "Lit (Mesh1)";
		////meshRenderer1->SetScale(0.5f);
		//meshRenderer1->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		////meshRenderer1->SetLocalRotation(Quaternion(M_PI2, { 0.0f, 1.0f, 0.0f }));

		//auto meshRenderer1Behaviour = actor1->AddComponent<sdk::Script>(meshRenderer1, "assets/scripts/mesh1_behaviour.lua");
		//ASSERT(!meshRenderer1Behaviour, "meshRenderer1Behaviour is nullptr");

		//auto meshRenderer2 = actor1->AddComponent<sdk::MeshRenderer>(meshRenderer1);
		//ASSERT(!meshRenderer2, "meshRenderer 2 is nullptr");
		//meshRenderer2->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		//meshRenderer2->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Sphere));
		//meshRenderer2->SetMaterial(awesomeMaterialIndex);
		//meshRenderer2->name = "Awesome (Mesh2)";

		//auto meshRenderer2Behaviour = actor1->AddComponent<sdk::Script>(meshRenderer2, "assets/scripts/mesh2_behaviour.lua");
		//ASSERT(!meshRenderer2Behaviour, "meshRenderer2Behaviour is nullptr");

		//auto meshRenderer3 = actor1->AddComponent<sdk::MeshRenderer>(meshRenderer2);
		//ASSERT(!meshRenderer3, "meshRenderer 3 is nullptr");
		//meshRenderer3->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		//meshRenderer3->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Sphere));
		//meshRenderer3->SetMaterial(reflectMaterialIndex);
		//meshRenderer3->name = "Reflect (Mesh3)";

		//auto meshRenderer3Behaviour = actor1->AddComponent<sdk::Script>(meshRenderer3, "assets/scripts/mesh3_behaviour.lua");
		//ASSERT(!meshRenderer3Behaviour, "meshRenderer3Behaviour is nullptr");

		//auto meshRenderer4 = actor1->AddComponent<sdk::MeshRenderer>(meshRenderer3);
		//ASSERT(!meshRenderer4, "meshRenderer 4 is nullptr");
		//meshRenderer4->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		//meshRenderer4->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Sphere));
		//meshRenderer4->SetMaterial(yokohamaRefractMaterialIndex);
		//meshRenderer4->name = "Refract (Mesh4)";

		//auto meshRenderer4Behaviour = actor1->AddComponent<sdk::Script>(meshRenderer4, "assets/scripts/mesh4_behaviour.lua");
		//ASSERT(!meshRenderer4Behaviour, "meshRenderer4Behaviour is nullptr");

		//auto soundEmitter = actor1->AddComponent<sdk::SoundEmitter>(meshRenderer4);
		//ASSERT(!soundEmitter, "soundEmitter is nullptr");

		//// === Hemisphere ===

		//sdk::Actor* hemisphereActor = m_scene->CreateActor(nullptr, "Hemisphere");
		//ASSERT(!hemisphereActor, "hemisphereActor is nullptr");

		//auto hemisphereMeshRenderer = hemisphereActor->AddComponent<sdk::MeshRenderer>();
		//ASSERT(!hemisphereMeshRenderer, "hemisphereMeshRenderer is nullptr");
		//hemisphereMeshRenderer->SetLocalPosition({ 0.0f, 0.0f, -10.0f });
		//hemisphereMeshRenderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Hemiphere));
		//hemisphereMeshRenderer->SetMaterial(litMaterialIndex);
		//hemisphereMeshRenderer->isLightScatteringEmitter = true;

		//// === Cone ===

		//sdk::Actor* coneActor = m_scene->CreateActor(nullptr, "Cone");
		//ASSERT(!coneActor, "coneActor is nullptr");

		//auto coneMeshRenderer = coneActor->AddComponent<sdk::MeshRenderer>();
		//ASSERT(!coneMeshRenderer, "coneMeshRenderer is nullptr");
		//coneActor->SetLocalPosition({ 0.0f, -1.2f, -5.0f });
		//coneMeshRenderer->SetLocalRotation({ 0.0f, 0.0f, 0.0f, 1.0f });
		//coneMeshRenderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cone));
		//coneMeshRenderer->SetMaterial(litMaterialIndex);

		//// === Cylindre ===

		//sdk::Actor* cylindreActor = m_scene->CreateActor(nullptr, "Cylindre");
		//ASSERT(!cylindreActor, "cylindreActor is nullptr");

		//auto cylindreMeshRenderer = cylindreActor->AddComponent<sdk::MeshRenderer>();
		//ASSERT(!cylindreMeshRenderer, "cylindreMeshRenderer is nullptr");
		//cylindreMeshRenderer->SetLocalPosition({ 1.5f, 0.0f, -5.0f });
		//cylindreMeshRenderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Cylinder));
		//cylindreMeshRenderer->SetMaterial(litMaterialIndex);

		//// === Capsule ===

		//sdk::Actor* capsuleActor = m_scene->CreateActor(nullptr, "Capsule");
		//ASSERT(!capsuleActor, "capsuleActor is nullptr");

		//auto capsuleMeshRenderer = capsuleActor->AddComponent<sdk::MeshRenderer>();
		//ASSERT(!capsuleMeshRenderer, "capsuleMeshRenderer is nullptr");
		//capsuleMeshRenderer->SetLocalPosition({ 0.0f, 2.0f, -5.0f });
		//capsuleMeshRenderer->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Capsule));
		//capsuleMeshRenderer->SetMaterial(litMaterialIndex);

		//// === Camera & Skybox ===

		//auto skyboxComponent = actor1->AddComponent<sdk::Skybox>();
		//ASSERT(!skyboxComponent, "skyboxComponent is nullptr");
		//skyboxComponent->SetMesh(static_cast<uint32>(sdk::PrimitivesIndex::Skybox));
		//skyboxComponent->SetMaterial(skyboxMaterialIndex);

		//sdk::Actor* player = m_scene->CreateActor(nullptr, "Player");
		//ASSERT(!player, "player is nullptr");

		//m_cameraComponent = player->AddComponent<sdk::Camera>();
		//ASSERT(!m_cameraComponent, "m_cameraComponent is nullptr");
		//{
		//	m_cameraComponent->renderArea.viewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f);
		//	m_cameraComponent->renderArea.scissor = vk::Rect2D({}, { window->GetWidth(), window->GetHeight() });
		//	m_cameraComponent->SetWidth(m_cameraComponent->renderArea.viewport.width);
		//	m_cameraComponent->SetHeight(m_cameraComponent->renderArea.viewport.height);
		//	m_cameraComponent->SetNear(0.001f);
		//}

		////m_editorCamera = std::make_unique<sdk::Camera>(nullptr);
		//sdk::RenderSystem::editorCamera = &m_editorCamera;
		//{
		//	m_editorCamera.renderArea.viewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(window->GetWidth()), static_cast<float>(window->GetHeight()), 0.0f, 1.0f);
		//	m_editorCamera.renderArea.scissor = vk::Rect2D({}, { window->GetWidth(), window->GetHeight() });
		//	m_editorCamera.SetWidth(m_editorCamera.renderArea.viewport.width);
		//	m_editorCamera.SetHeight(m_editorCamera.renderArea.viewport.height);
		//	m_editorCamera.SetNear(0.001f);
		//	m_editorCamera.isImplicit = true;
		//}

		//auto camPointLightComponent = player->AddComponent<sdk::PointLight>();
		//ASSERT(!camPointLightComponent, "camPointLightComponent is nullptr");

		//auto listenerComponent = player->AddComponent<sdk::SoundListener>();
		//ASSERT(!listenerComponent, "listenerComponent is nullptr");

		///*
		//auto textMeshRenderer1 = mainCamera->AddComponent<sdk::MeshRenderer>(nullptr, &textModel);
		//ASSERT(!textMeshRenderer1, "textMeshRenderer1 is nullptr");
		////textMeshRenderer1->SetGlobalPosition({ -0.25f, 0.25f, -1.01f });
		//textMeshRenderer1->SetGlobalPosition({ 5.0f, 20.0f, 0.01f });
		//textMeshRenderer1->SetGlobalRotation({ 1, 0, 0, 0 });
		//textMeshRenderer1->useTextOrtho = true;
		////*/
		////textMeshRenderer1->SetLocalRotation(Quaternion(3.14159f, Vector3(1, 0, 0)));
		////textMeshRenderer1->SetScale(1.0f / 1280.0f);


		//auto playerBehaviour = player->AddComponent<sdk::Script>(nullptr, "assets/scripts/player.lua");
		//ASSERT(!playerBehaviour, "playerBehaviour is nullptr");

		//playerBehaviour->AddOrSetBinding("cameraRotationSpeed", 0.9f);
		//playerBehaviour->AddOrSetBinding("cameraTranslationSpeed", 15);
		//playerBehaviour->AddOrSetBinding("mouseSpeed", 1);
		////cameraScriptComponent->AddOrSetBinding("camera", static_cast<sdk::Component*>(m_cameraComponent));
		//playerBehaviour->AddOrSetBinding("reach", 15);
		//playerBehaviour->AddOrSetBinding("force", 21.5f);

		////auto camteraTestRenderer = mainCamera->AddComponent<MeshRenderer>(m_cameraComponent, &box);
		////ASSERT(!camteraTestRenderer, "camteraTestRenderer is nullptr");
		////camteraTestRenderer->SetLocalPosition({ 0.0f, 0.0f, -5.0f });
		////camteraTestRenderer->SetLocalRotation(Quaternion(M_PI2, Vector3::Right()));

		//// === Physics ===

		//sdk::Actor* dynamicActor = m_scene->CreateActor(nullptr, "Dynamic Body");
		//ASSERT(!dynamicActor, "dynamicActor is nullptr");

		//auto dynamicBody = dynamicActor->AddComponent<sdk::Rigidbody>(nullptr, ph::CapsuleGeometry(), ph::Material(0.5f, 0.5f, 1.0f));
		//ASSERT(!dynamicBody, "dynamicBody is nullptr");

		//auto dynamicActorRenderer = dynamicActor->AddComponent<sdk::MeshRenderer>(dynamicBody);
		//ASSERT(!dynamicActorRenderer, "dynamicActorRenderer is nullptr");
		//dynamicActorRenderer->SetMesh(static_cast<aw::uint32>(aw::sdk::PrimitivesIndex::Capsule));

		//dynamicActor->Translate_WorldSpace({ 10.0f, 20.0f, -5.0f });

		/////___________________________________________________________________

		//sdk::Actor* staticActor = m_scene->CreateActor(nullptr, "Static Body");
		//ASSERT(!staticActor, "staticActor is nullptr");

		//auto staticBody = staticActor->AddComponent<sdk::SphereCollider>(nullptr);
		//ASSERT(!staticBody, "staticBody is nullptr");

		//staticActor->SetGlobalPosition({ 10.0f, 0.0f, -5.0f });

		/////___________________________________________________________________

		//sdk::Actor* triggerActor = m_scene->CreateActor(nullptr, "Trigger Body");
		//ASSERT(!triggerActor, "triggerActor is nullptr");

		//auto triggerBody = triggerActor->AddComponent<sdk::BoxCollider>(nullptr, Vector3(0.5f, 0.5f, 0.5f));
		//ASSERT(!triggerBody, "triggerBody is nullptr");
		//triggerBody->isTrigger = true;

		//triggerActor->SetGlobalPosition({ 10.0f, 5.0f, -5.0f });

		//sdk::Actor* planeActor = m_scene->CreateActor(nullptr, "Plane Body");
		//ASSERT(!planeActor, "planeActor is nullptr");

		//auto planeBody = planeActor->AddComponent<sdk::PlaneCollider>();
		//ASSERT(!planeBody, "planeBody is nullptr");
		//planeBody->SetGlobalPosition(Vector3(0, -5, 0));

		////sdk::Actor* physicsInteraction = m_scene->CreateActor(nullptr, "Physic Script");
		////ASSERT(!physicsInteraction, "physicsInteraction is nullptr");

		////auto physicsBehaviour = physicsInteraction->AddComponent<sdk::Script>(nullptr, "assets/scripts/physics.lua");
		////ASSERT(!physicsBehaviour, "physicsBehaviour is nullptr");

		////physicsBehaviour->AddOrSetBinding("camera", static_cast<sdk::Component*>(m_cameraComponent));
		////physicsBehaviour->AddOrSetBinding("reach", 10);
		////physicsBehaviour->AddOrSetBinding("force", 21.5f);

		//// === GUI ===

		//sdk::Actor* guiActor = m_scene->CreateActor(nullptr, "GUI Actor");
		//ASSERT(!guiActor, "guiActor is nullptr");

		//// === Sound ===

		//sdk::Actor* soundActor = m_scene->CreateActor(nullptr, "Sound Actor");
		//ASSERT(!soundActor, "soundActor is nullptr");

		//auto soundActorBehaviour = soundActor->AddComponent<sdk::Script>(nullptr, "assets/scripts/sound.lua");
		//ASSERT(!soundActorBehaviour, "soundActorBehaviour is nullptr");

		//auto soundActorEmitter = soundActor->AddComponent<sdk::SoundEmitter>();
		//ASSERT(!soundActorEmitter, "soundActorEmitter is nullptr");

		window.SetOnOutOfDateCallback(
			[this](uint32 width, uint32 height)
		{
			m_cameraComponent->renderArea.viewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f);
			m_cameraComponent->renderArea.scissor = vk::Rect2D({}, { width, height });
			m_cameraComponent->SetWidth(m_cameraComponent->renderArea.viewport.width);
			m_cameraComponent->SetHeight(m_cameraComponent->renderArea.viewport.height);

			m_editorCamera.renderArea.viewport = vk::Viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f);
			m_editorCamera.renderArea.scissor = vk::Rect2D({}, { width, height });
			m_editorCamera.SetWidth(m_editorCamera.renderArea.viewport.width);
			m_editorCamera.SetHeight(m_editorCamera.renderArea.viewport.height);

			sdk::RenderSystem::Resize(width, height);
		}
		);

		m_editWin->show();
		m_splash.close();

		sdk::SceneManager::Start();
		sdk::ScriptSystem::Start(m_shouldUpdateScripts);
		sdk::SoundSystem::Start(m_shouldUpdateSound);
		sdk::RenderSystem::Start(m_editWin->GetGameWindow(), m_editorCamera, m_shouldUpdateGraphics, m_isPlaying, m_landmarkControlAbsolute, m_currentToolInt, &m_selectedComponent);
		sdk::PhysicalSystem::Start(m_isPlaying);

		//Context::LogVulkan = false;
	}

	void EditorApplication::Update()
	{
		auto& window = m_editWin->GetGameWindow();

		if (m_isPlaying)
		{
			while (m_shouldUpdateScripts)
				std::this_thread::yield();

			m_shouldUpdateGraphics = true;
			m_shouldUpdateSound = true;

			sdk::SceneManager::CurrentScene->GarbageCollect();
			sdk::EmptySystem::GarbageCollect(); //Because it does not possess any Update mechanism

			BaseApplication::Update();

			window->Update();// This also update the Qt Window / events

			m_shouldUpdateScripts = true;

			float frameTime = sdk::Time::DeltaTime();
			if (InputManager::GetKeyDown(KeyCode::Space) == true)
			{
				if (InputManager::GetKeyDown(KeyCode::R))
					sdk::RenderSystem::LightScatteringUBO.exposure += 0.01f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::F))
					sdk::RenderSystem::LightScatteringUBO.exposure -= 0.01f * frameTime;

				if (InputManager::GetKeyDown(KeyCode::T))
					sdk::RenderSystem::LightScatteringUBO.decay += 0.01f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::G))
					sdk::RenderSystem::LightScatteringUBO.decay -= 0.01f * frameTime;

				if (InputManager::GetKeyDown(KeyCode::Y))
					sdk::RenderSystem::LightScatteringUBO.density += 0.5f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::H))
					sdk::RenderSystem::LightScatteringUBO.density -= 0.5f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::U))
					sdk::RenderSystem::LightScatteringUBO.weight += 0.5f * frameTime;
				if (InputManager::GetKeyDown(KeyCode::J))
					sdk::RenderSystem::LightScatteringUBO.weight -= 0.5f * frameTime;
			}
		}
		else
		{
			m_shouldUpdateGraphics = true;

			sdk::SceneManager::CurrentScene->GarbageCollect();
			BaseApplication::Update();
			window->Update();// This also update the Qt Window / events

			UpdateCamera();
		}
	}

	void EditorApplication::End()
	{
		Context::LogVulkan = true;

		BaseApplication::End();

		m_shouldUpdateGraphics = false;
		Context::GetDevice().WaitIdle();

		m_scene.reset();
		sdk::RenderSystem::UnInit();
		sdk::PhysicalSystem::UnInit();

		if (m_editWin)
			delete m_editWin;
	}

	void EditorApplication::UpdateCamera()
	{
		auto& window = m_editWin->GetGameWindow();

		/*auto mousePos = InputManager::GetMouseRelativePosition(*window);

		if (mousePos.x >= 0 && mousePos.x <= window->GetWidth() &&
			mousePos.y >= 0 && mousePos.y <= window->GetHeight())
		{*/
		if (InputManager::GetMouseUp(MouseButton::Right))
			UpdateCameraMousePicking();
		//}

			UpdateCameraPosition();
	}

	bool landmarkXHeld = false;
	bool landmarkYHeld = false;
	bool landmarkZHeld = false;
	bool landmarkCHeld = false;
	bool landmarkXYHeld = false;
	bool landmarkYZHeld = false;
	bool landmarkZXHeld = false;

	bool IsLandmarkHeld()
	{
		return landmarkXHeld
			|| landmarkYHeld
			|| landmarkZHeld
			|| landmarkCHeld
			|| landmarkXYHeld
			|| landmarkYZHeld
			|| landmarkZXHeld;
	}

	Vector2 landmarkMousePosHeld{};

	Vector3 editorCameraPositionHeld{};

	Vector3 selectedComponentGlobalPositionHeld{};
	Quaternion selectedComponentGlobalRotationHeld{};

	Vector3 selectedComponentPositionHeld{};
	Quaternion selectedComponentRotationHeld{};
	float selectedComponentScaleHeld{};

	Transform selectedMeshRendererTransform{};

	MousePos outCounter{};

	void EditorApplication::UpdateCameraMousePicking()
	{
		auto& window = m_editWin->GetGameWindow();
		auto mousePos = InputManager::GetMouseRelativePosition(*window);

		if (mousePos.x >= 0 && mousePos.x <= window->GetWidth() &&
			mousePos.y >= 0 && mousePos.y <= window->GetHeight())
		{
			auto ids = sdk::RenderSystem::GetIdsAtPixel(mousePos);

			const uint32& componentId = ids.first;
			const uint32& actorId = ids.second;

			if (InputManager::GetMousePressed(MouseButton::Left))
			{
				outCounter = {};

				if (actorId >= (~0 - 7) && actorId < ~0)
				{
					if (actorId == (~0 - 1))
						landmarkXHeld = true;
					else if (actorId == (~0 - 2))
						landmarkYHeld = true;
					else if (actorId == (~0 - 3))
						landmarkZHeld = true;
					else if (actorId == (~0 - 4))
						landmarkCHeld = true;
					else if (actorId == (~0 - 5))
						landmarkXYHeld = true;
					else if (actorId == (~0 - 6))
						landmarkYZHeld = true;
					else if (actorId == (~0 - 7))
						landmarkZXHeld = true;

					landmarkMousePosHeld = mousePos;

					if (m_selectedComponent != nullptr)
					{
						editorCameraPositionHeld = m_editorCamera.GlobalPosition();

						selectedComponentGlobalPositionHeld = m_selectedComponent->GlobalPosition();
						selectedComponentGlobalRotationHeld = m_selectedComponent->GlobalRotation();

						selectedComponentPositionHeld = m_selectedComponent->GetLocalPosition();
						selectedComponentRotationHeld = m_selectedComponent->GetLocalRotation();

						selectedComponentScaleHeld = m_selectedComponent->Scale();

						const uint32 typeId = m_selectedComponent->typeId;

						if (typeId == TypeInfo<sdk::MeshRenderer>::id)
						{
							sdk::MeshRenderer* meshRenderer = reinterpret_cast<sdk::MeshRenderer*>(m_selectedComponent);
							selectedMeshRendererTransform = meshRenderer->meshTransform;
						}
					}
				}
				else if (componentId != ~0 && actorId != ~0)
				{
					sdk::Actor* actor = sdk::SceneManager::FindActorById(actorId);

					if (actor != nullptr)
					{
						m_selectedComponent = actor->FindComponentById(componentId);

						if (m_selectedComponent != nullptr)
							m_editWin->PickComponent(m_selectedComponent);
					}
				}
				else
				{
					m_selectedComponent = nullptr;
					m_editWin->compView.Clear();
					m_editWin->actorView.SetSelectedScene(sdk::SceneManager::CurrentScene.get());
				}
			}
		}
		if (InputManager::GetMouseReleased(MouseButton::Left))
		{
			landmarkXHeld = false;
			landmarkYHeld = false;
			landmarkZHeld = false;
			landmarkCHeld = false;
			landmarkXYHeld = false;
			landmarkYZHeld = false;
			landmarkZXHeld = false;
		}
		if (InputManager::GetMouseDown(MouseButton::Left))
		{
			if (m_selectedComponent != nullptr)
			{
				// Virtual space (virtual mouse position after warp)
				if (IsLandmarkHeld())
				{
					MousePos newPos = mousePos;

					if (mousePos.x < 0)
					{
						newPos.x = window->GetWidth();
						outCounter.x--;
					}
					else if (mousePos.x > window->GetWidth())
					{
						newPos.x = 0;
						outCounter.x++;
					}

					if (mousePos.y < 0)
					{
						newPos.y = window->GetHeight();
						outCounter.y--;
					}
					else if (mousePos.y > window->GetHeight())
					{
						newPos.y = 0;
						outCounter.y++;
					}

					if (mousePos.x != newPos.x || mousePos.y != newPos.y)
						InputManager::SetMouseRelativePosition(newPos, *window);

					mousePos.x += outCounter.x * window->GetWidth();
					mousePos.y += outCounter.y * window->GetHeight();
				}

				if (InputManager::GetKeyUp(KeyCode::Shift) == true || m_selectedComponent->typeId != TypeInfo<sdk::MeshRenderer>::id)
				{
					if (m_currentTool == LandmarkTool::Translation)
					{
						const Vector3& cameraPosition = m_editorCamera.GlobalPosition();

						Vector3 mouseHoldTarget = m_editorCamera.ScreenToSpace(landmarkMousePosHeld);
						Vector3 mouseHoldDirection = (mouseHoldTarget - cameraPosition).GetNormalized();

						Vector3 target = m_editorCamera.ScreenToSpace({ mousePos.x, mousePos.y });
						Vector3 direction = (target - cameraPosition).GetNormalized();

						Vector3 camForward = m_editorCamera.GlobalRotation() * Vector3::Forward();

						Vector3 landmarkMousePosHeld3D;
						auto translationOnPlane = [&](const Plane& plane) -> Vector3
						{
							if (Ray(cameraPosition, mouseHoldDirection).CastBidirectional(plane, &landmarkMousePosHeld3D) == true)
							{
								Vector3 newPos;
								if (Ray(cameraPosition, direction).CastBidirectional(plane, &newPos) == true)
									return newPos - landmarkMousePosHeld3D;
							}

							return {};
						};

						if (landmarkCHeld == true)
						{
							Plane cameraObjectPlane(camForward, selectedComponentGlobalPositionHeld);

							Vector3 directionToMouse3D = translationOnPlane(cameraObjectPlane);

							if (m_landmarkControlAbsolute == true)
								m_selectedComponent->SetGlobalPosition(selectedComponentGlobalPositionHeld + directionToMouse3D);
							else
								m_selectedComponent->SetLocalPosition(selectedComponentPositionHeld + directionToMouse3D);

							if (InputManager::GetKeyDown(KeyCode::Alt))
								m_editorCamera.SetGlobalPosition(editorCameraPositionHeld + directionToMouse3D);
						}
						else
						{
							Vector3 right;
							Vector3 up;
							Vector3 forward;

							Plane rightPlane;
							Plane upPlane;
							Plane forwardPlane;

							if (m_landmarkControlAbsolute == true)
							{
								right = Vector3::Right();
								up = Vector3::Up();
								forward = Vector3::Forward();

								rightPlane = Plane(right, -selectedComponentGlobalPositionHeld.x);
								upPlane = Plane(up, -selectedComponentGlobalPositionHeld.y);
								forwardPlane = Plane(forward, -selectedComponentGlobalPositionHeld.z);
							}
							else
							{
								right = (selectedComponentRotationHeld * Vector3::Right()).GetNormalized();
								up = (selectedComponentRotationHeld * Vector3::Up()).GetNormalized();
								forward = (selectedComponentRotationHeld * Vector3::Forward()).GetNormalized();

								rightPlane = Plane(right, selectedComponentGlobalPositionHeld);
								upPlane = Plane(up, selectedComponentGlobalPositionHeld);
								forwardPlane = Plane(forward, selectedComponentGlobalPositionHeld);
							}

							auto setPosition = [&](const Vector3& direction, const Plane& plane1, const Plane& plane2)
							{
								float dot;

								if (fabs(plane1.normal.Dot(camForward)) > fabs(plane2.normal.Dot(camForward)))
									dot = translationOnPlane(plane1).Dot(direction);
								else
									dot = translationOnPlane(plane2).Dot(direction);

								if (InputManager::GetKeyDown(KeyCode::Control))
									dot = roundf(dot / m_translationSnap) * m_translationSnap;

								m_selectedComponent->SetGlobalPosition(selectedComponentGlobalPositionHeld + dot * direction);

								if (InputManager::GetKeyDown(KeyCode::Alt))
									m_editorCamera.SetGlobalPosition(editorCameraPositionHeld + dot * direction);
							};

							if (landmarkXHeld == true)
								setPosition(right, forwardPlane, upPlane);
							else if (landmarkYHeld == true)
								setPosition(up, forwardPlane, rightPlane);
							else if (landmarkZHeld == true)
								setPosition(forward, upPlane, rightPlane);
							else if (landmarkXYHeld == true)
							{
								Vector3 directionToMouse3D = translationOnPlane(forwardPlane);

								if (m_landmarkControlAbsolute == true)
									m_selectedComponent->SetGlobalPosition(selectedComponentGlobalPositionHeld + directionToMouse3D);
								else
									m_selectedComponent->SetLocalPosition(selectedComponentPositionHeld + directionToMouse3D);

								if (InputManager::GetKeyDown(KeyCode::Alt))
									m_editorCamera.SetGlobalPosition(editorCameraPositionHeld + directionToMouse3D);
							}
							else if (landmarkYZHeld == true)
							{
								Vector3 directionToMouse3D = translationOnPlane(rightPlane);

								if (m_landmarkControlAbsolute == true)
									m_selectedComponent->SetGlobalPosition(selectedComponentGlobalPositionHeld + directionToMouse3D);
								else
									m_selectedComponent->SetLocalPosition(selectedComponentPositionHeld + directionToMouse3D);

								if (InputManager::GetKeyDown(KeyCode::Alt))
									m_editorCamera.SetGlobalPosition(editorCameraPositionHeld + directionToMouse3D);
							}
							else if (landmarkZXHeld == true)
							{
								Vector3 directionToMouse3D = translationOnPlane(upPlane);

								if (m_landmarkControlAbsolute == true)
									m_selectedComponent->SetGlobalPosition(selectedComponentGlobalPositionHeld + directionToMouse3D);
								else
									m_selectedComponent->SetLocalPosition(selectedComponentPositionHeld + directionToMouse3D);

								if (InputManager::GetKeyDown(KeyCode::Alt))
									m_editorCamera.SetGlobalPosition(editorCameraPositionHeld + directionToMouse3D);
							}
						}
					}
					else if (m_currentTool == LandmarkTool::Rotation)
					{
						const Vector3& cameraPosition = m_editorCamera.GlobalPosition();

						Vector3 mouseHoldTarget = m_editorCamera.ScreenToSpace(landmarkMousePosHeld);
						Vector3 mouseHoldDirection = (mouseHoldTarget - cameraPosition).GetNormalized();

						Vector3 target = m_editorCamera.ScreenToSpace({ mousePos.x, mousePos.y });
						Vector3 direction = (target - cameraPosition).GetNormalized();

						Vector3 right;
						Vector3 up;
						Vector3 forward;

						Plane rightPlane;
						Plane upPlane;
						Plane forwardPlane;

						if (m_landmarkControlAbsolute == true)
						{
							right = Vector3::Right();
							up = Vector3::Up();
							forward = Vector3::Forward();

							rightPlane = Plane(right, -selectedComponentGlobalPositionHeld.x);
							upPlane = Plane(up, -selectedComponentGlobalPositionHeld.y);
							forwardPlane = Plane(forward, -selectedComponentGlobalPositionHeld.z);
						}
						else
						{
							right = (selectedComponentRotationHeld * Vector3::Right()).GetNormalized();
							up = (selectedComponentRotationHeld * Vector3::Up()).GetNormalized();
							forward = (selectedComponentRotationHeld * Vector3::Forward()).GetNormalized();

							rightPlane = Plane(right, selectedComponentGlobalPositionHeld);
							upPlane = Plane(up, selectedComponentGlobalPositionHeld);
							forwardPlane = Plane(forward, selectedComponentGlobalPositionHeld);
						}

						auto setRotation = [&](const Plane& plane, const Vector3& landmarkDirection)
						{
							Vector3 mousePosOnPlane;
							Vector3 landmarkMousePosHeld3D;

							if (Ray(cameraPosition, mouseHoldDirection).CastBidirectional(plane, &landmarkMousePosHeld3D) == true)
								Ray(cameraPosition, direction).CastBidirectional(plane, &mousePosOnPlane);

							float angle = -Vector3::AngleSignedBetween(
								mousePosOnPlane - selectedComponentGlobalPositionHeld,
								landmarkMousePosHeld3D - selectedComponentGlobalPositionHeld,
								plane.normal
							);

							if (InputManager::GetKeyDown(KeyCode::Control))
								angle = roundf(angle / m_rotationSnap) * m_rotationSnap;

							m_selectedComponent->SetGlobalRotation(Quaternion(angle, landmarkDirection) * selectedComponentGlobalRotationHeld);
						};

						if (landmarkXHeld == true)
							setRotation(rightPlane, right);
						else if (landmarkYHeld == true)
							setRotation(upPlane, up);
						else if (landmarkZHeld == true)
							setRotation(forwardPlane, forward);
					}
					else if (m_currentTool == LandmarkTool::Scale)
					{
						if (landmarkCHeld == true)
						{
							float scaleDifference = (mousePos.x - landmarkMousePosHeld.x) / 100.0f;

							if (InputManager::GetKeyDown(KeyCode::Control))
								scaleDifference = roundf(scaleDifference / m_scaleSnap) * m_scaleSnap;

							m_selectedComponent->SetScale(selectedComponentScaleHeld + scaleDifference);
						}
					}
				}
				else
				{
					const uint32 typeId = m_selectedComponent->typeId;

					if (typeId == TypeInfo<sdk::MeshRenderer>::id)
					{
						sdk::MeshRenderer* meshRenderer = reinterpret_cast<sdk::MeshRenderer*>(m_selectedComponent);

						if (m_currentTool == LandmarkTool::Translation)
						{
							const Vector3& cameraPosition = m_editorCamera.GlobalPosition();

							Vector3 mouseHoldTarget = m_editorCamera.ScreenToSpace(landmarkMousePosHeld);
							Vector3 mouseHoldDirection = (mouseHoldTarget - cameraPosition).GetNormalized();

							Vector3 target = m_editorCamera.ScreenToSpace({ mousePos.x, mousePos.y });
							Vector3 direction = (target - cameraPosition).GetNormalized();

							Vector3 camForward = m_editorCamera.GlobalRotation() * Vector3::Forward();

							auto translationOnPlane = [&](const Plane& plane) -> Vector3
							{
								Vector3 landmarkMousePosHeld3D;
								if (Ray(cameraPosition, mouseHoldDirection).CastBidirectional(plane, &landmarkMousePosHeld3D) == true)
								{
									Vector3 newPos;
									if (Ray(cameraPosition, direction).CastBidirectional(plane, &newPos) == true)
										return newPos - landmarkMousePosHeld3D;
								}

								return {};
							};

							Vector3 tmpPos = selectedComponentGlobalPositionHeld;
							Quaternion tmpRot = selectedComponentGlobalRotationHeld * selectedMeshRendererTransform.GetRotation();

							Vector3 right = Vector3(tmpRot * Vector3::Right()).GetNormalized();
							Vector3 up = Vector3(tmpRot * Vector3::Up()).GetNormalized();
							Vector3 forward = Vector3(tmpRot * Vector3::Forward()).GetNormalized();

							Plane rightPlane(right, tmpPos);
							Plane upPlane(up, tmpPos);
							Plane forwardPlane(forward, tmpPos);

							auto setPosition = [&](const Vector3& direction, const Vector3& landmarkDirection, const Plane& plane1, const Plane& plane2)
							{
								float dot;

								if (fabs(plane1.normal.Dot(camForward)) > fabs(plane2.normal.Dot(camForward)))
									dot = translationOnPlane(plane1).Dot(direction);
								else
									dot = translationOnPlane(plane2).Dot(direction);

								if (InputManager::GetKeyDown(KeyCode::Control))
									dot = roundf(dot / m_translationSnap) * m_translationSnap;

								meshRenderer->meshTransform.SetPosition(selectedMeshRendererTransform.GetPosition() + dot * landmarkDirection);
							};

							if (landmarkXHeld == true)
								setPosition(right, selectedMeshRendererTransform.GetRotation() * Vector3::Right(), forwardPlane, upPlane);
							else if (landmarkYHeld == true)
								setPosition(up, selectedMeshRendererTransform.GetRotation() * Vector3::Up(), forwardPlane, rightPlane);
							else if (landmarkZHeld == true)
								setPosition(forward, selectedMeshRendererTransform.GetRotation() * Vector3::Forward(), upPlane, rightPlane);
						}
						else if (m_currentTool == LandmarkTool::Rotation)
						{
							const Vector3& cameraPosition = m_editorCamera.GlobalPosition();

							Vector3 mouseHoldTarget = m_editorCamera.ScreenToSpace(landmarkMousePosHeld);
							Vector3 mouseHoldDirection = (mouseHoldTarget - cameraPosition).GetNormalized();

							Vector3 target = m_editorCamera.ScreenToSpace({ mousePos.x, mousePos.y });
							Vector3 direction = (target - cameraPosition).GetNormalized();

							Vector3 tmpPos = selectedComponentGlobalPositionHeld;
							Quaternion tmpRot = selectedComponentGlobalRotationHeld * selectedMeshRendererTransform.GetRotation();

							Vector3 right = Vector3(tmpRot * Vector3::Right()).GetNormalized();
							Vector3 up = Vector3(tmpRot * Vector3::Up()).GetNormalized();
							Vector3 forward = Vector3(tmpRot * Vector3::Forward()).GetNormalized();

							Plane rightPlane(right, tmpPos);
							Plane upPlane(up, tmpPos);
							Plane forwardPlane(forward, tmpPos);

							auto setRotation = [&](const Plane& plane, const Vector3& landmarkDirection)
							{
								Vector3 mousePosOnPlane;
								Vector3 landmarkMousePosHeld3D;

								if (Ray(cameraPosition, mouseHoldDirection).CastBidirectional(plane, &landmarkMousePosHeld3D) == true)
									Ray(cameraPosition, direction).CastBidirectional(plane, &mousePosOnPlane);

								float angle = -Vector3::AngleSignedBetween(
									mousePosOnPlane - selectedComponentPositionHeld,
									landmarkMousePosHeld3D - selectedComponentPositionHeld,
									plane.normal
								);

								if (InputManager::GetKeyDown(KeyCode::Control))
									angle = roundf(angle / m_rotationSnap) * m_rotationSnap;

								meshRenderer->meshTransform.SetRotation(Quaternion(angle, selectedMeshRendererTransform.GetRotation() * landmarkDirection) * selectedMeshRendererTransform.GetRotation());
							};

							if (landmarkXHeld == true)
								setRotation(rightPlane, Vector3::Right());
							else if (landmarkYHeld == true)
								setRotation(upPlane, Vector3::Up());
							else if (landmarkZHeld == true)
								setRotation(forwardPlane, Vector3::Forward());
						}
						else if (m_currentTool == LandmarkTool::Scale)
						{
							if (landmarkCHeld == true)
							{
								float scaleDifference = (mousePos.x - landmarkMousePosHeld.x) / 100.0f;

								if (InputManager::GetKeyDown(KeyCode::Control))
									scaleDifference = roundf(scaleDifference / m_scaleSnap) * m_scaleSnap;

								meshRenderer->meshTransform.scale = selectedMeshRendererTransform.scale + selectedMeshRendererTransform.scale * scaleDifference;
							}
							else
							{
								const Vector3& cameraPosition = m_editorCamera.GlobalPosition();

								Vector3 mouseHoldTarget = m_editorCamera.ScreenToSpace(landmarkMousePosHeld);
								Vector3 mouseHoldDirection = (mouseHoldTarget - cameraPosition).GetNormalized();

								Vector3 target = m_editorCamera.ScreenToSpace({ mousePos.x, mousePos.y });
								Vector3 direction = (target - cameraPosition).GetNormalized();

								Vector3 camForward = m_editorCamera.GlobalRotation() * Vector3::Forward();

								auto translationOnPlane = [&](const Plane& plane) -> Vector3
								{
									Vector3 landmarkMousePosHeld3D;
									if (Ray(cameraPosition, mouseHoldDirection).CastBidirectional(plane, &landmarkMousePosHeld3D) == true)
									{
										Vector3 newPos;
										if (Ray(cameraPosition, direction).CastBidirectional(plane, &newPos) == true)
											return newPos - landmarkMousePosHeld3D;
									}

									return {};
								};

								Vector3 tmpPos = selectedComponentGlobalPositionHeld;
								Quaternion tmpRot = selectedComponentGlobalRotationHeld * selectedMeshRendererTransform.GetRotation();

								Vector3 right = Vector3(tmpRot * Vector3::Right()).GetNormalized();
								Vector3 up = Vector3(tmpRot * Vector3::Up()).GetNormalized();
								Vector3 forward = Vector3(tmpRot * Vector3::Forward()).GetNormalized();

								Plane rightPlane(right, tmpPos);
								Plane upPlane(up, tmpPos);
								Plane forwardPlane(forward, tmpPos);

								auto setScale = [&](const Vector3& direction, const Vector3& scaleDirection, const Plane& plane1, const Plane& plane2)
								{
									float dot;

									if (fabs(plane1.normal.Dot(camForward)) > fabs(plane2.normal.Dot(camForward)))
										dot = translationOnPlane(plane1).Dot(direction);
									else
										dot = translationOnPlane(plane2).Dot(direction);

									if (InputManager::GetKeyDown(KeyCode::Control))
										dot = roundf(dot / m_scaleSnap) * m_scaleSnap;

									meshRenderer->meshTransform.scale = selectedMeshRendererTransform.scale + dot * scaleDirection;
								};

								if (landmarkXHeld == true)
									setScale(right, Vector3::Right(), forwardPlane, upPlane);
								else if (landmarkYHeld == true)
									setScale(up, Vector3::Up(), forwardPlane, rightPlane);
								else if (landmarkZHeld == true)
									setScale(forward, Vector3::Forward(), upPlane, rightPlane);
							}
						}
					}
				}
			}
		}
	}

	void EditorApplication::UpdateCameraPosition()
	{
		static MousePos clickPos;

		static Quaternion currentRotation;
		static Vector2 currentMouseRotation;
		static Vector2 newRotation;

		static MousePos positionOutCounter{};

		static Quaternion targetRotation;
		static Quaternion beginRotation;
		static Vector3 targetPosition;
		static float targetDistance;
		static bool targetReached{ true };

		if (targetReached == true)
		{
			auto& window = m_editWin->GetGameWindow();

			auto mousePos32 = InputManager::GetMouseRelativePosition(*window);

			if (InputManager::GetMousePressed(MouseButton::Right))
			{
				positionOutCounter = {};
				currentRotation = m_editorCamera.GlobalRotation();
				clickPos = mousePos32;
			}

			if (clickPos.x < 0 || clickPos.x > window->GetWidth() ||
				clickPos.y < 0 || clickPos.y > window->GetHeight())
				return;

			if (InputManager::GetMouseReleased(MouseButton::Right))
				currentMouseRotation = newRotation + currentMouseRotation;

			if (InputManager::GetMouseDown(MouseButton::Right))
			{
				{
					MousePos newPos = mousePos32;

					if (mousePos32.x < 0)
					{
						newPos.x = window->GetWidth();
						positionOutCounter.x--;
					}
					else if (mousePos32.x > window->GetWidth())
					{
						newPos.x = 0;
						positionOutCounter.x++;
					}

					if (mousePos32.y < 0)
					{
						newPos.y = window->GetHeight();
						positionOutCounter.y--;
					}
					else if (mousePos32.y > window->GetHeight())
					{
						newPos.y = 0;
						positionOutCounter.y++;
					}

					if (mousePos32.x != newPos.x || mousePos32.y != newPos.y)
						InputManager::SetMouseRelativePosition(newPos, *window);

					mousePos32.x += positionOutCounter.x * window->GetWidth();
					mousePos32.y += positionOutCounter.y * window->GetHeight();
				}

				Vector2 newRotation(
					static_cast<float>(mousePos32.x - clickPos.x) / static_cast<float>(window->GetWidth()),
					static_cast<float>(mousePos32.y - clickPos.y) / static_cast<float>(window->GetHeight())
				);

				m_editorCamera.SetGlobalRotation(
					Quaternion(-newRotation.x * m_editorCameraRotationSpeed, Vector3(0.0f, 1.0f, 0.0f)) *
					currentRotation *
					Quaternion(-newRotation.y * m_editorCameraRotationSpeed, Vector3(1.0f, 0.0f, 0.0f))
				);

				{
					float sprint = InputManager::GetKeyDown(KeyCode::Space) ? 2.25f : 1.0f;

					float speed = sprint * m_editorCameraTranslationSpeed * sdk::Time::UnscaledDeltaTime();

					if (InputManager::GetKeyDown(KeyCode::W))
						m_editorCamera.Translate_SelfSpace(speed * Vector3::Forward());
					if (InputManager::GetKeyDown(KeyCode::S))
						m_editorCamera.Translate_SelfSpace(-speed * Vector3::Forward());

					if (InputManager::GetKeyDown(KeyCode::D))
						m_editorCamera.Translate_SelfSpace(speed * Vector3::Right());
					if (InputManager::GetKeyDown(KeyCode::A))
						m_editorCamera.Translate_SelfSpace(-speed * Vector3::Right());

					if (InputManager::GetKeyDown(KeyCode::Q))
						m_editorCamera.Translate_SelfSpace(-speed * Vector3::Up());
					if (InputManager::GetKeyDown(KeyCode::E))
						m_editorCamera.Translate_SelfSpace(speed * Vector3::Up());
				}
			}

			if (InputManager::GetKeyPressed(KeyCode::J))
			{
				LOG_LOG("J");

				if (m_selectedComponent != nullptr)
				{
					LOG_LOG("focusing");
					Vector3 selectedCompPos = m_selectedComponent->GlobalPosition();
					Vector3 selectedCompToCam = m_editorCamera.GlobalPosition() - selectedCompPos;
					selectedCompToCam.Normalize();

					selectedCompToCam *= 6.0f;

					beginRotation = m_editorCamera.GlobalRotation();
					m_editorCamera.LookAt(selectedCompPos);
					targetRotation = m_editorCamera.GlobalRotation();
					m_editorCamera.SetGlobalRotation(beginRotation);

					targetPosition = selectedCompPos + selectedCompToCam;

					targetDistance = (targetPosition - m_editorCamera.GlobalPosition()).Norm();

					targetReached = false;
				}
			}
		}
		else
		{
			auto dampedString = [this](const Vector3& currentPos, const Vector3& targetPos, bool& targetReached, float springStrength = 3.0f) -> Vector3
			{
				Vector3 displacement = targetPos - currentPos;

				float displacementLength = displacement.Norm();

				if (displacementLength <= 0.25f)
				{
					targetReached = true;
					m_editorCamera.SetGlobalRotation(targetRotation);
					return currentPos;
				}

				float invDisplacementLength = 1.f / displacementLength;

				const float dampConstant = 0.000065f;

				float springMagitude = springStrength * displacementLength + dampConstant * invDisplacementLength;

				float scalar = std::min(invDisplacementLength * springMagitude * sdk::Time::UnscaledDeltaTime(), 1.f);
				displacement *= scalar;

				return currentPos + displacement;
			};

			float alpha = ((targetPosition - m_editorCamera.GlobalPosition()).Norm() / targetDistance) * 1.2f;

			m_editorCamera.SetGlobalRotation(Quaternion::SLerp(targetRotation, beginRotation, alpha));
			m_editorCamera.SetGlobalPosition(dampedString(m_editorCamera.GlobalPosition(), targetPosition, targetReached));
		}
	}
}
