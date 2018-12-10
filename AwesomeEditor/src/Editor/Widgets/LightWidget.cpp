#include <Editor/Widgets/LightWidget.hpp>

namespace ed
{
	LightWidget::LightWidget(aw::sdk::Light* comp, const QString& name, QWidget* parent) : ComponentWidget(comp, parent)
	{
		ComponentLayout* layout = new ComponentLayout(comp, name, this);

		m_color = new ColorLayout("Color:", comp->color);
		m_color->onValueChange = [comp](const aw::Color3f& color) { comp->color = color; };

		m_intensity = new FloatLayout("Intensity:", comp->intensity, 0.0f);
		m_intensity->onValueChange = [comp](float f) { comp->intensity = f; };

		layout->addRow(m_color);
		layout->addRow(m_intensity);

		setLayout(layout);
	}

	void LightWidget::Update()
	{
		aw::sdk::Light* comp = dynamic_cast<aw::sdk::Light*>(m_comp);
		
		dynamic_cast<ComponentLayout*>(layout())->Update(comp);

		m_color->Update(comp->color);
		m_intensity->Update(comp->intensity);
	}
}