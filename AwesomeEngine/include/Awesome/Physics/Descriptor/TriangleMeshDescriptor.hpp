#pragma once

#ifndef AWESOME_PHYSICS_TRIANGLE_MESH_DESCRIPTOR_GUARD
#define AWESOME_PHYSICS_TRIANGLE_MESH_DESCRIPTOR_GUARD

#include <vector>

#include <physX/cooking/PxTriangleMeshDesc.h>

#include <Awesome/Maths/Vector3.hpp>

namespace aw
{
	namespace ph
	{
		class TriangleMeshDescriptor
		{
			physx::PxTriangleMeshDesc m_handle;

		public:
			TriangleMeshDescriptor() = default;
			inline TriangleMeshDescriptor(const std::vector<Vector3>& vertices, const std::vector<uint32>& indices);
			inline TriangleMeshDescriptor(TriangleMeshDescriptor&& other);
			inline TriangleMeshDescriptor(const TriangleMeshDescriptor& other);
			~TriangleMeshDescriptor() = default;

			inline const physx::PxTriangleMeshDesc& Get() const;

			inline bool IsValid();

			inline void Create(const std::vector<Vector3>& vertices, const std::vector<uint32>& indices);


			inline TriangleMeshDescriptor& operator=(TriangleMeshDescriptor&& rhs);
			inline TriangleMeshDescriptor& operator=(const TriangleMeshDescriptor& rhs);
		};
	}
}

#include <Awesome/Physics/Descriptor/TriangleMeshDescriptor.inl>

#endif // GUARD