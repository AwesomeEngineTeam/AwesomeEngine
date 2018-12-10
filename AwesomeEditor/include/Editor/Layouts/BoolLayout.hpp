#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_BOOL_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_BOOL_LAYOUT_GUARD

#include <functional>

#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qformlayout.h>

namespace ed
{
	class BoolLayout : public QFormLayout
	{
	protected:
		QCheckBox* m_checkBox = nullptr;
		void OnValueChange(bool b);

		BoolLayout(bool isCheck, QWidget* parent);

	public:
		std::function<void(bool)> onValueChange;

		BoolLayout(const QString& name, bool isCheck = true, QWidget* parent = nullptr);
		BoolLayout(QLineEdit* fieldName, bool isCheck = true, QWidget* parent = nullptr);

		void Update(bool isCheck);

		QSize minimumSize() const override;
	};
}

#endif // GUARD