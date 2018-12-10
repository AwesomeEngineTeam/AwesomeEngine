#include <Editor/Widgets/CapsuleColliderWidget.hpp>

namespace ed
{
	CapsuleColliderWidget::CapsuleColliderWidget(aw::sdk::CapsuleCollider* comp, QWidget* parent) :
		ComponentWidget(comp, parent)
	{
		PhysicalComponentLayout* layout = new PhysicalComponentLayout(comp, "Capsule Collider", this);

		m_isTrigg = new BoolLayout("Is Triggered:", comp->isTrigger);
		m_isTrigg->onValueChange = [comp](bool b) { comp->isTrigger = b; };

		m_radius = new FloatLayout("Radius:", comp->radius, 0.001f);
		m_radius->onValueChange = [comp](float rad) { comp->radius = rad; };

		m_hHeight = new FloatLayout("Half Height:", comp->halfHeight, 0.001f);
		m_hHeight->onValueChange = [comp](float hHeig) { comp->halfHeight = hHeig; };

		layout->addRow(m_isTrigg);
		layout->addRow(m_radius);
		layout->addRow(m_hHeight);

		setLayout(layout);
	}

	void CapsuleColliderWidget::Update()
	{
		aw::sdk::CapsuleCollider* comp = dynamic_cast<aw::sdk::CapsuleCollider*>(m_comp);

		dynamic_cast<PhysicalComponentLayout*>(layout())->Update(comp);

		m_isTrigg->Update(comp->isTrigger);
		m_radius->Update(comp->radius);
		m_hHeight->Update(comp->halfHeight);
	}
}