#pragma once

#ifndef AWESOME_EDITOR_CORE_FOLDER_VIEW_GUARD
#define AWESOME_EDITOR_CORE_FOLDER_VIEW_GUARD

#include <QtWidgets/qtreeview.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qfilesystemmodel.h>
#include <QtWidgets/qdockwidget.h>

namespace ed
{
	class Window;

	class FolderView : public QDockWidget
	{
		class QFolderTree : public QTreeView
		{
		public:
			QFolderTree(QWidget* parent = nullptr);
			~QFolderTree() = default;

			void DoubleClick(const QModelIndex& index);
		};

	public:
		FolderView(Window& win);
		~FolderView() = default;

		const QTreeView* GetTreeSource() const;
	};
}

#endif // GUARD