#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_DROP_DOWN_MENU_GUARD
#define AWESOME_EDITOR_LAYOUTS_DROP_DOWN_MENU_GUARD

#include <functional>

#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qformlayout.h>

namespace ed
{
	class DropDownMenu : public QFormLayout
	{
		void OnValueChange(const QString& name);

		QComboBox* m_select;

	public:
		std::function<void(const std::string&)> onValueChange;

		DropDownMenu(const std::string& name, QWidget* parent = nullptr);

		void AddItem(const std::string& name);
		void SetCurrentItem(const std::string& name);
	};
}

#endif // GUARD