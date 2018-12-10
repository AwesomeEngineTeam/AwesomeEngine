#include <Editor/Widgets/SoundListenerWidget.hpp>

namespace ed
{
	SoundListenerWidget::SoundListenerWidget(aw::sdk::SoundListener* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		ComponentLayout* layout = new ComponentLayout(comp, "Sound Emitter", this);

		setLayout(layout);
	}

	void SoundListenerWidget::Update()
	{
		dynamic_cast<ComponentLayout*>(layout())->Update(m_comp);
	}
}