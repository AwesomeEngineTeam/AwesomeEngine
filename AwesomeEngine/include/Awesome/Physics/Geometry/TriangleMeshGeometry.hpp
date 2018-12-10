#pragma once

#ifndef AWESOME_PHYSICS_TRIANGLE_MESH_GEOMETRY_GUARD
#define AWESOME_PHYSICS_TRIANGLE_MESH_GEOMETRY_GUARD

#include <memory>

#include <physX/geometry/PxTriangleMeshGeometry.h>

#include <Awesome/Physics/Geometry/Geometry.hpp>
#include <Awesome/Physics/Mesh/MeshScale.hpp>
#include <Awesome/Physics/Mesh/TriangleMesh.hpp>

namespace aw
{
	namespace ph
	{
		class TriangleMeshGeometry : public Geometry
		{
			friend class Shape;
			physx::PxTriangleMeshGeometry m_handle;
			std::shared_ptr<TriangleMesh> m_sharedMesh;

		public:
			TriangleMeshGeometry() = default;
			inline TriangleMeshGeometry(std::shared_ptr<TriangleMesh>  mesh, const MeshScale& scale = MeshScale());
			inline TriangleMeshGeometry(TriangleMeshGeometry&& other);
			inline TriangleMeshGeometry(const TriangleMeshGeometry& other);
			~TriangleMeshGeometry() = default;

			inline GeometryType GetType() const override;

			inline physx::PxGeometry& Get() override;
			inline const physx::PxGeometry& Get() const override;

			inline bool IsValid() const override;

			inline MeshScale& Scale();
			inline const MeshScale& Scale() const;
			inline std::shared_ptr<TriangleMesh> Mesh();
			inline const std::shared_ptr<TriangleMesh> Mesh() const;

			inline TriangleMeshGeometry& operator=(TriangleMeshGeometry&& rhs);
			inline TriangleMeshGeometry& operator=(const TriangleMeshGeometry& rhs);
		};
	}
}

#include <Awesome/Physics/Geometry/TriangleMeshGeometry.inl>

#endif // GUARD