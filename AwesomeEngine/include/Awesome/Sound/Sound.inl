namespace aw
{
	namespace sound
	{
		inline void Init()
		{
			AkMemSettings memSettings;
			memSettings.uMaxNumPools = 20;

			AkStreamMgrSettings stmSettings;
			AK::StreamMgr::GetDefaultSettings(stmSettings);

			AkDeviceSettings deviceSettings;
			AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

			AkInitSettings initSettings;
			AK::SoundEngine::GetDefaultInitSettings(initSettings);

			AkPlatformInitSettings platformInitSettings;
			AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

			AkMusicSettings musicInit;
			AK::MusicEngine::GetDefaultInitSettings(musicInit);

			AK::SOUNDENGINE_DLL::Init(&memSettings, &stmSettings, &deviceSettings, &initSettings, &platformInitSettings, &musicInit);

			AK::SOUNDENGINE_DLL::SetBasePath(AKTEXT("assets/sounds/windows"));
			AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));
		}

		void Update()
		{
			AK::SOUNDENGINE_DLL::Tick();
		}

		bool LoadBank(const char* filename)
		{
			//TODO return id ?
			AkBankID id;

			return AK::SoundEngine::LoadBank(filename, AK_DEFAULT_POOL_ID, id) == AK_Success;
		}

		void UnInit()
		{
			//TODO Is it needed ?
			//AK::SoundEngine::ClearBanks();
			AK::SOUNDENGINE_DLL::Term();
		}

		void End()
		{
			AK::SoundEngine::StopAll();
			AK::SoundEngine::RenderAudio();
		}
	}
}