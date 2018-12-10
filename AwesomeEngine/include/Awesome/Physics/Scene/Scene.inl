namespace aw
{
	namespace ph
	{
		inline Scene::Scene(const SceneDescriptor& sceneDesc)
		{
			Create(sceneDesc);
		}

		inline Scene::Scene(Scene&& other) : m_handle{other.m_handle}
		{
			other.m_handle = nullptr;
		}

		inline Scene::~Scene()
		{
			Destroy();
		}

		inline void Scene::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline void Scene::AddActor(Actor& actor)
		{
			m_handle->addActor(*(actor.GetAsActor()));
		}

		inline void Scene::RemoveActor(Actor& actor)
		{
			m_handle->removeActor(*(actor.GetAsActor()));
		}

		inline void Scene::Simulate(float elapsedTime)
		{
			m_handle->simulate(elapsedTime);
		}

		inline bool Scene::FetchResult(bool block)
		{
			return m_handle->fetchResults(block);
		}

		inline physx::PxScene& Scene::GetHandle()
		{
			ASSERT(!m_handle, "Handle is nullptr");
			
			return *m_handle;
		}

		inline Scene& Scene::operator=(Scene&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}