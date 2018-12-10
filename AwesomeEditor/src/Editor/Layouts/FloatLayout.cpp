#include <Awesome/Maths/Algorithm.hpp>

#include <Editor/Layouts/FloatLayout.hpp>

namespace ed
{
	FloatLayout::FloatLayout(float value, float min, float max, float step, QWidget* parent) : QFormLayout(parent)
	{
		setSpacing(2);

		m_spinBox = new QDoubleSpinBox;

		m_spinBox->setDecimals(5);
		m_spinBox->setSingleStep(step);
		m_spinBox->setRange(min, max);
		m_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
		m_spinBox->setValue(value);
		connect(m_spinBox, &QDoubleSpinBox::valueChanged, this, &FloatLayout::OnValueChange);
	}

	FloatLayout::FloatLayout(const QString& name, float value, float min, float max, float step, QWidget* parent) : FloatLayout(value, min, max, step, parent)
	{
		addRow(name, m_spinBox);
	}

	FloatLayout::FloatLayout(QLineEdit* fieldName, float value, float min, float max, float step, QWidget* parent) : FloatLayout(value, min, max, step, parent)
	{
		addRow(fieldName, m_spinBox);
	}

	void FloatLayout::OnValueChange(double d)
	{
		if (onValueChange)
			onValueChange(static_cast<float>(d));
	}

	void FloatLayout::Update(float value)
	{
		if (!aw::NearlyEqual(m_spinBox->value(), value))
		{
			disconnect(m_spinBox, &QDoubleSpinBox::valueChanged, this, &FloatLayout::OnValueChange);

			m_spinBox->setValue(value);

			connect(m_spinBox, &QDoubleSpinBox::valueChanged, this, &FloatLayout::OnValueChange);
		}
	}

	QSize FloatLayout::minimumSize() const
	{
		return QSize(0, 15);
	}
}