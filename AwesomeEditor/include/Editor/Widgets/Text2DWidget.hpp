#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_TEXT2D_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_TEXT2D_WIDGET_GUARD

#include <Awesome/SDK/Components/Render/Text2D.hpp>

#include <Editor/Widgets/ComponentWidget.hpp>
#include <Editor/Layouts/TextLayout.hpp>

namespace ed
{
	class Text2DWidget : public ComponentWidget
	{
		TextLayout* m_text = nullptr;

	public:
		Text2DWidget(aw::sdk::Text2D* comp, QWidget* parent = nullptr);
		
		void Update() override;
	};
}

#endif // GUARD