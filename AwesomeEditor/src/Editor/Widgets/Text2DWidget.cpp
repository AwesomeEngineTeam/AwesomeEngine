#include <Editor/Widgets/Text2DWidget.hpp>

namespace ed
{
	Text2DWidget::Text2DWidget(aw::sdk::Text2D* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		ComponentLayout* layout = new ComponentLayout(comp, "Text2D", this);

		m_text = new TextLayout("Text:", comp->GetContent());
		m_text->onValueChange = [comp](const std::string& str) { comp->SetContent(str); };

		layout->addRow(m_text);

		setLayout(layout);
	}

	void Text2DWidget::Update()
	{
		aw::sdk::Text2D* comp = dynamic_cast<aw::sdk::Text2D*>(m_comp);

		dynamic_cast<ComponentLayout*>(layout())->Update(comp);

		m_text->Update(comp->GetContent());
	}
}