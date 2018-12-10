#include <Editor/Layouts/PhysicalComponentLayout.hpp>

namespace ed
{
	PhysicalComponentLayout::PhysicalComponentLayout(aw::sdk::PhysicalComponent* comp, const QString& name, QWidget* parent) : ComponentLayout(comp, name, parent)
	{
		m_collGroup = new UIntLayout("Layer Group:", static_cast<aw::uint32>(comp->collisionLayer.load()));
		m_collGroup->onValueChange = [comp](int i) { comp->collisionLayer = static_cast<aw::sdk::CollisionLayer>(i); };

		m_collMask = new UIntLayout("Layer Mask:", comp->filterMask);
		m_collMask->onValueChange = [comp](int i) { comp->filterMask = i; };

		m_material[0] = new FloatLayout("Static Friction:", comp->material.staticFriction);
		m_material[0]->onValueChange = [comp](float f) { comp->material.staticFriction = f; };

		m_material[1] = new FloatLayout("Dynamic Friction:", comp->material.dynamicFriction);
		m_material[1]->onValueChange = [comp](float f) { comp->material.dynamicFriction = f; };

		m_material[2] = new FloatLayout("Restitution:", comp->material.restitution, 0, 1);
		m_material[2]->onValueChange = [comp](float f) { comp->material.restitution = f; };

		addRow(m_collGroup);
		addRow(m_collMask);
		addRow(m_material[0]);
		addRow(m_material[1]);
		addRow(m_material[2]);
	}

	void PhysicalComponentLayout::Update(aw::sdk::PhysicalComponent* comp)
	{
		ComponentLayout::Update(comp);

		m_collGroup->Update(static_cast<aw::uint32>(comp->collisionLayer.load()));
		m_collMask->Update(comp->filterMask);

		m_material[0]->Update(comp->material.staticFriction);
		m_material[1]->Update(comp->material.dynamicFriction);
		m_material[2]->Update(comp->material.restitution);
	}
}