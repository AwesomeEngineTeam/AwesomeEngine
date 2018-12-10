#pragma once

#ifndef AWESOME_PHYSICS_BOX_GEOMETRY_GUARD
#define AWESOME_PHYSICS_BOX_GEOMETRY_GUARD

#include <physX/geometry/PxBoxGeometry.h>

#include <Awesome/Maths/Vector3.hpp>
#include <Awesome/Physics/Geometry/Geometry.hpp>

namespace aw
{
	namespace ph
	{
		class BoxGeometry : public Geometry
		{
			friend class Shape;
			physx::PxBoxGeometry m_handle;

		public:
			inline BoxGeometry(Vector3 halfScale = Vector3(0.5f));
			inline BoxGeometry(BoxGeometry&& other);
			inline BoxGeometry(const BoxGeometry& other);
			~BoxGeometry() = default;

			inline GeometryType GetType() const override;

			inline physx::PxGeometry& Get() override;
			inline const physx::PxGeometry& Get() const override;

			inline bool IsValid() const override;

			inline Vector3 GetHalfScale() const;
			inline void SetHalfScale(Vector3 halfScale);

			inline BoxGeometry& operator=(BoxGeometry&& rhs);
			inline BoxGeometry& operator=(const BoxGeometry& rhs);
		};
	}
}

#include <Awesome/Physics/Geometry/BoxGeometry.inl>

#endif // GUARD