namespace aw
{
	namespace sdk
	{
		inline const Camera& RenderSystem::GetFirstCamera()
		{
			ASSERT(s_cameras.empty() == true, "There is no camera!");

			return *s_cameras.front();
		}

		//inline Camera& RenderSystem::GetFirstCamera()
		//{
		//	ASSERT(s_cameras.empty() == true, "There is no camera!");

		//	return *s_cameras.front();
		//}

		inline Framegraph& RenderSystem::GetFramegraph()
		{
			return s_framegraph;
		}

		inline RenderQueue& RenderSystem::GetOpaqueRenderQueue()
		{
			return s_opaqueRQ;
		}

		inline ResourceManager& RenderSystem::GetResourceManager()
		{
			return s_resourceManager;
		}

		inline Material& RenderSystem::GetMaterial(uint32 index)
		{
			return s_materials[index];
		}

		inline uint32 RenderSystem::RegisterMesh(Mesh* mesh)
		{
			for (uint64 i{ 0 }; i < s_meshes.size(); i++)
			{
				if (s_meshes[i] == mesh)
					return i;
			}

			s_meshes.push_back(mesh);

			return s_meshes.size() - 1;
		}

		inline Mesh* RenderSystem::GetMesh(uint32 index)
		{
			return s_meshes[index];
		}

		inline uint32 RenderSystem::RegisterTexture(SampledTextureHandle tex)
		{
			s_textures.push_back(tex);

			return s_textures.size() - 1;
		}

		inline void RenderSystem::UnRegisterTexture(uint32 textureIndex)
		{
			LOG_WARNING("The texture leaks here (not exactly, but still...)");
			//s_resourceManager.
			//s_textures[textureIndex].Destroy();
		}

		inline SampledTextureHandle RenderSystem::GetTexture(uint32 index)
		{
			return s_textures[index];
		}

		inline SampledTextureHandle RenderSystem::GetPresentImage()
		{
			return s_frameResourcesManager.GetImage(s_presentImageIndex);
		}

		inline void RenderSystem::InitModelUBD(uint64 size, uint64 countPerPool)
		{
			ClearModelUBD();

			s_modelUBDSize = size;
			s_modelUBDCountPerPool = countPerPool;

			s_modelUBDLayoutInfo = {};
			s_modelUBDLayoutInfo.AddBinding(vk::DescriptorType::eUniformBufferDynamic, vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 1);

			s_modelUBDLayout = Context::GetDevice().Get().createDescriptorSetLayoutUnique(
				s_modelUBDLayoutInfo.GetCreateInfo()
			);
		}

		inline void RenderSystem::ClearModelUBD()
		{
			s_modelUBDGroups.clear();
		}

		inline void RenderSystem::ReserveModelUBD(MeshRenderer* meshRenderer)
		{
			auto& group = FindOrCreateSuitableGroupModelUBD();

			meshRenderer->m_dynamicBlock = group.dynamicPool->Reserve();
			meshRenderer->m_geometryDset = group.descriptorSet.get();
		}

		inline void RenderSystem::ReserveModelUBD(Text2D* text2D)
		{
			auto& group = FindOrCreateSuitableGroupModelUBD();

			text2D->m_dynamicBlock = group.dynamicPool->Reserve();
			text2D->m_geometryDset = group.descriptorSet.get();
		}

		inline void RenderSystem::ReserveModelUBD(Billboard* billboard)
		{
			auto& group = FindOrCreateSuitableGroupModelUBD();

			billboard->m_dynamicBlock = group.dynamicPool->Reserve();
			billboard->m_geometryDset = group.descriptorSet.get();
		}

		inline uint32 RenderSystem::GetMeshIndex(Mesh* mesh)
		{
			for (uint32 i = 0; i < s_meshes.size(); i++)
			{
				if (mesh == s_meshes[i])
					return i;
			}

			return 0u;
		}
	}
}