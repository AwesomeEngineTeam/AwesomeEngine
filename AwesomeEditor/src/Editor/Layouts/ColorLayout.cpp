#include <QtWidgets/qlabel.h>

#include <Editor/Layouts/ColorLayout.hpp>

namespace ed
{
	ColorLayout::ColorLayout(const QString& name, const aw::Color3f& value, QWidget* parent) :
		QFormLayout(parent),
		m_value{ value }
	{
		setSpacing(2);

		addRow(new QLabel(name));

		m_rLayout = new FloatLayout("R:", value.red, 0.0f);
		m_rLayout->onValueChange = [this](float f)
		{
			m_value.red = f;

			if (onValueChange)
				onValueChange(m_value);
		};

		m_gLayout = new FloatLayout("G:", value.green, 0.0f);
		m_gLayout->onValueChange = [this](float f)
		{
			m_value.green = f;

			if (onValueChange)
				onValueChange(m_value);
		};

		m_bLayout = new FloatLayout("B:", value.blue, 0.0f);
		m_bLayout->onValueChange = [this](float f)
		{
			m_value.blue = f;

			if (onValueChange)
				onValueChange(m_value);
		};

		addRow(m_rLayout);
		addRow(m_gLayout);
		addRow(m_bLayout);
	}

	void ColorLayout::Update(const aw::Color3f& value)
	{
		m_value = value;

		m_rLayout->Update(m_value.red);
		m_gLayout->Update(m_value.green);
		m_bLayout->Update(m_value.blue);
	}
}