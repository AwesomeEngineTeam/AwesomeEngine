#include <Awesome/SDK/Lua/Libraries.hpp>

#define WRAP_TR(field) LUWRA_MEMBER(UniformTransform, field)

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace utr
			{
				static int __mul(lua_State* L)
				{
					luwra::Value<Quaternion>::push(L, Quaternion());

					bool same = LuaHelper::AreOfSameType(L, 2, 3);
					lua_pop(L, 1);

					if (same)
						LUA_PUSH_OP_RESULT(L, Quaternion, UniformTransform, Quaternion, *);
					else
						LUA_PUSH_OP_RESULT(L, Vector3, UniformTransform, Vector3, *);

					return 1;
				}

				const luwra::MemberMap Functions
				{
					{ "Identity", LUWRA_WRAP(UniformTransform::Identity) }
				};

				const luwra::MemberMap Fields
				{
					WRAP_TR(GetPosition),
					WRAP_TR(SetPosition),

					WRAP_TR(GetRotation),
					WRAP_TR(SetRotation),
					WRAP_TR(scale),

					WRAP_TR(GetInversed),
					//TODO ToMatrix ?

					WRAP_TR(Inverse),
				};

				const luwra::MemberMap MetaMethods
				{
					LUWRA_METAMETHOD_MEMBER(UniformTransform, __tostring, ToString),
					LUWRA_METAMETHOD_MEMBER(UniformTransform, __eq, operator==),
					//LUWRA_METAMETHOD_MEMBER_OVERLOAD(UniformTransform, __mul, operator*, Quaternion(UniformTransform::*)(const Quaternion&) const)
					LUA_REG(__mul)
				};
			}
		}
	}
}

#undef WRAP_TR