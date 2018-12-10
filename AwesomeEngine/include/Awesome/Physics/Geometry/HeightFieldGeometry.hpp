#pragma once

#ifndef AWESOME_PHYSICS_HEIGHT_FIELD_GEOMETRY_GUARD
#define AWESOME_PHYSICS_HEIGHT_FIELD_GEOMETRY_GUARD

#include <memory>

#include <physX/geometry/PxHeightFieldGeometry.h>

#include <Awesome/Maths/Vector3.hpp>

#include <Awesome/Physics/Geometry/Geometry.hpp>
#include <Awesome/Physics/Mesh/HeightField.hpp>

namespace aw
{
	namespace ph
	{
		class HeightFieldGeometry : public Geometry
		{
			friend class Shape;
			physx::PxHeightFieldGeometry m_handle;
			std::shared_ptr<HeightField> m_sharedMesh;

		public:
			HeightFieldGeometry() = default;
			inline HeightFieldGeometry(std::shared_ptr<HeightField> mesh, float heightScale, float rowScale, float columnScale);
			inline HeightFieldGeometry(std::shared_ptr<HeightField> mesh, Vector3 scale);
			inline HeightFieldGeometry(HeightFieldGeometry&& other);
			inline HeightFieldGeometry(const HeightFieldGeometry& other);
			~HeightFieldGeometry() = default;

			inline GeometryType GetType() const override;

			inline physx::PxGeometry& Get() override;
			inline const physx::PxGeometry& Get() const override;

			inline bool IsValid() const override;

			inline float& HeightScale();
			inline float HeightScale() const;
			inline float& RowScale();
			inline float RowScale() const;
			inline float& ColumnScale();
			inline float ColumnScale() const;
			inline std::shared_ptr<HeightField> Mesh();
			inline const std::shared_ptr<HeightField> Mesh() const;

			inline HeightFieldGeometry& operator=(HeightFieldGeometry&& rhs);
			inline HeightFieldGeometry& operator=(const HeightFieldGeometry& rhs);
		};
	}
}

#include <Awesome/Physics/Geometry/HeightFieldGeometry.inl>

#endif // GUARD