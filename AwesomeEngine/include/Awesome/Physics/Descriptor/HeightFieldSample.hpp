#pragma once

#ifndef AWESOME_PHYSICS_HEIGHT_FIELD_SAMPLE_GUARD
#define AWESOME_PHYSICS_HEIGHT_FIELD_SAMPLE_GUARD

#include <utility>

#include <physX/geometry/PxHeightFieldSample.h>

#include <Awesome/Core/Types.hpp>

namespace aw
{
	namespace ph
	{
		class HeightFieldSample
		{
			physx::PxHeightFieldSample m_handle;

		public:
			HeightFieldSample() = default;
			inline HeightFieldSample(HeightFieldSample&& other);
			inline HeightFieldSample(const HeightFieldSample& other);
			~HeightFieldSample() = default;

			inline const physx::PxHeightFieldSample& Get() const;

			inline uint16& Height();
			inline uint16 Height() const;

			inline HeightFieldSample& operator=(HeightFieldSample&& rhs);
			inline HeightFieldSample& operator=(const HeightFieldSample& rhs);
		};
	}
}

#include <Awesome/Physics/Descriptor/HeightFieldSample.inl>

#endif // GUARD