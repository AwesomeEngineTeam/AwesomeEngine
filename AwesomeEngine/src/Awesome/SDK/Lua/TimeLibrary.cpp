#include <Awesome/SDK/Lua/Libraries.hpp>

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace time
			{
				const luwra::MemberMap Functions
				{
					{ "DeltaTime", LUWRA_WRAP(Time::DeltaTime) },
					{ "GlobalTime", LUWRA_WRAP(Time::GlobalTime) },
					{ "TimeScale", LUWRA_WRAP(Time::TimeScale) },
					{ "SetTimeScale", LUWRA_WRAP(Time::SetTimeScale) }
				};
			}
		}
	}
}