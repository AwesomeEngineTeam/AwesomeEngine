#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_TEXT_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_TEXT_LAYOUT_GUARD

#include <functional>

#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qformlayout.h>

namespace ed
{
	class TextLayout : public QFormLayout
	{
		QLineEdit* m_qText = nullptr;

		void OnValueChange();

		TextLayout(const std::string& text, QWidget* parent);

	public:
		std::function<void(const std::string& str)> onValueChange;

		TextLayout(const std::string& name, const std::string& text = "Text", QWidget* parent = nullptr);
		TextLayout(QLineEdit* fieldName, const std::string& text = "Text", QWidget* parent = nullptr);

		void Update(const std::string& str);

		QSize sizeHint() const override;
	};
}

#endif // GUARD