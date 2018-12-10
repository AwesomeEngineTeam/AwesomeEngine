namespace aw
{
	namespace ph
	{
		inline SceneDescriptor::SceneDescriptor(SceneDescriptor&& other) : m_handle { std::move(other.m_handle) }
		{
		}

		inline SceneDescriptor::SceneDescriptor(const SceneDescriptor& other) : m_handle{ other.m_handle }
		{
		}

		inline SceneDescriptor::~SceneDescriptor()
		{
			Destroy();
		}

		inline physx::PxSceneDesc& SceneDescriptor::Get()
		{
			return m_handle;
		}

		inline const physx::PxSceneDesc& SceneDescriptor::Get() const
		{
			return m_handle;
		}

		inline bool SceneDescriptor::IsValid() const
		{
			return m_handle.isValid();
		}

		inline void SceneDescriptor::Destroy()
		{
		}

		inline SceneDescriptor& SceneDescriptor::operator=(SceneDescriptor&& rhs)
		{
			Destroy();

			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline SceneDescriptor& SceneDescriptor::operator=(const SceneDescriptor& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;

			return *this;
		}
	}
}