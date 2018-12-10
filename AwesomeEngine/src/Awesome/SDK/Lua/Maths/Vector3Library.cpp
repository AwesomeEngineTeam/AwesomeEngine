#include <Awesome/SDK/Lua/Libraries.hpp>

#define WRAP_VEC3(field) LUWRA_MEMBER(Vector3, field)

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace vec3
			{
				GENERATE_METAMETHOD_FROM_OPERATOR(__mul, Vector3, Vector3, float, *)

				const luwra::MemberMap Functions
				{
					{ "Lerp", LUWRA_WRAP(Vector3::Lerp) },
					{ "LerpUnclamped", LUWRA_WRAP(Vector3::LerpUnclamped) },

					{ "Nlerp", LUWRA_WRAP(Vector3::Nlerp) },
					{ "NlerpUnclamped", LUWRA_WRAP(Vector3::NlerpUnclamped) },

					{ "Slerp", LUWRA_WRAP_OVERLOAD(Vector3::Slerp, Vector3(const Vector3&, const Vector3&, float)) },
					{ "SlerpUnclamped", LUWRA_WRAP_OVERLOAD(Vector3::SlerpUnclamped, Vector3(const Vector3&, const Vector3&, float)) },

					{ "SlerpOriented", LUWRA_WRAP_OVERLOAD(Vector3::Slerp, Vector3(const Vector3&, const Vector3&, const Vector3&, float)) },
					{ "SlerpOrientedUnclamped", LUWRA_WRAP_OVERLOAD(Vector3::SlerpUnclamped, Vector3(const Vector3&, const Vector3&, const Vector3&, float)) },

					{ "GramSchmidt", LUWRA_WRAP(Vector3::GramSchmidt) },
					{ "AngleSignedBetween", LUWRA_WRAP(Vector3::AngleSignedBetween) },

					{ "Zero", LUWRA_WRAP(Vector3::Zero) },
					{ "One", LUWRA_WRAP(Vector3::One) },
					{ "Right", LUWRA_WRAP(Vector3::Right) },
					{ "Left", LUWRA_WRAP(Vector3::Left) },
					{ "Up", LUWRA_WRAP(Vector3::Up) },
					{ "Down", LUWRA_WRAP(Vector3::Down) },
					{ "Forward", LUWRA_WRAP(Vector3::Forward) },
					{ "Backward", LUWRA_WRAP(Vector3::Backward) },
				};

				const luwra::MemberMap Fields
				{
					WRAP_VEC3(x),
					WRAP_VEC3(y),
					WRAP_VEC3(z),

					WRAP_VEC3(GetNormalized),
					WRAP_VEC3(GetClamped),

					WRAP_VEC3(Normalize),
					WRAP_VEC3(Clamp),

					WRAP_VEC3(Norm),
					WRAP_VEC3(SquaredNorm),

					WRAP_VEC3(Dot),
					WRAP_VEC3(PerpDot),

					WRAP_VEC3(Cross),
					WRAP_VEC3(DoubleCross),
				};

				const luwra::MemberMap MetaMethods
				{
					LUWRA_METAMETHOD_MEMBER(Vector3, __tostring, ToString),
					LUWRA_METAMETHOD_MEMBER_OVERLOAD(Vector3, __add, operator+, Vector3(Vector3::*)(const Vector3&) const),
					LUWRA_METAMETHOD_MEMBER_OVERLOAD(Vector3, __sub, operator-, Vector3(Vector3::*)(const Vector3&) const),
					LUWRA_METAMETHOD_MEMBER_OVERLOAD(Vector3, __unm, operator-, Vector3(Vector3::*)() const),
					//        LUWRA_METAMETHOD_MEMBER(Vector3, __mul, operator*),
					LUA_REG(__mul),
					LUWRA_METAMETHOD_MEMBER(Vector3, __div, operator/),
					LUWRA_METAMETHOD_MEMBER(Vector3, __pow, operator^),
					LUWRA_METAMETHOD_MEMBER(Vector3, __bor, operator|),
					LUWRA_METAMETHOD_MEMBER(Vector3, __eq, operator==)
				};
			}
		}
	}
}

#undef WRAP_VEC3