#pragma once

#ifndef AWESOME_SDK_SOUND_EMITTER_GUARD
#define AWESOME_SDK_SOUND_EMITTER_GUARD

#include <Awesome/SDK/Components/Component.hpp>
#include <Awesome/Sound/Sound.hpp>

namespace aw
{
	namespace sdk
	{
		class SoundEmitter : public Component
		{
			AkGameObjectID m_id;
			friend class SoundSystem;

		public:
			enum class ActionType
			{
				Stop = AK::SoundEngine::AkActionOnEventType_Stop,
				Pause = AK::SoundEngine::AkActionOnEventType_Pause,
				Resume = AK::SoundEngine::AkActionOnEventType_Resume,
				Break = AK::SoundEngine::AkActionOnEventType_Break,
				ReleaseEnvelope = AK::SoundEngine::AkActionOnEventType_ReleaseEnvelope
			};

			enum class InterpMode
			{
				Log1 = AkCurveInterpolation_Log1,
				Log3 = AkCurveInterpolation_Log3,
				Sine = AkCurveInterpolation_Sine,
				SineRecip = AkCurveInterpolation_SineRecip,
				S_Curve = AkCurveInterpolation_SCurve,
				Inversed_S_Curve = AkCurveInterpolation_InvSCurve,
				Linear = AkCurveInterpolation_Linear,
				Exp1 = AkCurveInterpolation_Exp1,
				Exp3 = AkCurveInterpolation_Exp3,
				LastFadeCurve = AkCurveInterpolation_LastFadeCurve,
				Constant = AkCurveInterpolation_Constant
			};

			inline SoundEmitter(Actor* actor);

			inline void PostEvent(const char* eventName);
			inline void ExecuteActionOnEvent(const char* eventName, ActionType actionType, int transitionDuration = 0, InterpMode interpMode = InterpMode::Linear);
			inline void SetRTPCValue(const char* rtpcName, AkRtpcValue value, int valueChangeDuration = 0, InterpMode interpMode = InterpMode::Linear, bool bypassInternalValueInterpolation = false);

			inline AkGameObjectID GetID() const;

			REFLECT_BEGIN(SoundEmitter)
				REFLECT_MEMBER_INHERITANCE(Component)
				REFLECT_MEMBER(m_id)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::SoundEmitter>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::SoundEmitter);
	};
}

#include <Awesome/SDK/Components/Sound/SoundEmitter.inl>

#endif //Guard