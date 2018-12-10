#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_SOUND_LISTENER_GUARD
#define AWESOME_EDITOR_WIDGETS_SOUND_LISTENER_GUARD

#include <Awesome/SDK/Components/Sound/SoundListener.hpp>

#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	class SoundListenerWidget : public ComponentWidget
	{
	public:
		SoundListenerWidget(aw::sdk::SoundListener* comp, QWidget* parent = nullptr);

		void Update() override;
	};
}

#endif // GUARD