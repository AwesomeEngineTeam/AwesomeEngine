namespace aw
{
	namespace ph
	{
		inline HeightFieldSample::HeightFieldSample(HeightFieldSample&& other) : m_handle{ std::move(other.m_handle) }
		{
		}

		inline HeightFieldSample::HeightFieldSample(const HeightFieldSample& other) : m_handle{ other.m_handle }
		{

		}

		inline const physx::PxHeightFieldSample& HeightFieldSample::Get() const
		{
			return m_handle;
		}

		inline uint16& HeightFieldSample::Height()
		{
			return reinterpret_cast<uint16&>(m_handle.height);
		}

		inline uint16 HeightFieldSample::Height() const
		{
			return m_handle.height;
		}

		inline HeightFieldSample& HeightFieldSample::operator=(HeightFieldSample&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline HeightFieldSample& HeightFieldSample::operator=(const HeightFieldSample& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}