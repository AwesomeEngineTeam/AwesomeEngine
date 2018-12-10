#pragma once

#ifndef AWESOME_PHYSICS_GEOMETRY_GUARD
#define AWESOME_PHYSICS_GEOMETRY_GUARD

#include <utility>

#include <physX/geometry/PxGeometry.h>

namespace aw
{
	namespace ph
	{
		enum class GeometryType
		{
			Plane = physx::PxGeometryType::ePLANE,
			Box = physx::PxGeometryType::eBOX,
			Sphere = physx::PxGeometryType::eSPHERE,
			Capsule = physx::PxGeometryType::eCAPSULE,
			TriangleMesh = physx::PxGeometryType::eTRIANGLEMESH,
			ConvexMesh = physx::PxGeometryType::eCONVEXMESH,
			HeightField = physx::PxGeometryType::eHEIGHTFIELD,
			GeometryCount = physx::PxGeometryType::eGEOMETRY_COUNT
		};

		class Geometry
		{
		public:
			Geometry() = default;
			virtual ~Geometry() = default;

			virtual inline GeometryType GetType() const = 0;

			virtual physx::PxGeometry& Get() = 0;
			virtual const physx::PxGeometry& Get() const = 0;

			virtual bool IsValid() const = 0;
		};
	}
}

#endif // GUARD