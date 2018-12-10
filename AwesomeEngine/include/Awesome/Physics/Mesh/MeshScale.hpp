#pragma once

#ifndef AWESOME_PHYSICS_MESH_SCALE_GUARD
#define AWESOME_PHYSICS_MESH_SCALE_GUARD

#include <physX/geometry/PxMeshScale.h>

#include <Awesome/Maths/Vector3.hpp>
#include <Awesome/Maths/Quaternion.hpp>

namespace aw
{
	namespace ph
	{
		class MeshScale
		{
			physx::PxMeshScale m_handle;

		public:
			MeshScale() = default;
			inline MeshScale(float scale);
			inline MeshScale(const Vector3& scale, const Quaternion& rotation = Quaternion::Identity());
			inline MeshScale(MeshScale&& other);
			inline MeshScale(const MeshScale& other);
			~MeshScale() = default;

			inline const physx::PxMeshScale& Get() const;

			inline Vector3& Scale();
			inline const Vector3& Scale() const;

			inline Quaternion GetRotation() const;
			inline void SetRotation(const Quaternion& rotation);

			inline MeshScale& operator=(MeshScale&& rhs);
			inline MeshScale& operator=(const MeshScale& rhs);
		};
	}
}

#include <Awesome/Physics/Mesh/MeshScale.inl>

#endif // GUARD