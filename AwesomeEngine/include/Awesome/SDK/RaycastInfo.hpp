#pragma once

#ifndef AWESOME_SDK_RAYCAST_INFO_GUARD
#define AWESOME_SDK_RAYCAST_INFO_GUARD

#include <Awesome/Physics/RaycastHit.hpp>
#include <Awesome/SDK/Components/Physical/PhysicalComponent.hpp>

namespace aw
{
	namespace sdk
	{
		class RaycastInfo
		{
		public:
			Actor* body = nullptr;
			Component* comp = nullptr;

			float distance = 0.0f;

			Vector3 position;
			Vector3 normal;

			RaycastInfo() = default;
			inline RaycastInfo(const aw::ph::RaycastHit& hit);
			inline RaycastInfo(RaycastInfo&& other);
			inline RaycastInfo(const RaycastInfo& other);
			~RaycastInfo() = default;

			inline RaycastInfo& operator=(RaycastInfo&& rhs);
			inline RaycastInfo& operator=(const RaycastInfo& rhs);
		};
	}
}

#include <Awesome/SDK/RaycastInfo.inl>

#endif // GUARD