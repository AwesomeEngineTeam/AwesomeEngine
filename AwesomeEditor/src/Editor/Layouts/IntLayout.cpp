#include <Editor/Layouts/IntLayout.hpp>

namespace ed
{
	IntLayout::IntLayout(int value, int min, int max, int step, QWidget* parent) : QFormLayout(parent)
	{
		setSpacing(2);

		m_spinBox = new QSpinBox;
		m_spinBox->setRange(min, max);
		m_spinBox->setSingleStep(step);
		m_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
		m_spinBox->setValue(value);
		connect(m_spinBox, &QSpinBox::valueChanged, this, &IntLayout::OnValueChange);
	}

	IntLayout::IntLayout(const QString& name, int value, int min, int max, int step, QWidget* parent) : IntLayout(value, min, max, step, parent)
	{
		addRow(name, m_spinBox);
	}

	IntLayout::IntLayout(QLineEdit* fieldName, int value, int min, int max, int step, QWidget* parent) : IntLayout(value, min, max, step, parent)
	{
		addRow(fieldName, m_spinBox);
	}

	void IntLayout::OnValueChange(int i)
	{
		if (onValueChange)
			onValueChange(i);
	}

	void IntLayout::Update(int value)
	{
		if (m_spinBox->value() != value)
		{
			disconnect(m_spinBox, &QSpinBox::valueChanged, this, &IntLayout::OnValueChange);

			m_spinBox->setValue(value);

			connect(m_spinBox, &QSpinBox::valueChanged, this, &IntLayout::OnValueChange);
		}
	}

	QSize IntLayout::minimumSize() const
	{
		return QSize(0, 15);
	}
}