#pragma once

#ifndef AWESOME_PHYSICS_RIGIDBODY_GUARD
#define AWESOME_PHYSICS_RIGIDBODY_GUARD

#include <vector>

#include <physX/PxRigidActor.h>
#include <physX/PxRigidDynamic.h>
#include <physX/extensions/PxShapeExt.h>

#include <Awesome/Maths/UniformTransform.hpp>

#include <Awesome/Physics/Config.hpp>
#include <Awesome/Physics/Scene/Actor.hpp>
#include <Awesome/Physics/Geometry/Shape.hpp>

namespace aw
{
	namespace sdk
	{
		class PhysicalComponent;
	}

	namespace ph
	{
		enum class ForceMode
		{
			Force = physx::PxForceMode::eFORCE,
			Impulse = physx::PxForceMode::eIMPULSE,
			VelocityChange = physx::PxForceMode::eVELOCITY_CHANGE,
			Acceleration = physx::PxForceMode::eACCELERATION,
		};

		class Rigidbody : public Actor
		{
		protected:
			physx::PxRigidActor* m_handle = nullptr;
			void* m_userData = nullptr;

		public:
			Rigidbody() = default;
			inline Rigidbody(Rigidbody&& other);
			Rigidbody(const Rigidbody&) = delete;
			inline virtual ~Rigidbody();

			inline physx::PxRigidActor* Get();
			inline const physx::PxRigidActor* Get() const;

			inline physx::PxActor* GetAsActor() override;
			inline const physx::PxActor* GetAsActor() const override;

			inline void SetGlobalTransform(TransformBase tr);

			AWESOME_PHYSICS_API std::vector<Shape> GetShapes() const;

			inline bool GetUseGravity() const;
			inline void SetUseGravity(bool b = true);
			inline bool IsActive() const;
			inline void SetActive(bool b = true);

			template <typename T>
			inline T* GetUserData();
			template <typename T>
			inline const T* GetUserData() const;

			inline sdk::PhysicalComponent* GetPhysicalComponent();
			inline const sdk::PhysicalComponent* GetPhysicalComponent() const;

			virtual void Create(const TransformBase& tr, void* userData) = 0;
			inline void Destroy();

			AWESOME_PHYSICS_API void ClearShapes();
			AWESOME_PHYSICS_API void AttachShape(Shape& shape);
			AWESOME_PHYSICS_API void DetachShape(Shape& shape);
			AWESOME_PHYSICS_API Shape CreateShape(const Geometry& geo, const Material& mat);

			AWESOME_PHYSICS_API UniformTransform FetchGlobalTransform(Shape& shape);

			inline Rigidbody& operator=(Rigidbody&& rhs);
			Rigidbody& operator=(const Rigidbody&) = delete;
		};
	}
}

#include <Awesome/Physics/Body/Rigidbody.inl>

#endif // GUARD