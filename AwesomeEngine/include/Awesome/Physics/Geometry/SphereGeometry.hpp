#pragma once

#ifndef AWESOME_PHYSICS_SPHERE_GEOMETRY_GUARD
#define AWESOME_PHYSICS_SPHERE_GEOMETRY_GUARD

#include <physX/geometry/PxSphereGeometry.h>

#include <Awesome/Maths/Vector3.hpp>
#include <Awesome/Physics/Geometry/Geometry.hpp>

namespace aw
{
	namespace ph
	{
		class SphereGeometry : public Geometry
		{
			friend class Shape;
			physx::PxSphereGeometry m_handle;

		public:
			inline SphereGeometry(float radius = 0.5f);
			inline SphereGeometry(SphereGeometry&& other);
			inline SphereGeometry(const SphereGeometry& other);
			~SphereGeometry() = default;

			inline GeometryType GetType() const override;

			inline physx::PxGeometry& Get() override;
			inline const physx::PxGeometry& Get() const override;

			inline bool IsValid() const override;

			inline float& Radius();
			inline float Radius() const;

			inline SphereGeometry& operator=(SphereGeometry&& rhs);
			inline SphereGeometry& operator=(const SphereGeometry& rhs);
		};
	}
}

#include <Awesome/Physics/Geometry/SphereGeometry.inl>

#endif // GUARD