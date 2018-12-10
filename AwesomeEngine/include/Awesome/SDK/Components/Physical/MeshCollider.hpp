#pragma once

#ifndef AWESOME_SDK_MESH_COLLIDER_GUARD
#define AWESOME_SDK_MESH_COLLIDER_GUARD

#include <Awesome/Physics/ConvexMeshGeometry.hpp>

#include <Awesome/SDK/Components/Physical/Collider.hpp>

namespace aw
{
	namespace sdk
	{
		class MeshCollider : public Collider
		{
		public:
			inline MeshCollider(Actor* actor, const std::vector<Vector3>& vertices, const std::vector<uint32>& indices);
			~MeshCollider() override = default;
		};
	}

	template <>
	class TypeInfo<sdk::MeshCollider>
	{
	public:
		static const uint32 id = static_cast<uint32>(sdk::ComponentID::MeshCollider);
	};
}

#include <Awesome/SDK/Components/Physical/MeshCollider.inl>

#endif // GUARD