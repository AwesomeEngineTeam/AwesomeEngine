#include <Awesome/SDK/Lua/Libraries.hpp>

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace math_utils
			{
				const luwra::MemberMap Table
				{
					{ "PI", M_PI },
					{ "PI2", M_PI2 },
					{ "2PI", M_2PI },

					{ "DegToRad", DEG_TO_RAD },
					{ "RadToDeg", RAD_TO_DEG },

					{ "Sqrt2", SQRT2 },
					{ "Sqrt3", SQRT3 },

					{ "FloatEpsilon", FLT_EPSILON },

					//Functions
					{ "GetClamped", LUWRA_WRAP(GetClamped) },

					{ "DegreeToRadian", LUWRA_WRAP(DegreeToRadian) },
					{ "RadianToDegree", LUWRA_WRAP(RadianToDegree) },
					{ "Lerp", LUWRA_WRAP(Lerp) },
					{ "Square", LUWRA_WRAP(Square) },
					{ "InvSqrt", LUWRA_WRAP(InvSqrt) },
					{ "Sign", LUWRA_WRAP_OVERLOAD(Sign, float(const float)) },
					{ "CopySign", LUWRA_WRAP(CopySign) },
					{ "NearlyEqual", LUWRA_WRAP(NearlyEqual) },
					{ "NearlyZero", LUWRA_WRAP(NearlyZero) }
				};
			}
		}
	}
}