#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_LIGHT_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_LIGHT_WIDGET_GUARD

#include <Awesome/SDK/Components/Render/Light.hpp>
#include <Awesome/SDK/Components/Render/PointLight.hpp>
#include <Awesome/SDK/Components/Render/DirectionalLight.hpp>

#include <Editor/Widgets/ComponentWidget.hpp>
#include <Editor/Layouts/ColorLayout.hpp>

namespace ed
{
	class LightWidget : public ComponentWidget
	{
		ColorLayout* m_color = nullptr;
		FloatLayout* m_intensity = nullptr;

	public:
		LightWidget(aw::sdk::Light* comp, const QString& name, QWidget* parent = nullptr);
		
		void Update() override;
	};
}

#endif // GUARD