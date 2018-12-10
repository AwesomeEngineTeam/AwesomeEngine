#include <Editor/Widgets/SphereColliderWidget.hpp>

namespace ed
{
	SphereColliderWidget::SphereColliderWidget(aw::sdk::SphereCollider* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		PhysicalComponentLayout* layout = new PhysicalComponentLayout(comp, "Sphere Collider", this);

		m_isTrigg = new BoolLayout("Is Triggered:", comp->isTrigger);
		m_isTrigg->onValueChange = [comp](bool b) { comp->isTrigger = b; };

		m_radius = new FloatLayout("Radius:", comp->radius, 0.001f);
		m_radius->onValueChange = [comp](float radius) { comp->radius = radius; };

		layout->addRow(m_isTrigg);
		layout->addRow(m_radius);

		setLayout(layout);
	}

	void SphereColliderWidget::Update()
	{
		aw::sdk::SphereCollider* comp = dynamic_cast<aw::sdk::SphereCollider*>(m_comp);

		dynamic_cast<PhysicalComponentLayout*>(layout())->Update(comp);

		m_isTrigg->Update(comp->isTrigger);
		m_radius->Update(comp->radius);
	}
}