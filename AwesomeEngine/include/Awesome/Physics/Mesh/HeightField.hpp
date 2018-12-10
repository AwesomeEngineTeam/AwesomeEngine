#pragma once

#ifndef AWESOME_PHYSICS_HEIGHT_FIELD_GUARD
#define AWESOME_PHYSICS_HEIGHT_FIELD_GUARD

#include <physX/geometry/PxHeightField.h>

#include <Awesome/Physics/Config.hpp>
#include <Awesome/Physics/Descriptor/HeightFieldDescriptor.hpp>

namespace aw
{
	namespace ph
	{
		class HeightField
		{
			physx::PxHeightField* m_handle = nullptr;

		public:
			HeightField() = default;
			inline HeightField(const HeightFieldDescriptor& desc);
			inline HeightField(HeightField&& other);
			HeightField(const HeightField& other) = delete;
			inline ~HeightField();

			AWESOME_PHYSICS_API void Create(const HeightFieldDescriptor& desc);
			inline void Destroy();

			inline physx::PxHeightField* Get();
			inline const physx::PxHeightField* Get() const;

			inline HeightField& operator=(HeightField&& rhs);
			HeightField& operator=(const HeightField& rhs) = delete;
		};
	}
}

#include <Awesome/Physics/Mesh/HeightField.inl>

#endif // GUARD