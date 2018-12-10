#include <QtWidgets/qlabel.h>

#include <Editor/Layouts/Vector3Layout.hpp>

namespace ed
{
	Vector3Layout::Vector3Layout(const QString& name, const aw::Vector3& value, float min, float max, QWidget* parent) :
		QFormLayout(parent),
		m_value{ value }
	{
		setSpacing(2);

		addRow(new QLabel(name));

		m_xLayout = new FloatLayout("X:", value.x, min, max);
		m_xLayout->onValueChange = [this](float f)
		{
			m_value.x = f;

			if (onValueChange)
				onValueChange(m_value);
		};

		m_yLayout = new FloatLayout("Y:", value.y, min, max);
		m_yLayout->onValueChange = [this](float f)
		{
			m_value.y = f;

			if (onValueChange)
				onValueChange(m_value);
		};

		m_zLayout = new FloatLayout("Z:", value.z, min, max);
		m_zLayout->onValueChange = [this](float f)
		{
			m_value.z = f;

			if (onValueChange)
				onValueChange(m_value);
		};

		addRow(m_xLayout);
		addRow(m_yLayout);
		addRow(m_zLayout);
	}

	void Vector3Layout::Update(const aw::Vector3& value)
	{
		m_value = value;

		m_xLayout->Update(m_value.x);
		m_yLayout->Update(m_value.y);
		m_zLayout->Update(m_value.z);
	}
}