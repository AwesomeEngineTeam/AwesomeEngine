namespace aw
{
	namespace ph
	{
		inline  Pvd::Pvd(Pvd&& other) : m_handle{other.m_handle}
		{
			other.m_handle = nullptr;
		}

		inline Pvd::~Pvd()
		{
			Destroy();
		}

		inline physx::PxPvd* Pvd::Get()
		{
			return m_handle;
		}

		inline const physx::PxPvd* Pvd::Get() const
		{
			return m_handle;
		}

		inline void Pvd::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline Pvd& Pvd::operator=(Pvd&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}