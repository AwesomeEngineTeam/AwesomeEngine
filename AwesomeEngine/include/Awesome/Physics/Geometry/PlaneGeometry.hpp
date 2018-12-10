#pragma once

#ifndef AWESOME_PHYSICS_PLANE_GEOMETRY_GUARD
#define AWESOME_PHYSICS_PLANE_GEOMETRY_GUARD

#include <physX/geometry/PxPlaneGeometry.h>

#include <Awesome/Physics/Geometry/Geometry.hpp>

namespace aw
{
	namespace ph
	{
		class PlaneGeometry : public Geometry
		{
			friend class Shape;
			physx::PxPlaneGeometry m_handle;

		public:
			PlaneGeometry() = default;
			inline PlaneGeometry(PlaneGeometry&& other);
			inline PlaneGeometry(const PlaneGeometry& other);
			~PlaneGeometry() = default;

			inline GeometryType GetType() const override;

			inline physx::PxGeometry& Get() override;
			inline const physx::PxGeometry& Get() const override;

			inline bool IsValid() const override;

			inline PlaneGeometry& operator=(PlaneGeometry&& rhs);
			inline PlaneGeometry& operator=(const PlaneGeometry& rhs);
		};
	}
}

#include <Awesome/Physics/Geometry/PlaneGeometry.inl>

#endif // GUARD