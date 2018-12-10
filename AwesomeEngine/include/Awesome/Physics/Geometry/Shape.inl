namespace aw
{
	namespace ph
	{
		inline Shape::Shape(physx::PxShape* shape)
		{
			Set(shape);
		}

		inline Shape::Shape(Shape&& other) : m_handle{ std::move(other.m_handle) }
		{
		}

		inline Shape::Shape(const Shape& other) : m_handle{ other.m_handle }
		{
		}

		inline physx::PxShape* Shape::Get()
		{
			return m_handle;
		}

		inline const physx::PxShape* Shape::Get() const
		{
			return m_handle;
		}

		inline GeometryType Shape::GetGeometryType() const
		{
			return static_cast<GeometryType>(m_handle->getGeometryType());
		}

		inline void Shape::SetGeometry(const Geometry& geo)
		{
			m_handle->setGeometry(geo.Get());
		}

		inline bool Shape::GetPlaneGeometry(PlaneGeometry& geo) const
		{
			return m_handle->getPlaneGeometry(geo.m_handle);
		}

		inline bool Shape::GetBoxGeometry(BoxGeometry& geo) const
		{
			return m_handle->getBoxGeometry(geo.m_handle);
		}

		inline bool Shape::GetSphereGeometry(SphereGeometry& geo) const
		{
			return m_handle->getSphereGeometry(geo.m_handle);
		}

		inline bool Shape::GetCapsuleGeometry(CapsuleGeometry& geo) const
		{
			return m_handle->getCapsuleGeometry(geo.m_handle);
		}

		inline bool Shape::GetConvexMeshGeometry(ConvexMeshGeometry& geo) const
		{
			return m_handle->getConvexMeshGeometry(geo.m_handle);
		}

		inline bool Shape::GetHeightFieldGeometry(HeightFieldGeometry& geo) const
		{
			return m_handle->getHeightFieldGeometry(geo.m_handle);
		}

		inline physx::PxMaterial* Shape::GetMaterial()
		{
			auto temp = m_handle->getMaterialFromInternalFaceIndex(0u);
			return temp;
		}

		inline bool Shape::IsTrigger() const
		{
			return m_handle->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE) && !m_handle->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE);
		}

		inline void Shape::SetTrigger(bool b)
		{
			if (b)
			{
				m_handle->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !b);
				m_handle->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, b);
			}
			else // Do this because of PhysX implementation...
			{
				m_handle->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, b);
				m_handle->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !b);
			}
		}

		inline uint32 Shape::GetFilterGroup() const
		{
			return m_handle->getSimulationFilterData().word0;
		}

		inline void Shape::SetFilterGroup(uint32 filterGroup)
		{
			physx::PxFilterData filterData = m_handle->getSimulationFilterData();
			filterData.word0 = filterGroup;

			m_handle->setSimulationFilterData(filterData);
		}

		inline uint32 Shape::GetFilterMask() const
		{
			return m_handle->getSimulationFilterData().word1;
		}

		inline void Shape::SetFilterMask(uint32 filterMask)
		{
			physx::PxFilterData filterData = m_handle->getSimulationFilterData();
			filterData.word1 = filterMask;

			m_handle->setSimulationFilterData(filterData);
		}

		inline void Shape::SetFiltering(uint32 filterGroup, uint32 filterMask)
		{
			physx::PxFilterData filterData;
			filterData.word0 = filterGroup;
			filterData.word1 = filterMask;

			m_handle->setSimulationFilterData(filterData);
		}

		inline bool Shape::IsValid() const
		{
			return m_handle;
		}

		inline void Shape::Set(physx::PxShape* shape)
		{
			m_handle = shape;
		}

		inline Shape& Shape::operator=(Shape&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline Shape& Shape::operator=(const Shape& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}

		inline bool Shape::operator==(const Shape& rhs)
		{
			return m_handle == rhs.m_handle;
		}

		inline bool Shape::operator!=(const Shape& rhs)
		{
			return m_handle != rhs.m_handle;
		}
	}
}