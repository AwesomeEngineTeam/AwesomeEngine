#include <Awesome/SDK/Lua/Libraries.hpp>

#define WRAP_ROT(field) LUWRA_MEMBER(TransformBase::Rotation, field)

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace rot
			{
				const luwra::MemberMap Fields
				{
					WRAP_ROT(GetInversed),
					WRAP_ROT(Inverse)
				};

				const luwra::MemberMap MetaMethods
				{
					LUWRA_METAMETHOD_MEMBER(TransformBase::Rotation, __tostring, ToString)
				};
			}
		}
	}
}

#undef WRAP_ROT