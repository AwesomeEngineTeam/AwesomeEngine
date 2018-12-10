#include <QtWidgets/qinputdialog.h>
#include <Editor/Core/EditorApplication.hpp>

namespace ed
{
	ActorView::QActorTree::QActorTree(ActorView& view) : m_actView{ view }
	{
		setColumnCount(2);
		setColumnWidth(0, 70);
	}

	void ActorView::QActorTree::mousePressEvent(QMouseEvent* event)
	{
		clearSelection();
		m_actView.m_compView.Clear();

		QTreeWidget::mousePressEvent(event);
	}

	void ActorView::QActorTree::contextMenuEvent(QContextMenuEvent* event)
	{
		if (m_actView.m_isPlaying)
			return;

		QMenu* menu = new QMenu;
		QMenu* creMenu = menu->addMenu("&Create");

		QAction* empAct = new QAction("&Empty", this);
		empAct->setStatusTip("Create a new empty Actor");
		connect(empAct, &QAction::triggered, &m_actView, &ActorView::Add);
		creMenu->addAction(empAct);

		QAction* quadAct = new QAction("&Quad", this);
		quadAct->setStatusTip("Create a new Quad Actor");
		connect(quadAct, &QAction::triggered, &m_actView, &ActorView::AddQuad);
		creMenu->addAction(quadAct);

		QAction* cubeAct = new QAction("&Cube", this);
		cubeAct->setStatusTip("Create a new Cube Actor");
		connect(cubeAct, &QAction::triggered, &m_actView, &ActorView::AddCube);
		creMenu->addAction(cubeAct);

		QAction* sphAct = new QAction("&Sphere", this);
		sphAct->setStatusTip("Create a new Cube Actor");
		connect(sphAct, &QAction::triggered, &m_actView, &ActorView::AddSphere);
		creMenu->addAction(sphAct);

		QAction* capAct = new QAction("&Capsule", this);
		capAct->setStatusTip("Create a new Cube Actor");
		connect(capAct, &QAction::triggered, &m_actView, &ActorView::AddCapsule);
		creMenu->addAction(capAct);

		QAction* cylAct = new QAction("&Cylinder", this);
		cylAct->setStatusTip("Create a new Cube Actor");
		connect(cylAct, &QAction::triggered, &m_actView, &ActorView::AddCylinder);
		creMenu->addAction(cylAct);

		QAction* coneAct = new QAction("&Cone", this);
		coneAct->setStatusTip("Create a new Cube Actor");
		connect(coneAct, &QAction::triggered, &m_actView, &ActorView::AddCone);
		creMenu->addAction(coneAct);

		QAction* desAct = new QAction("&Destroy", this);
		desAct->setShortcut(QKeySequence::Delete);
		desAct->setStatusTip("Delete an Actor");
		connect(desAct, &QAction::triggered, &m_actView, &ActorView::Remove);
		menu->addAction(desAct);

		menu->exec(event->globalPos());
		delete menu;
	}

	inline ActorView::QActorItem::QActorItem(aw::sdk::Actor* _actor, QTreeWidget* view) :
		QTreeWidgetItem(view),
		actor{ _actor }
	{
	}

	inline ActorView::QActorItem::QActorItem(aw::sdk::Actor* _actor, QTreeWidgetItem* parent) :
		QTreeWidgetItem(),
		actor{ _actor }
	{
		parent->addChild(this);
	}

	ActorView::ActorView(Window& win) :
		QDockWidget("Actor", &win),
		m_folderView{ win.folderView },
		m_compView{ win.compView }
	{
		setObjectName("Actor View");

		QTreeWidget* tree = new QActorTree(*this);

		setWidget(tree);
		setAcceptDrops(true);

		QTreeWidget::connect(tree, &QTreeWidget::doubleClicked, this, &ActorView::DoubleClick);

		aw::sdk::SceneManager::CurrentScene->onSceneStart = [this](aw::sdk::Scene* scene) { SetSelectedScene(scene); };
	}

	void ActorView::SetSelectedScene(aw::sdk::Scene* scene)
	{
		if (!scene)
			return;

		m_compView.Clear();

		reinterpret_cast<QTreeWidget*>(widget())->clear(); // tree->clear()

		for (auto it = scene->GetFirstActor(); it != scene->GetLastActor(); it++)
			ProcessActors(*it);

		raise();
	}

	void ActorView::SetSelectedActor(aw::sdk::Actor* actor)
	{
		ASSERT(actor == nullptr, "actor is nullptr");

		reinterpret_cast<QTreeWidget*>(widget())->clearSelection();
		FindItemFromActor(actor)->setSelected(true);
	}

	void ActorView::DoubleClick(const QModelIndex &index)
	{
		m_compView.SetSelectedActor(reinterpret_cast<QActorItem*>(index.internalPointer())->actor);
	}

	void ActorView::ProcessActors(aw::sdk::Actor* actor, QTreeWidgetItem* parent)
	{
		parent = AddTreeNode(actor, parent);

		for (auto it = actor->GetFirstChild(); it != actor->GetLastChild(); it++)
			ProcessActors(*it, parent);
	}

	QTreeWidgetItem* ActorView::FindItemRecur(aw::sdk::Actor* actor, QActorItem* item)
	{
		if (actor == item->actor)
			return item;

		for (int i = 0; i < item->childCount(); i++)
		{
			auto res = FindItemRecur(actor, dynamic_cast<QActorItem*>(item->child(i)));

			if (res)
				return res;
		}

		return nullptr;
	}

	QTreeWidgetItem* ActorView::FindItemFromActor(aw::sdk::Actor* actor)
	{
		if (!actor)
			return nullptr;

		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		for (auto it = tree->topLevelItem(0); it != nullptr; it = tree->itemBelow(it))
		{
			auto res = FindItemRecur(actor, dynamic_cast<QActorItem*>(it));

			if (res)
				return res;
		}

		return nullptr;
	}

	QTreeWidgetItem* ActorView::AddTreeNode(aw::sdk::Actor* actor, QTreeWidgetItem* parent)
	{
		QTreeWidgetItem* treeItem = parent ? new QActorItem(actor, parent) : new QActorItem(actor, reinterpret_cast<QTreeWidget*>(widget()));
		treeItem->setText(0, "Actor"); // only Actor it this thime
		treeItem->setText(1, actor->name.c_str());

		return treeItem;
	}

	void ActorView::AddActor(aw::sdk::Actor* actor, aw::sdk::Actor* parent)
	{
		AddTreeNode(actor, FindItemFromActor(parent));
	}

	void ActorView::RemoveActor(aw::sdk::Actor* actor)
	{
		RemoveActor(FindItemFromActor(actor));
	}

	void ActorView::RemoveActor(QTreeWidgetItem* node)
	{
		if (node == nullptr)
			return;

		if (node->parent())
			node->parent()->removeChild(node);
		else
		{
			QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());
			delete tree->takeTopLevelItem(tree->indexOfTopLevelItem(node));
		}
	}

	void ActorView::SetOnGame(bool b)
	{
		m_isPlaying = b;

		if (b)
		{
			aw::sdk::Scene::onCreateActor = [this](aw::sdk::Actor* actor, aw::sdk::Actor* parent) { AddActor(actor, parent); };
			aw::sdk::Scene::onDestroyActor = [this](aw::sdk::Actor* actor)
			{
				m_compView.Clear();
				RemoveActor(actor);
			};
		}
		else
		{
			aw::sdk::Scene::onCreateActor = nullptr;
			aw::sdk::Scene::onDestroyActor = nullptr;
		}
	}

	void ActorView::Add()
	{
		QInputDialog dialog;

		if (!dialog.exec())
			return;

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() == 0)
			AddTreeNode(editor->InstantiateActor(dialog.textValue().toStdString()), nullptr);
		else
			AddTreeNode(editor->InstantiateActor(dialog.textValue().toStdString(), dynamic_cast<QActorItem*>(tree->currentItem())->actor), tree->currentItem());
	}

	void ActorView::AddQuad()
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() == 0)
			AddTreeNode(editor->InstantiateQuad(), nullptr);
		else
			AddTreeNode(editor->InstantiateQuad(dynamic_cast<QActorItem*>(tree->currentItem())->actor), tree->currentItem());
	}

	void ActorView::AddCube()
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() == 0)
			AddTreeNode(editor->InstantiateCube(), nullptr);
		else
			AddTreeNode(editor->InstantiateCube(dynamic_cast<QActorItem*>(tree->currentItem())->actor), tree->currentItem());
	}

	void ActorView::AddSphere()
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() == 0)
			AddTreeNode(editor->InstantiateSphere(), nullptr);
		else
			AddTreeNode(editor->InstantiateSphere(dynamic_cast<QActorItem*>(tree->currentItem())->actor), tree->currentItem());
	}

	void ActorView::AddCapsule()
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() == 0)
			AddTreeNode(editor->InstantiateCapsule(), nullptr);
		else
			AddTreeNode(editor->InstantiateCapsule(dynamic_cast<QActorItem*>(tree->currentItem())->actor), tree->currentItem());
	}

	void ActorView::AddCylinder()
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() == 0)
			AddTreeNode(editor->InstantiateCylinder(), nullptr);
		else
			AddTreeNode(editor->InstantiateCylinder(dynamic_cast<QActorItem*>(tree->currentItem())->actor), tree->currentItem());
	}

	void ActorView::AddCone()
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() == 0)
			AddTreeNode(editor->InstantiateCone(), nullptr);
		else
			AddTreeNode(editor->InstantiateCone(dynamic_cast<QActorItem*>(tree->currentItem())->actor), tree->currentItem());
	}

	void ActorView::Remove()
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() != 0)
		{
			QActorItem* act = dynamic_cast<QActorItem*>(tree->currentItem());

			editor->Destroy(act->actor); // delete actor before
			RemoveActor(act);

			m_compView.Clear();
		}
	}

	void ActorView::RenameActor(aw::sdk::Actor* actor, const std::string& name)
	{
		QTreeWidgetItem* item = FindItemFromActor(actor);

		if (item)
			item->setText(1, name.c_str());
	}

	void ActorView::dragEnterEvent(QDragEnterEvent* event)
	{
		if (event->source() == m_folderView.GetTreeSource())
		{
			QString objName = event->mimeData()->text();

			if (objName.endsWith(".gltf", Qt::CaseInsensitive) || objName.endsWith(".obj", Qt::CaseInsensitive))
				event->accept();
		}
		else
			event->ignore();
	}

	void ActorView::dropEvent(QDropEvent* event)
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());

		QString path = event->mimeData()->text();
		path = path.remove(0, path.indexOf("assets/"));

		AddTreeNode(editor->Import(path.toStdString()));

		event->accept();
	}
}