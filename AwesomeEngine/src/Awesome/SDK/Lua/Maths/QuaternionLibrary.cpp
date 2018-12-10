#include <Awesome/SDK/Lua/Libraries.hpp>

#define WRAP_QUAT(field) LUWRA_MEMBER(Quaternion, field)

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace quat
			{
				static int __mul(lua_State* L)
				{
					if (LuaHelper::IsUserData(L, 1) && LuaHelper::IsUserData(L, 2))
					{
						if (LuaHelper::AreOfSameType(L, 1, 2))
							LUA_PUSH_OP_RESULT(L, Quaternion, Quaternion, Quaternion, *);
						else
							LUA_PUSH_OP_RESULT(L, Vector3, Quaternion, Vector3, *);
					}
					else
						GENERATE_METAMETHOD_BODY_FROM_OPERATOR(L, Quaternion, Quaternion, float, *)

						return 1;
				}

				const luwra::MemberMap Functions
				{
					{ "FromAxis", LUWRA_WRAP(Quaternion::FromAxis) },
					{ "FromEulerAngles", LUWRA_WRAP(Quaternion::FromEulerAngles) },

					{ "Lerp", LUWRA_WRAP(Quaternion::Lerp) },
					{ "NLerp", LUWRA_WRAP(Quaternion::NLerp) },
					{ "SLerp", LUWRA_WRAP(Quaternion::SLerp) },

					{ "Zero", LUWRA_WRAP(Quaternion::Zero) },
					{ "Identity", LUWRA_WRAP(Quaternion::Identity) },
				};

				const luwra::MemberMap Fields
				{
					WRAP_QUAT(x),
					WRAP_QUAT(y),
					WRAP_QUAT(z),
					WRAP_QUAT(w),

					WRAP_QUAT(GetConjugated),
					WRAP_QUAT(GetInversed),
					WRAP_QUAT(GetNormalized),
					WRAP_QUAT(GetClamped),

					WRAP_QUAT(Conjugate),
					WRAP_QUAT(Inverse),
					WRAP_QUAT(Normalize),
					WRAP_QUAT(Clamp),

					WRAP_QUAT(Norm),
					WRAP_QUAT(SquaredNorm),

					WRAP_QUAT(Dot),
					WRAP_QUAT(Cross),

					WRAP_QUAT(EulerAngles)

					//TODO ToMatrix ?
				};

				const luwra::MemberMap MetaMethods
				{
					LUWRA_METAMETHOD_MEMBER(Quaternion, __tostring, ToString),
					LUWRA_METAMETHOD_MEMBER_OVERLOAD(Quaternion, __add, operator+, Quaternion(Quaternion::*)(const Quaternion&) const),
					LUWRA_METAMETHOD_MEMBER_OVERLOAD(Quaternion, __sub, operator-, Quaternion(Quaternion::*)(const Quaternion&) const),
					LUWRA_METAMETHOD_MEMBER_OVERLOAD(Quaternion, __unm, operator-, Quaternion(Quaternion::*)() const),
					LUA_REG(__mul),
					LUWRA_METAMETHOD_MEMBER(Quaternion, __div, operator/),
					LUWRA_METAMETHOD_MEMBER(Quaternion, __pow, operator^),
					LUWRA_METAMETHOD_MEMBER(Quaternion, __bor, operator|),
					LUWRA_METAMETHOD_MEMBER(Quaternion, __eq, operator==)
				};
			}
		}
	}
}

#undef WRAP_QUAT