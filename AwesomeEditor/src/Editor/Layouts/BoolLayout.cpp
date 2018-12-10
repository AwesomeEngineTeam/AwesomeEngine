#include <Editor/Layouts/BoolLayout.hpp>

namespace ed
{
	BoolLayout::BoolLayout(bool isCheck, QWidget* parent) : QFormLayout(parent)
	{
		setSpacing(2);

		m_checkBox = new QCheckBox;
		m_checkBox->setChecked(isCheck);
		connect(m_checkBox, &QCheckBox::stateChanged, this, &BoolLayout::OnValueChange);
	}

	BoolLayout::BoolLayout(const QString& name, bool isCheck, QWidget* parent) : BoolLayout(isCheck, parent)
	{
		addRow(name, m_checkBox);
	}

	BoolLayout::BoolLayout(QLineEdit* fieldName, bool isCheck, QWidget* parent) : BoolLayout(isCheck, parent)
	{
		addRow(fieldName, m_checkBox);
	}

	void BoolLayout::OnValueChange(bool b)
	{
		if(onValueChange)
			onValueChange(b);
	}

	void BoolLayout::Update(bool isCheck)
	{
		if (m_checkBox->isChecked() != isCheck)
		{
			disconnect(m_checkBox, &QCheckBox::stateChanged, this, &BoolLayout::OnValueChange);

			m_checkBox->setChecked(isCheck);

			connect(m_checkBox, &QCheckBox::stateChanged, this, &BoolLayout::OnValueChange);
		}
	}

	QSize BoolLayout::minimumSize() const
	{
		return QSize(0, 15);
	}
}