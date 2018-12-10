#include <Editor/Widgets/PlaneColliderWidget.hpp>

namespace ed
{
	PlaneColliderWidget::PlaneColliderWidget(aw::sdk::PlaneCollider* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		PhysicalComponentLayout* layout = new PhysicalComponentLayout(comp, "Plane Collider", this);

		m_isTrigg = new BoolLayout("Is Triggered:", comp->isTrigger);
		m_isTrigg->onValueChange = [comp](bool b) { comp->isTrigger = b; };

		layout->addRow(m_isTrigg);

		setLayout(layout);
	}

	void PlaneColliderWidget::Update()
	{
		aw::sdk::PlaneCollider* comp = dynamic_cast<aw::sdk::PlaneCollider*>(m_comp);

		dynamic_cast<PhysicalComponentLayout*>(layout())->Update(comp);

		m_isTrigg->Update(comp->isTrigger);
	}
}