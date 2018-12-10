namespace aw
{
	namespace sdk
	{
		SoundEmitter::SoundEmitter(Actor* actor) : Component(actor, static_cast<uint32>(ComponentID::SoundEmitter), "Unnamed SoundEmitter")
		{

		}

		inline void SoundEmitter::PostEvent(const char* eventName)
		{
			AK::SoundEngine::PostEvent(eventName, m_id);
		}

		inline void SoundEmitter::ExecuteActionOnEvent(const char* eventName, ActionType actionType, int transitionDuration, InterpMode interpMode)
		{
			AK::SoundEngine::ExecuteActionOnEvent(eventName,
												static_cast<AK::SoundEngine::AkActionOnEventType>(actionType),
												static_cast<AkGameObjectID>(m_id),
												static_cast<AkTimeMs>(transitionDuration),
												static_cast<AkCurveInterpolation>(interpMode));
		}

		inline void SoundEmitter::SetRTPCValue(const char* rtpcName, AkRtpcValue value, int valueChangeDuration, InterpMode interpMode, bool bypassInternalValueInterpolation)
		{
			AK::SoundEngine::SetRTPCValue(rtpcName,
										value,
										static_cast<AkGameObjectID>(m_id),
										static_cast<AkTimeMs>(valueChangeDuration),
										static_cast<AkCurveInterpolation>(interpMode),
										bypassInternalValueInterpolation);
		}

		inline AkGameObjectID SoundEmitter::GetID() const
		{
			return m_id;
		}
	}
}