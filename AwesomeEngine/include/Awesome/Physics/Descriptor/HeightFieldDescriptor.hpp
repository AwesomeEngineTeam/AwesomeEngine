#pragma once

#ifndef AWESOME_PHYSICS_HEIGHT_FIELD_DESCRIPTOR_GUARD
#define AWESOME_PHYSICS_HEIGHT_FIELD_DESCRIPTOR_GUARD

#include <physX/geometry/PxHeightFieldDesc.h>

#include <Awesome/Core/Debug.hpp>

#include <Awesome/Physics/Descriptor/HeightFieldSample.hpp>

namespace aw
{
	namespace ph
	{
		class HeightFieldDescriptor
		{
			physx::PxHeightFieldDesc m_handle;

		public:
			HeightFieldDescriptor() = default;
			inline HeightFieldDescriptor(HeightFieldDescriptor&& other);
			inline HeightFieldDescriptor(const HeightFieldDescriptor& other);
			~HeightFieldDescriptor() = default;

			inline const physx::PxHeightFieldDesc& Get() const;

			inline bool IsValid() const;

			inline void Create(uint32 numCols, uint32 numRows, std::vector<HeightFieldSample> samples);

			inline HeightFieldDescriptor& operator=(HeightFieldDescriptor&& rhs);
			inline HeightFieldDescriptor& operator=(const HeightFieldDescriptor& rhs);
		};
	}
}

#include <Awesome/Physics/Descriptor/HeightFieldDescriptor.inl>

#endif // GUARD