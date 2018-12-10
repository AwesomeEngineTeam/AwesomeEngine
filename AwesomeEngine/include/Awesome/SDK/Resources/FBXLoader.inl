namespace aw
{
	namespace sdk
	{
		inline FBXLoader::Scene::Scene()
		{
			m_handle = FbxScene::Create(FBXLoader::GetInstance().m_fbxMgr, "FbxScene");

			FbxAxisSystem awAxis(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
			if (m_handle->GetGlobalSettings().GetAxisSystem() != awAxis)
				awAxis.ConvertScene(m_handle);
			m_handle->GetGlobalSettings().GetSystemUnit();

			if (m_handle->GetGlobalSettings().GetSystemUnit() != FbxSystemUnit::cm)
				FbxSystemUnit(1.0f).ConvertScene(m_handle);
		}

		inline FBXLoader::Scene::~Scene()
		{
			m_handle->Destroy();
		}

		inline FbxScene* FBXLoader::Scene::Get()
		{
			return m_handle;
		}

		inline FBXLoader::Importer::Importer(const std::string& filename, Scene& scene)
		{
			m_handle = FbxImporter::Create(FBXLoader::GetInstance().m_fbxMgr, "importer");
			ASSERT(!m_handle->Initialize(filename.c_str(), -1, FBXLoader::GetInstance().m_fbxMgr->GetIOSettings()), "FBX failed to create Importer");
			ASSERT(!m_handle->Import(scene.Get()), "FBX import failed!");
		}

		inline FBXLoader::Importer::~Importer()
		{
			m_handle->Destroy();
		}

		inline Model FBXLoader::Importer::Process()
		{
			ProcessNode(m_handle->GetScene()->GetRootNode());

			//return Model{ m_meshes.front(), m_materials.front() }; // TODO
			THROW("Model has changed, you can't do that anymore");
			return Model{};
		}

		inline FBXLoader::FBXLoader()
		{
			m_fbxMgr = FbxManager::Create();
			FbxIOSettings* settings = FbxIOSettings::Create(m_fbxMgr, IOSROOT);
			m_fbxMgr->SetIOSettings(settings);
		}

		inline FBXLoader::~FBXLoader()
		{
			m_fbxMgr->Destroy();
		}

		inline FBXLoader& FBXLoader::GetInstance()
		{
			static FBXLoader awFBXLoader;
			return awFBXLoader;
		}
	}
}