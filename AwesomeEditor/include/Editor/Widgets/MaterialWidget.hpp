#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_MATERIAL_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_MATERIAL_WIDGET_GUARD

#include <Editor/Core/Material.hpp>
#include <Editor/Layouts/BoolLayout.hpp>
#include <Editor/Layouts/TextLayout.hpp>
#include <Editor/Widgets/DropWidget.hpp>

namespace ed
{
	class MaterialLayout : public QFormLayout
	{
		ed::Material m_material;

	public:
		TextLayout* name = nullptr;

		MaterialLayout(ed::Material& material, QWidget * parent = nullptr);
	};

	class MaterialWidget : public QWidget
	{
	public:
		MaterialWidget(ed::Material& material, QWidget* parent = nullptr);
	};
}

#endif // GUARD