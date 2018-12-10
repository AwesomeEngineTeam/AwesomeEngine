#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_SOUND_EMITTER_GUARD
#define AWESOME_EDITOR_WIDGETS_SOUND_EMITTER_GUARD

#include <Awesome/SDK/Components/Sound/SoundEmitter.hpp>

#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	class SoundEmitterWidget : public ComponentWidget
	{
	public:
		SoundEmitterWidget(aw::sdk::SoundEmitter* comp, QWidget* parent = nullptr);
		
		void Update() override;
	};
}

#endif // GUARD