#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_BOX_COLLIDER_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_BOX_COLLIDER_WIDGET_GUARD

#include <Awesome/SDK/Components/Physical/BoxCollider.hpp>

#include <Editor/Layouts/PhysicalComponentLayout.hpp>
#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	class BoxColliderWidget : public ComponentWidget
	{
		BoolLayout* m_isTrigg = nullptr;
		Vector3Layout* m_hScale = nullptr;

	public:
		BoxColliderWidget(aw::sdk::BoxCollider* comp, QWidget* parent = nullptr);

		void Update() override;
	};
}

#endif // GUARD