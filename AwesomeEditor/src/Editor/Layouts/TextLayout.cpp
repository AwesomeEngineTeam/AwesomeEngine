#include <Editor/Layouts/TextLayout.hpp>

#include <iostream>

namespace ed
{
	TextLayout::TextLayout(const std::string& text, QWidget* parent)
	{
		setSpacing(2);

		m_qText = new QLineEdit;
		m_qText->setText(text.c_str());

		connect(m_qText, &QLineEdit::editingFinished, this, &TextLayout::OnValueChange);
	}

	TextLayout::TextLayout(const std::string& name, const std::string& text, QWidget* parent) : TextLayout(text, parent)
	{
		addRow(name.c_str(), m_qText);
	}

	TextLayout::TextLayout(QLineEdit* fieldName, const std::string& text, QWidget* parent) : TextLayout(text, parent)
	{
		addRow(fieldName, m_qText);
	}

	void TextLayout::OnValueChange()
	{
		if (onValueChange)
			onValueChange(m_qText->text().toUtf8().data());
	}

	void TextLayout::Update(const std::string& str)
	{
		if (!m_qText->hasFocus() && m_qText->text() != str.c_str())
		{
			disconnect(m_qText, &QLineEdit::editingFinished, this, &TextLayout::OnValueChange);

			m_qText->setText(str.c_str());

			connect(m_qText, &QLineEdit::editingFinished, this, &TextLayout::OnValueChange);
		}
	}

	QSize TextLayout::sizeHint() const
	{
		return QSize(100, 20);
	}
}
