namespace aw
{
	namespace ph
	{
		inline Foundation::Foundation(AllocatorCallback& allocCallback, DebugCallback& debCallback)
		{
			Create(allocCallback, debCallback);
		}

		inline Foundation::Foundation(Foundation&& other) : m_handle{ other.m_handle }
		{
			other.m_handle = nullptr;
		}

		inline Foundation::~Foundation()
		{
			Destroy();
		}

		inline physx::PxFoundation* Foundation::Get()
		{
			return m_handle;
		}

		inline const physx::PxFoundation* Foundation::Get() const
		{
			return m_handle;
		}

		inline void Foundation::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline Foundation& Foundation::operator=(Foundation&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}