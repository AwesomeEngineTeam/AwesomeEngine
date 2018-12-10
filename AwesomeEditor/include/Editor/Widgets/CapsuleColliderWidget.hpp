#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_CAPSULE_COLLIDER_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_CAPSULE_COLLIDER_WIDGET_GUARD

#include <Awesome/SDK/Components/Physical/CapsuleCollider.hpp>

#include <Editor/Layouts/PhysicalComponentLayout.hpp>
#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	class CapsuleColliderWidget : public ComponentWidget
	{
		BoolLayout* m_isTrigg = nullptr;
		FloatLayout* m_radius = nullptr;
		FloatLayout* m_hHeight = nullptr;

	public:
		CapsuleColliderWidget(aw::sdk::CapsuleCollider* comp, QWidget* parent = nullptr);
		
		void Update() override;
	};
}

#endif // GUARD