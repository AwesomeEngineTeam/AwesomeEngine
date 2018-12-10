#pragma once

#ifndef AWESOME_PHYSICS_CAPSULE_GEOMETRY_GUARD
#define AWESOME_PHYSICS_CAPSULE_GEOMETRY_GUARD

#include <physX/geometry/PxCapsuleGeometry.h>

#include <Awesome/Physics/Geometry/Geometry.hpp>

namespace aw
{
	namespace ph
	{
		class CapsuleGeometry : public Geometry
		{
			friend class Shape;
			physx::PxCapsuleGeometry m_handle;

		public:
			inline CapsuleGeometry(float radius = 0.5f, float halfHeight = 0.5f);
			inline CapsuleGeometry(CapsuleGeometry&& other);
			inline CapsuleGeometry(const CapsuleGeometry& other);
			~CapsuleGeometry() = default;

			inline GeometryType GetType() const override;

			inline physx::PxGeometry& Get() override;
			inline const physx::PxGeometry& Get() const override;

			inline bool IsValid() const override;

			inline float& Radius();
			inline float Radius() const;
			inline float& HalfHeight();
			inline float HalfHeight() const;

			inline CapsuleGeometry& operator=(CapsuleGeometry&& rhs);
			inline CapsuleGeometry& operator=(const CapsuleGeometry& rhs);
		};
	}
}

#include <Awesome/Physics/Geometry/CapsuleGeometry.inl>

#endif // GUARD