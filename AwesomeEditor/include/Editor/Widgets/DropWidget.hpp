#pragma once

#ifndef AWESOME_EDITOR_WIDGETS_DROP_WIDGET_GUARD
#define AWESOME_EDITOR_WIDGETS_DROP_WIDGET_GUARD

#include <functional>

#include <QtCore/qmimedata.h>
#include <QtCore/qstringlist.h>
#include <QtGui/qevent.h>
#include <QtWidgets/qlabel.h>

namespace ed
{
	class DropWidget : public QLabel
	{
		QStringList m_allowedExt;
		std::vector<QObject*> m_allowedSources;

	public:
		std::function<void(const std::string&)> onValueChange;

		DropWidget(QWidget* parent = nullptr);
		~DropWidget() = default;

		void AddExtension(const QString& str);
		void RemoveExtension(const QString& str);

		void AddSources(QObject* source);
		void RemoveSources(QObject* source);

	protected:
		void dragEnterEvent(QDragEnterEvent* event) override;
		void dropEvent(QDropEvent* event) override;
	};
}

#endif // GUARD