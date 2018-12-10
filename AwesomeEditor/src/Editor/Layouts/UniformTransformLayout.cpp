#include <Editor/Layouts/UniformTransformLayout.hpp>

namespace ed
{
	UniformTransformLayout::UniformTransformLayout(const aw::UniformTransform& tr, QWidget* parent) :
		QFormLayout(parent),
		m_value{ tr }
	{
		setSpacing(5);

		m_posLayout = new Vector3Layout("Position:", tr.GetPosition());
		m_posLayout->onValueChange = [this](const aw::Vector3& pos)
		{
			m_value.SetPosition(pos);

			if (onValueChange)
				onValueChange(m_value);
		};

		m_rotLayout = new Vector3Layout("Rotation:", tr.GetRotation().EulerAngles());
		m_rotLayout->onValueChange = [this](const aw::Vector3& rot)
		{
			m_value.SetRotation(aw::Quaternion::FromEulerAngles(rot * DEG_TO_RAD));

			if (onValueChange)
				onValueChange(m_value);
		};

		m_scLayout = new FloatLayout("Scale:", tr.scale);
		m_scLayout->onValueChange = [this](float sc)
		{
			m_value.scale = sc;

			if (onValueChange)
				onValueChange(m_value);
		};

		addRow(m_posLayout);
		addRow(m_rotLayout);
		addRow(m_scLayout);
	}

	void UniformTransformLayout::Update(const aw::UniformTransform& tr)
	{
		m_value = tr;

		m_posLayout->Update(m_value.GetPosition());
		m_rotLayout->Update(m_value.GetRotation().EulerAngles());
		m_scLayout->Update(m_value.scale);
	}
}