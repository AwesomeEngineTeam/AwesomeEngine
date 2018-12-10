#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_CAMERA_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_CAMERA_WIDGET_GUARD

#include <Awesome/SDK/Components/Render/Camera.hpp>

#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	class CameraWidget : public ComponentWidget
	{
		FloatLayout* m_fov = nullptr;
		FloatLayout* m_skFov = nullptr;
		FloatLayout* m_camNear = nullptr;
		FloatLayout* m_camFar = nullptr;

	public:
		CameraWidget(aw::sdk::Camera* comp, QWidget* parent = nullptr);

		void Update() override;
	};
}

#endif // GUARD