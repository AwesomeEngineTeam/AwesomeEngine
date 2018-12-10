#include <Editor/Widgets/CameraWidget.hpp>

namespace ed
{
	CameraWidget::CameraWidget(aw::sdk::Camera* comp, QWidget* parent) : ComponentWidget(comp, parent)
	{
		ComponentLayout* layout = new ComponentLayout(comp, "Camera", this);

		m_fov = new FloatLayout("FOV:", comp->GetFOV());
		m_fov->onValueChange = [comp](float f) {comp->SetFOV(f); };

		m_skFov = new FloatLayout("Skybox FOV:", comp->GetSkyboxFOV());
		m_skFov->onValueChange = [comp](float f) {comp->SetSkyboxFOV(f); };

		m_camNear = new FloatLayout("Near:", comp->GetNear());
		m_camNear->onValueChange = [comp](float f) {comp->SetNear(f); };

		m_camFar = new FloatLayout("Far:", comp->GetFar());
		m_camFar->onValueChange = [comp](float f) {comp->SetFar(f); };

		layout->addRow(m_fov);
		layout->addRow(m_skFov);
		layout->addRow(m_camNear);
		layout->addRow(m_camFar);

		setLayout(layout);
	}

	void CameraWidget::Update()
	{
		dynamic_cast<ComponentLayout*>(layout())->Update(m_comp);

		aw::sdk::Camera* comp = dynamic_cast<aw::sdk::Camera*>(m_comp);

		m_fov->Update(comp->GetFOV());
		m_skFov->Update(comp->GetSkyboxFOV());
		m_camNear->Update(comp->GetNear());
		m_camFar->Update(comp->GetFar());
	}
}