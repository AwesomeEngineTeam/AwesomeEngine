#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	ComponentWidget::ComponentWidget(aw::sdk::Component* comp, bool defaultLayout, QWidget* parent) :
		QWidget(parent),
		m_comp{ comp }
	{
		if (defaultLayout)
			setLayout(new ComponentLayout(comp, "Component", this));
	}

	void ComponentWidget::Update()
	{
	}
}