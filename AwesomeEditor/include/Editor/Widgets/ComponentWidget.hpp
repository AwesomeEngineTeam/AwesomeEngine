#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_COMPONENT_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_COMPONENT_WIDGET_GUARD

#include <Editor/Layouts/ComponentLayout.hpp>

namespace ed
{
	class ComponentWidget : public QWidget
	{
	protected:
		aw::sdk::Component* m_comp = nullptr;

	public:
		ComponentWidget(aw::sdk::Component* comp, bool defaultLayout = false, QWidget* parent = nullptr);

		virtual void Update();
	};
}

#endif // GUARD