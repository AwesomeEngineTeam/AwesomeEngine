#pragma once

#ifndef AWESOME_PHYSICS_SHAPE_GUARD
#define AWESOME_PHYSICS_SHAPE_GUARD

#include <physX/PxShape.h>
#include <physX/PxFiltering.h>

#include <Awesome/Physics/Config.hpp>
#include <Awesome/Physics/Geometry/Geometry.hpp>
#include <Awesome/Physics/Geometry/PlaneGeometry.hpp>
#include <Awesome/Physics/Geometry/BoxGeometry.hpp>
#include <Awesome/Physics/Geometry/SphereGeometry.hpp>
#include <Awesome/Physics/Geometry/CapsuleGeometry.hpp>
#include <Awesome/Physics/Geometry/PlaneGeometry.hpp>
#include <Awesome/Physics/Geometry/ConvexMeshGeometry.hpp>
#include <Awesome/Physics/Geometry/TriangleMeshGeometry.hpp>
#include <Awesome/Physics/Geometry/HeightFieldGeometry.hpp>
#include <Awesome/Physics/Material.hpp>

namespace aw
{
	namespace ph
	{
		class Rigidbody;

		class Shape
		{
			physx::PxShape* m_handle = nullptr;

		public:
			Shape() = default;
			inline Shape(physx::PxShape* shape);
			inline Shape(Shape&& other);
			inline Shape(const Shape& other);
			~Shape() = default;

			inline physx::PxShape* Get();
			inline const physx::PxShape* Get() const;

			inline GeometryType GetGeometryType() const;
			inline void SetGeometry(const Geometry& geo);
			inline bool GetPlaneGeometry(PlaneGeometry& geo) const;
			inline bool GetBoxGeometry(BoxGeometry& geo) const;
			inline bool GetSphereGeometry(SphereGeometry& geo) const;
			inline bool GetCapsuleGeometry(CapsuleGeometry& geo) const;
			inline bool GetConvexMeshGeometry(ConvexMeshGeometry& geo) const;
			inline bool GetHeightFieldGeometry(HeightFieldGeometry& geo) const;
			inline physx::PxMaterial* GetMaterial();

			inline bool IsTrigger() const;
			inline void SetTrigger(bool b = true);

			inline uint32 GetFilterGroup() const;
			inline void SetFilterGroup(uint32 filterGroup);

			inline uint32 GetFilterMask() const;
			inline void SetFilterMask(uint32 filterMask);

			inline void SetFiltering(uint32 filterGroup, uint32 filterMask);

			inline bool IsValid() const;

			inline void Set(physx::PxShape* shape);
			AWESOME_PHYSICS_API void Create(Rigidbody& body, const Geometry& geo, const Material& mat);
			AWESOME_PHYSICS_API void Create(Rigidbody& body, const Geometry& geo, const std::vector<Material>& mats);

			inline Shape& operator=(Shape&& rhs);
			inline Shape& operator=(const Shape& rhs);

			inline bool operator==(const Shape& rhs);
			inline bool operator!=(const Shape& rhs);
		};
	}
}

#include <Awesome/Physics/Geometry/Shape.inl>

#endif // GUARD