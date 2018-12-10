#include <Editor/Layouts/DropDownMenu.hpp>

namespace ed
{
	DropDownMenu::DropDownMenu(const std::string& name, QWidget* parent) : QFormLayout(parent)
	{
		setSpacing(2);

		m_select = new QComboBox;

		connect(m_select, &QComboBox::currentIndexChanged, this, &DropDownMenu::OnValueChange);

		addRow(name.c_str() , m_select);
	}

	void DropDownMenu::OnValueChange(const QString& name)
	{
		if (onValueChange)
			onValueChange(name.toUtf8().data());
	}

	void DropDownMenu::AddItem(const std::string& name)
	{
		m_select->addItem(name.c_str());
	}

	void DropDownMenu::SetCurrentItem(const std::string& name)
	{
		if (m_select->currentText() != name.c_str())
		{
			disconnect(m_select, &QComboBox::currentIndexChanged, this, &DropDownMenu::OnValueChange);

			m_select->setCurrentText(name.c_str());

			connect(m_select, &QComboBox::currentIndexChanged, this, &DropDownMenu::OnValueChange);
		}
	}
}