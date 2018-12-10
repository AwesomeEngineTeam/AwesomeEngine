namespace aw
{
	namespace ph
	{
		inline Instance::Instance(TolerancesScale scale)
		{
			Create(scale);
		}

		inline Instance::Instance(Instance&& other) : m_handle{ other.m_handle }
		{
			other.m_handle = nullptr;
		}

		inline Instance::~Instance()
		{
			Destroy();
		}

		inline physx::PxPhysics* Instance::Get()
		{
			return m_handle;
		}

		inline const physx::PxPhysics* Instance::Get() const
		{
			return m_handle;
		}

		inline TolerancesScale Instance::GetTolerancesScale() const
		{
			return m_handle ? reinterpret_cast<const TolerancesScale&>(m_handle->getTolerancesScale()) : TolerancesScale();
		}

		inline void Instance::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline Instance& Instance::operator=(Instance&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}