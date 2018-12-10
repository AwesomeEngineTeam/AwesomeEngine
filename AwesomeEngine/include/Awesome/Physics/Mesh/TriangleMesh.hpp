#pragma once

#ifndef AWESOME_PHYSICS_TRIANGLE_MESH_GUARD
#define AWESOME_PHYSICS_TRIANGLE_MESH_GUARD

#include <physX/geometry/PxTriangleMesh.h>

#include <Awesome/Physics/Config.hpp>
#include <Awesome/Physics/Descriptor/TriangleMeshDescriptor.hpp>

namespace aw
{
	namespace ph
	{
		class TriangleMesh
		{
			physx::PxTriangleMesh* m_handle = nullptr;

		public:
			TriangleMesh() = default;
			inline TriangleMesh(const TriangleMeshDescriptor& desc);
			TriangleMesh(const TriangleMesh&) = delete;
			inline TriangleMesh(TriangleMesh&& other);
			inline ~TriangleMesh();

			inline physx::PxTriangleMesh* Get();
			inline const physx::PxTriangleMesh* Get() const;

			AWESOME_PHYSICS_API void Create(const TriangleMeshDescriptor& desc);
			inline void Destroy();

			TriangleMesh& operator=(const TriangleMesh&) = delete;
			inline TriangleMesh& operator=(TriangleMesh&& rhs);
		};
	}
}

#include <Awesome/Physics/Mesh/TriangleMesh.inl>

#endif // GUARD