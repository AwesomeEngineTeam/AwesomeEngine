#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_PHYSICAL_COMPONENT_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_PHYSICAL_COMPONENT_LAYOUT_GUARD

#include <Awesome/SDK/Components/Physical/PhysicalComponent.hpp>

#include <Editor/Layouts/UIntLayout.hpp>
#include <Editor/Layouts/ComponentLayout.hpp>

namespace ed
{
	class PhysicalComponentLayout : public ComponentLayout
	{
		UIntLayout* m_collGroup = nullptr;
		UIntLayout* m_collMask = nullptr;
		FloatLayout* m_material[3] = {};

	public:
		PhysicalComponentLayout(aw::sdk::PhysicalComponent* comp, const QString& name, QWidget* parent = nullptr);

		void Update(aw::sdk::PhysicalComponent* comp);
	};
}

#endif // GUARD