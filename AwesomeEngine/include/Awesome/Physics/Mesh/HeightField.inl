namespace aw
{
	namespace ph
	{
		inline HeightField::HeightField(const HeightFieldDescriptor& desc)
		{
			Create(desc);
		}

		inline HeightField::HeightField(HeightField&& other) : m_handle{ other.m_handle }
		{
			other.m_handle = nullptr;
		}

		inline HeightField::~HeightField()
		{
			Destroy();
		}

		inline void HeightField::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline physx::PxHeightField* HeightField::Get()
		{
			return m_handle;
		}

		inline const physx::PxHeightField* HeightField::Get() const
		{
			return m_handle;
		}

		inline HeightField& HeightField::operator=(HeightField&& rhs)
		{
			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}