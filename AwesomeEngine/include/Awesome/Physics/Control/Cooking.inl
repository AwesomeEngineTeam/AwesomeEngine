namespace aw
{
	namespace ph
	{

		inline Cooking::Cooking(Cooking&& other) : m_handle{ other.m_handle }
		{
			other.m_handle = nullptr;
		}

		inline Cooking::~Cooking()
		{
			Destroy();
		}

		inline physx::PxCooking* Cooking::Get()
		{
			return m_handle;
		}

		inline const physx::PxCooking* Cooking::Get() const
		{
			return m_handle;
		}

		inline void Cooking::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline Cooking& Cooking::operator=(Cooking&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}