#pragma once

#ifndef AWESOME_PHYSICS_MATERIAL_GUARD
#define AWESOME_PHYSICS_MATERIAL_GUARD

#include <physX/PxMaterial.h>

#include <Awesome/Maths/Vector3.hpp>

#include <Awesome/Physics/Config.hpp>

namespace aw
{
	namespace sdk
	{
		class Rigidbody;
	}

	namespace ph
	{
		class Material
		{
			friend aw::sdk::Rigidbody;
			friend class Rigidbody;
			friend class Shape;

			physx::PxMaterial* m_handle = nullptr;

			inline void Update();

		public:
			std::atomic<float> staticFriction;
			std::atomic<float> dynamicFriction;
			std::atomic<float> restitution;

			Material() = default;
			inline Material(float staticFriction, float dynamicFriction, float restitution);
			inline Material(Vector3 characteristic);
			inline Material(Material&& other);
			Material(const Material& other);
			inline ~Material();

			AWESOME_PHYSICS_API void Create(float staticFriction, float dynamicFriction, float restitution);
			inline void Create(Vector3 characteristic);
			inline void Destroy();

			inline Material& operator=(Material&& rhs);
			Material& operator=(const Material&) = delete;

			REFLECT_BEGIN(Material)
				REFLECT_MEMBER(staticFriction)
				REFLECT_MEMBER(dynamicFriction)
				REFLECT_MEMBER(restitution)
			REFLECT_END()
		};
	}
}

#include <Awesome/Physics/Material.inl>

#endif // GUARD