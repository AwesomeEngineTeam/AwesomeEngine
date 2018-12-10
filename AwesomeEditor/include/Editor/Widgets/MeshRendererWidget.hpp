#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_MESH_RENDERER_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_MESH_RENDERER_WIDGET_GUARD

#include <Awesome/SDK/Components/Render/MeshRenderer.hpp>

#include <Editor/Widgets/DropWidget.hpp>
#include <Editor/Widgets/ComponentWidget.hpp>

namespace ed
{
	class MeshRendererWidget : public ComponentWidget
	{
		Vector3Layout* m_posLayout;
		Vector3Layout* m_rotLayout;
		Vector3Layout* m_scaleLayout;
		FloatLayout* m_scaleULayout;
		FloatLayout* m_scaleVLayout;
		FloatLayout* m_offsetULayout;
		FloatLayout* m_offsetVLayout;

		DropWidget* m_meshDrop = nullptr;
		DropWidget* m_matDrop = nullptr;

	public:
		MeshRendererWidget(aw::sdk::MeshRenderer* comp, QWidget* parent = nullptr);

		void Update() override;
	};
}

#endif // GUARD