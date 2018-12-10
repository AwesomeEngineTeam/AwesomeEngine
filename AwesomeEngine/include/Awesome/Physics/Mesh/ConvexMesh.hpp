#pragma once

#ifndef AWESOME_PHYSICS_CONVEX_MESH_GUARD
#define AWESOME_PHYSICS_CONVEX_MESH_GUARD

#include <physX/geometry/PxConvexMesh.h>

#include <Awesome/Physics/Config.hpp>
#include <Awesome/Physics/Descriptor/ConvexMeshDescriptor.hpp>

namespace aw
{
	namespace ph
	{
		class ConvexMesh
		{
			physx::PxConvexMesh* m_handle = nullptr;

		public:
			ConvexMesh() = default;
			inline ConvexMesh(const ConvexMeshDescriptor& desc);
			inline ConvexMesh(ConvexMesh&& other);
			ConvexMesh(const ConvexMesh&) = delete;
			inline ~ConvexMesh();

			inline physx::PxConvexMesh* Get();
			inline const physx::PxConvexMesh* Get() const;

			AWESOME_PHYSICS_API void Create(const ConvexMeshDescriptor& desc);
			inline void Destroy();

			inline ConvexMesh& operator=(ConvexMesh&& rhs);
			ConvexMesh& operator=(const ConvexMesh&) = delete;
		};
	}
}

#include <Awesome/Physics/Mesh/ConvexMesh.inl>

#endif // GUARD