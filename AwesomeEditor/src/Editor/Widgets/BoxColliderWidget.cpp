#include <Editor/Widgets/BoxColliderWidget.hpp>

namespace ed
{
	BoxColliderWidget::BoxColliderWidget(aw::sdk::BoxCollider* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		PhysicalComponentLayout* layout = new PhysicalComponentLayout(comp, "Box Collider", this);

		m_isTrigg = new BoolLayout("Is Triggered:", comp->isTrigger);
		m_isTrigg->onValueChange = [comp](bool b) { comp->isTrigger = b; };

		m_hScale = new Vector3Layout("Half Scale:", comp->halfScale, 0.001f);
		m_hScale->onValueChange = [comp](const aw::Vector3& hSc) { comp->halfScale = hSc; };

		layout->addRow(m_isTrigg);
		layout->addRow(m_hScale);

		setLayout(layout);
	}

	void BoxColliderWidget::Update()
	{
		aw::sdk::BoxCollider* comp = dynamic_cast<aw::sdk::BoxCollider*>(m_comp);
		
		dynamic_cast<PhysicalComponentLayout*>(layout())->Update(comp);

		m_isTrigg->Update(comp->isTrigger);
		m_hScale->Update(comp->halfScale);
	}
}