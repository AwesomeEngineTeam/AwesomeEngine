#pragma once

#ifndef AWESOME_SOUND_SOUND_GUARD
#define AWESOME_SOUND_SOUND_GUARD

#include "AkSoundEngineDLL.h"

namespace aw
{
	namespace sound
	{
		static inline void Init();
		static inline void UnInit();

		static inline void Update();

		static inline bool LoadBank(const char* filename);

		static inline void End();
	};
}

#include <Awesome/Sound/Sound.inl>

#endif //Guard