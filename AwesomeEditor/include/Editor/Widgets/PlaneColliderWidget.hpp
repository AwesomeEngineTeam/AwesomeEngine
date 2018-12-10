#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_PLANE_COLLIDER_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_PLANE_COLLIDER_WIDGET_GUARD

#include <Awesome/SDK/Components/Physical/PlaneCollider.hpp>

#include <Editor/Layouts/PhysicalComponentLayout.hpp>
#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	class PlaneColliderWidget : public ComponentWidget
	{
		BoolLayout* m_isTrigg = nullptr;

	public:
		PlaneColliderWidget(aw::sdk::PlaneCollider* comp, QWidget* parent = nullptr);

		void Update() override;
	};
}

#endif // GUARD