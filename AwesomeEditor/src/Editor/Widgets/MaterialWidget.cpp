#include <Editor/Widgets/MaterialWidget.hpp>
#include <Editor/Core/EditorApplication.hpp>
#include <Editor/Layouts/FloatLayout.hpp>
#include <Awesome/Maths/Vector2.hpp>

namespace ed
{
	MaterialLayout::MaterialLayout(ed::Material& material, QWidget* parent) : QFormLayout(parent)
	{
		m_material = material;

		setSpacing(2);

		QLabel* title = new QLabel("Material");
		title->setFont(QFont("Arial", 14, QFont::Bold));
		addRow(title);

		name = new TextLayout("Name:");
		addRow(name);

		aw::Material& mat = material.GetMaterial();

		FloatLayout* m_scaleULayout = new FloatLayout("Scale U:", mat.GetScaleUV().x);
		m_scaleULayout->onValueChange = [this](float f)
		{
			aw::Material& mat = m_material.GetMaterial();

			aw::Vector2 scale = mat.GetScaleUV();
			scale.x = f;
			mat.SetScaleUV(scale);
		};

		addRow(m_scaleULayout);

		FloatLayout* m_scaleVLayout = new FloatLayout("Scale V:", mat.GetScaleUV().y);
		m_scaleVLayout->onValueChange = [this](float f)
		{
			aw::Material& mat = m_material.GetMaterial();

			aw::Vector2 scale = mat.GetScaleUV();
			scale.y = f;
			mat.SetScaleUV(scale);
		};

		addRow(m_scaleVLayout);

		FloatLayout* m_offsetULayout = new FloatLayout("offset U:", mat.GetOffsetUV().x);
		m_offsetULayout->onValueChange = [this](float f)
		{
			aw::Material& mat = m_material.GetMaterial();

			aw::Vector2 offset = mat.GetOffsetUV();
			offset.x = f;
			mat.SetOffsetUV(offset);
		};

		addRow(m_offsetULayout);

		FloatLayout* m_offsetVLayout = new FloatLayout("offset V:", mat.GetOffsetUV().y);
		m_offsetVLayout->onValueChange = [this](float f)
		{
			aw::Material& mat = m_material.GetMaterial();

			aw::Vector2 offset = mat.GetOffsetUV();
			offset.y = f;
			mat.SetOffsetUV(offset);
		};

		addRow(m_offsetVLayout);

	}

	class LitMaterialFormLayout : public MaterialLayout
	{
	public:
		LitMaterialFormLayout(ed::Material& material, QWidget* parent = nullptr) : MaterialLayout(material, parent)
		{
			addRow("Diffuse Map:", material.NewDiffuseDropWidget());
			addRow("Normal Map:", material.NewNormalDropWidget());
			addRow("Specular Map:", material.NewSpecularDropWidget());
			addRow("Height Map:", material.NewHeightDropWidget());
		}
	};

	class UnlitMaterialFormLayout : public MaterialLayout
	{
	public:
		UnlitMaterialFormLayout(ed::Material& material, QWidget* parent = nullptr) : MaterialLayout(material, parent)
		{
			addRow("Diffuse Map:", material.NewDiffuseDropWidget());
		}
	};

	class PBRLitMaterialFormLayout : public MaterialLayout
	{
	public:
		PBRLitMaterialFormLayout(ed::Material& material, QWidget* parent = nullptr) : MaterialLayout(material, parent)
		{
			addRow("Diffuse Map:", material.NewDiffuseDropWidget());
			addRow("Normal Map:", material.NewNormalDropWidget());
			addRow("MetalicRoughness Map:", material.NewMetalicRoughnessDropWidget());
			addRow("Occlusion Map:", material.NewOcclusionDropWidget());
			addRow("Emissive Map:", material.NewEmissiveDropWidget());
		}
	};

	MaterialWidget::MaterialWidget(ed::Material& material, QWidget* parent) : QWidget(parent)
	{
		switch (material.GetType())
		{
		case MaterialType::Lit:
			setLayout(new LitMaterialFormLayout(material));
			break;

		case MaterialType::Unlit:
			setLayout(new UnlitMaterialFormLayout(material));
			break;

		case MaterialType::PBRLit:
			setLayout(new PBRLitMaterialFormLayout(material));
			break;

		default:
			break;
		}
	}
}