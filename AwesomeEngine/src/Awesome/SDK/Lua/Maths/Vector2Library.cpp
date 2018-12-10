#include <Awesome/SDK/Lua/Libraries.hpp>

#define WRAP_VEC2(field) LUWRA_MEMBER(Vector2, field)

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace vec2
			{
				GENERATE_METAMETHOD_FROM_OPERATOR(__mul, Vector2, Vector2, float, *)

				const luwra::MemberMap Functions
				{
					{ "Lerp", LUWRA_WRAP(Vector2::Lerp) },
					{ "LerpUnclamped", LUWRA_WRAP(Vector2::LerpUnclamped) },

					{ "Nlerp", LUWRA_WRAP(Vector2::Nlerp) },
					{ "NlerpUnclamped", LUWRA_WRAP(Vector2::NlerpUnclamped) },

					{ "Slerp", LUWRA_WRAP(Vector2::Slerp) },
					{ "SlerpUnclamped", LUWRA_WRAP(Vector2::SlerpUnclamped) },

					{ "DistanceBetween", LUWRA_WRAP(Vector2::DistanceBetween) },
					{ "AngleBetween", LUWRA_WRAP(Vector2::AngleBetween) },

					{ "Zero", LUWRA_WRAP(Vector2::Zero) },
					{ "One", LUWRA_WRAP(Vector2::One) },
					{ "Right", LUWRA_WRAP(Vector2::Right) },
					{ "Left", LUWRA_WRAP(Vector2::Left) },
					{ "Up", LUWRA_WRAP(Vector2::Up) },
					{ "Down", LUWRA_WRAP(Vector2::Down) },
				};

				const luwra::MemberMap Fields
				{
					WRAP_VEC2(x),
					WRAP_VEC2(y),

					WRAP_VEC2(GetNormalized),
					WRAP_VEC2(GetClamped),

					WRAP_VEC2(Normalize),
					WRAP_VEC2(Clamp),

					WRAP_VEC2(Norm),
					WRAP_VEC2(SquaredNorm),

					WRAP_VEC2(Dot)
				};

				const luwra::MemberMap MetaMethods
				{
					LUWRA_METAMETHOD_MEMBER(Vector2, __tostring, ToString),
					LUWRA_METAMETHOD_MEMBER_OVERLOAD(Vector2, __add, operator+, Vector2(Vector2::*)(const Vector2&) const),
					LUWRA_METAMETHOD_MEMBER_OVERLOAD(Vector2, __sub, operator-, Vector2(Vector2::*)(const Vector2&) const),
					LUWRA_METAMETHOD_MEMBER_OVERLOAD(Vector2, __unm, operator-, Vector2(Vector2::*)() const),
					LUA_REG(__mul),
					LUWRA_METAMETHOD_MEMBER(Vector2, __div, operator/),
					LUWRA_METAMETHOD_MEMBER(Vector2, __bor, operator|),
					LUWRA_METAMETHOD_MEMBER(Vector2, __eq, operator==)
				};
			}
		}
	}
}

#undef WRAP_VEC2