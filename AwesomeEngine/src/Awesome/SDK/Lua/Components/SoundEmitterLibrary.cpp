#include <Awesome/SDK/Lua/Libraries.hpp>

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace sound_emitter
			{
				const luwra::MemberMap Fields
				{
					COMPONENT_FIELDS(SoundEmitter),
					LUWRA_MEMBER(SoundEmitter, PostEvent),
					LUWRA_MEMBER(SoundEmitter, ExecuteActionOnEvent),
					LUWRA_MEMBER(SoundEmitter, SetRTPCValue),
					LUWRA_MEMBER(SoundEmitter, GetID)
				};

				const luwra::MemberMap ActionType
				{
					{ "Stop", SoundEmitter::ActionType::Stop },
					{ "Pause", SoundEmitter::ActionType::Pause },
					{ "Resume", SoundEmitter::ActionType::Resume},
					{ "Break", SoundEmitter::ActionType::Break },
					{ "ReleaseEnvelope", SoundEmitter::ActionType::ReleaseEnvelope }
				};

				const luwra::MemberMap InterpMode
				{
					{ "Log1", SoundEmitter::InterpMode::Log1 },
					{ "Log3", SoundEmitter::InterpMode::Log3 },
					{ "Sine", SoundEmitter::InterpMode::Sine },
					{ "SineRecip", SoundEmitter::InterpMode::SineRecip },
					{ "S_Curve", SoundEmitter::InterpMode::S_Curve },
					{ "Inversed_S_Curve", SoundEmitter::InterpMode::Inversed_S_Curve },
					{ "Linear", SoundEmitter::InterpMode::Linear },
					{ "Exp1", SoundEmitter::InterpMode::Exp1 },
					{ "Exp3", SoundEmitter::InterpMode::Exp3 },
					{ "LastFadeCurve", SoundEmitter::InterpMode::LastFadeCurve },
					{ "Constant", SoundEmitter::InterpMode::Constant }
				};

				const luwra::MemberMap SoundMap
				{
					{ "ActionType", ActionType },
					{ "InterpMode", InterpMode },
					{ "SetState", LUWRA_WRAP_OVERLOAD(AK::SoundEngine::SetState, AKRESULT(const char*, const char*)) },
					{ "MuteBackgroundMusic", LUWRA_WRAP(AK::SoundEngine::MuteBackgroundMusic) },
					{ "GetBackgroundMusicMute", LUWRA_WRAP(AK::SoundEngine::GetBackgroundMusicMute) }
				};
			}
		}
	}
}