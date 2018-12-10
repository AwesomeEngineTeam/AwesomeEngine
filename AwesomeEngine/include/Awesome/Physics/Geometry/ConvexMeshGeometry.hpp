#pragma once

#ifndef AWESOME_PHYSICS_CONVEX_PLANE_GEOMETRY_GUARD
#define AWESOME_PHYSICS_CONVEX_PLANE_GEOMETRY_GUARD

#include <memory>

#include <physX/geometry/PxConvexMeshGeometry.h>

#include <Awesome/Physics/Geometry/Geometry.hpp>
#include <Awesome/Physics/Mesh/ConvexMesh.hpp>
#include <Awesome/Physics/Mesh/MeshScale.hpp>

namespace aw
{
	namespace ph
	{
		class ConvexMeshGeometry : public Geometry
		{
			friend class Shape;
			physx::PxConvexMeshGeometry m_handle;
			std::shared_ptr<ConvexMesh> m_sharedMesh;

		public:
			ConvexMeshGeometry() = default;
			inline ConvexMeshGeometry(std::shared_ptr<ConvexMesh> mesh, const MeshScale& scale = MeshScale());
			inline ConvexMeshGeometry(ConvexMeshGeometry&& other);
			inline ConvexMeshGeometry(const ConvexMeshGeometry& other);
			~ConvexMeshGeometry() = default;

			inline GeometryType GetType() const override;

			inline physx::PxGeometry& Get() override;
			inline const physx::PxGeometry& Get() const override;

			inline bool IsValid() const override;

			inline MeshScale& Scale();
			inline const MeshScale& Scale() const;
			inline std::shared_ptr<ConvexMesh> Mesh();
			inline const std::shared_ptr<ConvexMesh> Mesh() const;

			inline ConvexMeshGeometry& operator=(ConvexMeshGeometry&& rhs);
			inline ConvexMeshGeometry& operator=(const ConvexMeshGeometry& rhs);
		};
	}
}

#include <Awesome/Physics/Geometry/ConvexMeshGeometry.inl>

#endif // GUARD