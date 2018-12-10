#include <QtGui/qdesktopservices.h>

#include <Awesome/SDK/Scene/SceneManager.hpp>

#include <Editor/Core/Window.hpp>

namespace ed
{
	FolderView::QFolderTree::QFolderTree(QWidget* parent) : QTreeView(parent)
	{
		QFileSystemModel* fileModel = new QFileSystemModel;
		fileModel->setReadOnly(false);

		setModel(fileModel);
		setRootIndex(fileModel->setRootPath("assets/"));

		hideColumn(3);
		setColumnWidth(0, 400);

		setDragEnabled(true);
		setAcceptDrops(true);
		setDropIndicatorShown(true);
		setDefaultDropAction(Qt::MoveAction);
		setEditTriggers(QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);

		QWidget::connect(this, &QTreeView::doubleClicked, this, &QFolderTree::DoubleClick);
	}

	void FolderView::QFolderTree::DoubleClick(const QModelIndex& index)
	{
		QFileSystemModel* model = dynamic_cast<QFileSystemModel*>(this->model());
		QString path = model->filePath(index);

		if (path.endsWith(SCENE_EXT))
		{
			path = path.remove(0, path.indexOf(SCENE_PATH) + 14); // 14 == "assets/scenes/" size
			aw::sdk::SceneManager::LoadScene(path.remove(SCENE_EXT).toStdString());
		}
		else if(!model->isDir(index))
			QDesktopServices::openUrl(QUrl::fromLocalFile(path));
	}

	FolderView::FolderView(Window& win) : QDockWidget("Folder", &win)
	{
		setObjectName("Folder View");

		setWidget(new QFolderTree);
	}

	const QTreeView* FolderView::GetTreeSource() const
	{
		return reinterpret_cast<QTreeView*>(widget());
	}
}