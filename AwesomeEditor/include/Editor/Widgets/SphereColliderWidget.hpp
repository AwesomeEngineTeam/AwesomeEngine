#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_SPHERE_COLLIDER_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_SPHERE_COLLIDER_WIDGET_GUARD

#include <Awesome/SDK/Components/Physical/SphereCollider.hpp>

#include <Editor/Layouts/PhysicalComponentLayout.hpp>
#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	class SphereColliderWidget : public ComponentWidget
	{
		BoolLayout* m_isTrigg = nullptr;
		FloatLayout* m_radius = nullptr;

	public:
		SphereColliderWidget(aw::sdk::SphereCollider* comp, QWidget* parent = nullptr);
		
		void Update() override;
	};
}

#endif // GUARD