#include <QtWidgets/qlayout.h>

#include <iostream>

#include <Editor/Widgets/DropWidget.hpp>

namespace ed
{
	DropWidget::DropWidget(QWidget* parent) : QLabel(parent)
	{
		setMinimumSize(0, 20);
		setStyleSheet("QWidget { border: 1px solid #5A5A5A }");
		setAcceptDrops(true);
	}

	void DropWidget::AddExtension(const QString& str)
	{
		if (!m_allowedExt.contains(str, Qt::CaseInsensitive))
			m_allowedExt.push_back(str);
	}

	void DropWidget::RemoveExtension(const QString& str)
	{
		m_allowedExt.removeAll(str);
	}

	void DropWidget::AddSources(QObject* source)
	{
		for (QObject* it : m_allowedSources)
		{
			if (it == source)
				return;
		}

		m_allowedSources.push_back(source);
	}

	void DropWidget::RemoveSources(QObject* source)
	{
		for (std::vector<QObject*>::iterator it = m_allowedSources.begin(); it != m_allowedSources.end(); it++)
		{
			if (*it == source)
			{
				m_allowedSources.erase(it);
				return;
			}
		}
	}

	void DropWidget::dragEnterEvent(QDragEnterEvent* event)
	{
		bool isAllowed = false;

		if (m_allowedSources.size() != 0)
		{
			for (auto curr : m_allowedSources)
			{
				if (event->source() == curr)
				{
					isAllowed = true;
					break;
				}
			}

			if (!isAllowed)
			{
				event->ignore();
				return;
			}

			isAllowed = false;
		}

		if (m_allowedExt.size() != 0)
		{
			QString objName = event->mimeData()->text();

			for (auto curr : m_allowedExt)
			{
				if (objName.endsWith(curr, Qt::CaseInsensitive))
				{
					isAllowed = true;
					break;
				}
			}

			isAllowed ? event->accept() : event->ignore();
		}
		else
			event->accept();
	}

	void DropWidget::dropEvent(QDropEvent* event)
	{
		QString objName = event->mimeData()->text();
		int index = objName.indexOf("assets/");

		if (index != -1)
			objName = objName.remove(0, index + 7); // 7 == "assets/" size

		if (objName.isEmpty() || objName == text())
		{
			event->ignore();
			return;
		}

		setText(objName);

		if (onValueChange)
			onValueChange(objName.toUtf8().data());

		event->accept();
	}
}