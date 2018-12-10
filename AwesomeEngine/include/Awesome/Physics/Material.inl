namespace aw
{
	namespace ph
	{
		inline Material::Material(float staticFriction, float dynamicFriction, float restitution) :
			staticFriction{ staticFriction },
			dynamicFriction{ dynamicFriction },
			restitution{ restitution }
		{
			Create(staticFriction, dynamicFriction, restitution);
		}

		inline Material::Material(Vector3 characteristic) :
			staticFriction{ characteristic.x.load() },
			dynamicFriction{ characteristic.y.load() },
			restitution{ characteristic.z.load() }
		{
			Create(characteristic);
		}

		inline Material::Material(Material&& other) : m_handle{ other.m_handle }
		{
			other.m_handle = nullptr;
		}

		inline Material::Material(const Material& other) : Material(other.staticFriction, other.dynamicFriction, other.restitution)
		{
		}

		inline Material::~Material()
		{
			Destroy();
		}

		/*inline float Material::GetStaticFriction() const
		{
			return m_handle->getStaticFriction();
		}

		inline void Material::SetStaticFriction(float f)
		{
			m_handle->setStaticFriction(f);
		}

		inline float Material::GetDynamicFriction() const
		{
			return m_handle->getDynamicFriction();
		}

		inline void Material::SetDynamicFriction(float f)
		{
			m_handle->setDynamicFriction(f);
		}

		inline float Material::GetRestitution() const
		{
			return m_handle->getRestitution();
		}

		inline void Material::SetRestitution(float f)
		{
			m_handle->setRestitution(f);
		}*/

		//inline physx::PxMaterial* Material::Get()
		//{
		//	return m_handle;
		//}

		//inline const physx::PxMaterial* Material::Get() const
		//{
		//	return m_handle;
		//}

		void Material::Update()
		{
			m_handle->setStaticFriction(staticFriction);
			m_handle->setDynamicFriction(dynamicFriction);
			m_handle->setRestitution(restitution);
		}

		inline void Material::Create(Vector3 characteristic)
		{
			Create(characteristic.x, characteristic.y, characteristic.z);
		}

		inline void Material::Destroy()
		{
			if (m_handle)
				m_handle->release();

			m_handle = nullptr;
		}

		inline Material& Material::operator=(Material&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}