#include <Editor/Layouts/UIntLayout.hpp>

namespace ed
{
	UIntLayout::UIntLayout(aw::uint32 value, aw::uint32 min, aw::uint32 max, aw::uint32 step, QWidget* parent) : QFormLayout(parent)
	{
		setSpacing(2);

		m_spinBox = new QDoubleSpinBox;

		m_spinBox->setDecimals(0);
		m_spinBox->setSingleStep(step);
		m_spinBox->setRange(min, max);
		m_spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
		m_spinBox->setValue(value);
		connect(m_spinBox, &QDoubleSpinBox::valueChanged, this, &UIntLayout::OnValueChange);
	}

	UIntLayout::UIntLayout(const QString& name, aw::uint32 value, aw::uint32 min, aw::uint32 max, aw::uint32 step, QWidget* parent) : UIntLayout(value, min, max, step, parent)
	{
		addRow(name, m_spinBox);
	}

	UIntLayout::UIntLayout(QLineEdit* fieldName, aw::uint32 value, aw::uint32 min, aw::uint32 max, aw::uint32 step, QWidget* parent) : UIntLayout(value, min, max, step, parent)
	{
		addRow(fieldName, m_spinBox);
	}

	void UIntLayout::OnValueChange(double d)
	{
		if (onValueChange)
			onValueChange(static_cast<aw::uint32>(d));
	}

	void UIntLayout::Update(aw::uint32 value)
	{
		if (static_cast<aw::uint32>(m_spinBox->value()) != value)
		{
			disconnect(m_spinBox, &QDoubleSpinBox::valueChanged, this, &UIntLayout::OnValueChange);

			m_spinBox->setValue(value);

			connect(m_spinBox, &QDoubleSpinBox::valueChanged, this, &UIntLayout::OnValueChange);
		}
	}

	QSize UIntLayout::minimumSize() const
	{
		return QSize(0, 15);
	}
}