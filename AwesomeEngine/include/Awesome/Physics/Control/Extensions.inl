namespace aw
{
	namespace ph
	{
		inline Extensions::Extensions(Extensions&& other) : m_isInit{ other.m_isInit }
		{
			other.m_isInit = false;
		}

		inline Extensions::~Extensions()
		{
			Destroy();
		}

		inline Extensions& Extensions::operator=(Extensions&& other)
		{
			Destroy();

			m_isInit = other.m_isInit;
			other.m_isInit = false;

			return *this;
		}
	}
}