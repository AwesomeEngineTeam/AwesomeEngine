#include <Editor/Widgets/MeshRendererWidget.hpp>
#include <Editor/Layouts/TransformLayout.hpp>
#include <Editor/Core/EditorApplication.hpp>

namespace ed
{
	MeshRendererWidget::MeshRendererWidget(aw::sdk::MeshRenderer* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		ASSERT(comp == nullptr, "comp is nullptr");

		ComponentLayout* layout = new ComponentLayout(comp, "Mesh Renderer", this);

		auto& tr = comp->meshTransform;

		m_posLayout = new Vector3Layout("Position:", tr.GetPosition());
		m_posLayout->onValueChange = [comp](const aw::Vector3& pos)
		{
			auto& tr = comp->meshTransform;

			tr.SetPosition(pos);
		};

		m_rotLayout = new Vector3Layout("Rotation:", tr.GetRotation().EulerAngles());
		m_rotLayout->onValueChange = [comp](const aw::Vector3& rot)
		{
			auto& tr = comp->meshTransform;

			tr.SetRotation(aw::Quaternion::FromEulerAngles(rot * DEG_TO_RAD));
		};

		m_scaleLayout = new Vector3Layout("Scale:", tr.scale);
		m_scaleLayout->onValueChange = [comp](const aw::Vector3& sc)
		{
			auto& tr = comp->meshTransform;

			tr.scale = sc;
		};

		layout->addRow(new QLabel("Mesh Transform:"));

		layout->addRow(m_posLayout);
		layout->addRow(m_rotLayout);
		layout->addRow(m_scaleLayout);

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());

		m_meshDrop = new DropWidget;
		m_meshDrop->AddExtension(".awmesh"); m_meshDrop->AddExtension(".obj"); m_meshDrop->AddExtension(".gltf"); m_meshDrop->AddExtension(".fbx");
		m_meshDrop->setText(editor->GetWindow().GetAssetName(comp->GetMesh(), AssetsView::AssetsType::Mesh));

		m_meshDrop->onValueChange = [comp, editor](const std::string& str) {
			comp->SetMesh(editor->GetWindow().GetAssetIndex(QString(str.c_str())));
		};

		m_matDrop = new DropWidget;
		m_matDrop->AddExtension(".mat");

		m_matDrop->setText(editor->GetWindow().GetAssetName(comp->GetMaterial(), AssetsView::AssetsType::Material));

		m_matDrop->onValueChange = [comp, editor](const std::string& str) {
			comp->SetMaterial(editor->GetWindow().GetAssetIndex(QString(str.c_str())));
		};

		layout->addRow("Mesh:", m_meshDrop);
		layout->addRow("Material:", m_matDrop);

		m_scaleULayout = new FloatLayout("Scale U:", comp->scaleUV.x);
		m_scaleULayout->onValueChange = [comp](float f)
		{
			comp->scaleUV.x = f;
		};

		layout->addRow(m_scaleULayout);

		m_scaleVLayout = new FloatLayout("Scale V:", comp->scaleUV.y);
		m_scaleVLayout->onValueChange = [comp](float f)
		{
			comp->scaleUV.y = f;
		};

		layout->addRow(m_scaleVLayout);

		m_offsetULayout = new FloatLayout("offset U:", comp->offsetUV.x);
		m_offsetULayout->onValueChange = [comp](float f)
		{
			comp->offsetUV.x = f;
		};

		layout->addRow(m_offsetULayout);

		m_offsetVLayout = new FloatLayout("offset V:", comp->offsetUV.y);
		m_offsetVLayout->onValueChange = [comp](float f)
		{
			comp->offsetUV.y = f;
		};

		layout->addRow(m_offsetVLayout);

		setLayout(layout);
	}

	void MeshRendererWidget::Update()
	{
		aw::sdk::MeshRenderer* comp = dynamic_cast<aw::sdk::MeshRenderer*>(m_comp);

		dynamic_cast<ComponentLayout*>(layout())->Update(m_comp);

		auto& tr = comp->meshTransform;

		m_posLayout->Update(tr.GetPosition());
		m_rotLayout->Update(tr.GetRotation().EulerAngles());
		m_scaleLayout->Update(tr.scale);

		// WARNING: DropWidget not Updated.
	}
}