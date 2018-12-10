#include <Awesome/SDK/Lua/Libraries.hpp>

#define WRAP_RIGIDBODY(field) LUWRA_MEMBER(aw::sdk::Rigidbody, field)

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace physics
			{
				const luwra::MemberMap Functions
				{
					{ "Raycast", LUWRA_WRAP(PhysicalSystem::Raycast) },
				};

				const luwra::MemberMap RaycastFields
				{
					LUWRA_MEMBER(RaycastInfo, body),
					LUWRA_MEMBER(RaycastInfo, comp),

					LUWRA_MEMBER(RaycastInfo, distance),

					LUWRA_MEMBER(RaycastInfo, position),
					LUWRA_MEMBER(RaycastInfo, normal),
				};

				const luwra::MemberMap RigidbodyFields
				{
					COMPONENT_FIELDS(Rigidbody),

					WRAP_RIGIDBODY(mass),
					WRAP_RIGIDBODY(useGravity),
					WRAP_RIGIDBODY(lockAxisMask),

					WRAP_RIGIDBODY(radius),
					WRAP_RIGIDBODY(halfHeight),
					WRAP_RIGIDBODY(halfScale),

					WRAP_RIGIDBODY(linearVelocity),
					WRAP_RIGIDBODY(angularVelocity),

					LUWRA_MEMBER(Rigidbody, AddForce),
					LUWRA_MEMBER(Rigidbody, AddTorque),
					LUWRA_MEMBER(Rigidbody, SetKinematic),
					LUWRA_MEMBER(Rigidbody, IsKinematic),
				};

				const luwra::MemberMap ForceMode
				{
					{ "Acceleration", ph::ForceMode::Acceleration },
					{ "Force", ph::ForceMode::Force },
					{ "Impulse", ph::ForceMode::Impulse },
					{ "VelocityChange", ph::ForceMode::VelocityChange }
				};
			}
		}
	}
}