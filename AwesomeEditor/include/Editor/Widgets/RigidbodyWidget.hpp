#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_RIGIDBODY_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_RIGIDBODY_WIDGET_GUARD

#include <Awesome/SDK/Components/Physical/Rigidbody.hpp>

#include <Editor/Layouts/PhysicalComponentLayout.hpp>
#include <Editor/Layouts/DropDownMenu.hpp>
#include <Editor/Layouts/UIntLayout.hpp>
#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	class RigidbodyWidget : public ComponentWidget
	{
		DropDownMenu* m_shapeSelect = nullptr;
		FloatLayout* m_radLayout = nullptr;
		FloatLayout* m_hHeightLayout = nullptr;
		Vector3Layout* m_hScaleLayout = nullptr;

		DropDownMenu* m_detectionSelect = nullptr;

		FloatLayout* m_mass = nullptr;
		UIntLayout* m_lockAxis = nullptr;
		BoolLayout* m_gravKin[2] = {nullptr, nullptr};
		Vector3Layout* m_vels[2] = { nullptr, nullptr };

	public:
		RigidbodyWidget(aw::sdk::Rigidbody* comp, QWidget* parent = nullptr);
		
		void Update() override;
	};
}

#endif // GUARD