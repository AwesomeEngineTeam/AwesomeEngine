#include <Editor/Widgets/SoundEmitterWidget.hpp>

namespace ed
{
	SoundEmitterWidget::SoundEmitterWidget(aw::sdk::SoundEmitter* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		ComponentLayout* layout = new ComponentLayout(comp, "Sound Emitter", this);

		setLayout(layout);
	}

	void SoundEmitterWidget::Update()
	{
		dynamic_cast<ComponentLayout*>(layout())->Update(m_comp);
	}
}