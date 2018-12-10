#include <QtWidgets/qinputdialog.h>

#include <Editor/Core/EditorApplication.hpp>

namespace ed
{
	ComponentView::QComponentTree::QComponentTree(ComponentView& view) :
		QTreeWidget(&view),
		m_view{ view }
	{
		setColumnCount(2);
		setColumnWidth(0, 110);
	}

	void ComponentView::QComponentTree::contextMenuEvent(QContextMenuEvent* event)
	{
		if (m_view.m_isPlaying)
			return;

		event->accept();

		QMenu* menu = new QMenu;
		QMenu* compMenu = menu->addMenu("&Create");

		QAction* compAct = new QAction("&Empty", this);
		compAct->setStatusTip("Create a new empty Component");
		connect(compAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::Component>);
		compMenu->addAction(compAct);

		QAction* camAct = new QAction("&Camera", this);
		camAct->setStatusTip("Create a new Camera Component");
		connect(camAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::Camera>);
		compMenu->addAction(camAct);

		QAction* meshAct = new QAction("&Mesh Renderer", this);
		meshAct->setStatusTip("Create a new Mesh Renderer Component");
		connect(meshAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::MeshRenderer>);
		compMenu->addAction(meshAct);

		QAction* pLiAct = new QAction("&Point Light", this);
		pLiAct->setStatusTip("Create a new Point Light Component");
		connect(pLiAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::PointLight>);
		compMenu->addAction(pLiAct);

		QAction* dLiAct = new QAction("&Directional Light", this);
		dLiAct->setStatusTip("Create a new Directional Light Component");
		connect(dLiAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::DirectionalLight>);
		//dLiAct->setDisabled(true);
		compMenu->addAction(dLiAct);

		QAction* rbAct = new QAction("&Rigidbody", this);
		rbAct->setStatusTip("Create a new Rigidbody Component");
		connect(rbAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::Rigidbody>);
		compMenu->addAction(rbAct);

		QAction* pColAct = new QAction("&Plane Collider", this);
		pColAct->setStatusTip("Create a new Plane Collider Component");
		connect(pColAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::PlaneCollider>);
		compMenu->addAction(pColAct);

		QAction* bColAct = new QAction("&Box Collider", this);
		bColAct->setStatusTip("Create a new Box Collider Component");
		connect(bColAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::BoxCollider>);
		compMenu->addAction(bColAct);

		QAction* sColAct = new QAction("&Sphere Collider", this);
		sColAct->setStatusTip("Create a new Sphere Collider Component");
		connect(sColAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::SphereCollider>);
		compMenu->addAction(sColAct);

		QAction* cColAct = new QAction("&Capsule Collider", this);
		cColAct->setStatusTip("Create a new Capsule Collider Component");
		connect(cColAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::CapsuleCollider>);
		compMenu->addAction(cColAct);

		QAction* scrAct = new QAction("&Script", this);
		scrAct->setStatusTip("Create a new Script Component");
		connect(scrAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::Script>);
		compMenu->addAction(scrAct);

		QAction* sLisAct = new QAction("&Sound Listener", this);
		sLisAct->setStatusTip("Create a new Sound Listener Component");
		connect(sLisAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::SoundListener>);
		compMenu->addAction(sLisAct);

		QAction* sEmAct = new QAction("&Sound Emitter", this);
		sEmAct->setStatusTip("Create a new Sound Emitter Component");
		connect(sEmAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::SoundEmitter>);
		compMenu->addAction(sEmAct);

		QAction* textAct = new QAction("&Text2D", this);
		textAct->setStatusTip("Create a newTExt2D Component");
		connect(textAct, &QAction::triggered, &m_view, &ComponentView::Add<aw::sdk::Text2D>);
		compMenu->addAction(textAct);

		QAction* desAct = new QAction("&Destroy", this);
		desAct->setStatusTip("Delete a Component");
		QWidget::connect(desAct, &QAction::triggered, &m_view, &ComponentView::Remove);
		menu->addAction(desAct);

		menu->addSeparator();

		QAction* cpyAct = new QAction("&Copy", this);
		cpyAct->setStatusTip("Copy a Component");
		QWidget::connect(cpyAct, &QAction::triggered, &m_view, &ComponentView::Copy);
		menu->addAction(cpyAct);

		menu->exec(event->globalPos());
		delete menu;
	}

	ComponentView::QComponentItem::QComponentItem(aw::sdk::Component* _comp, QTreeWidget* view) :
		QTreeWidgetItem(view),
		comp{ _comp }
	{
	}

	ComponentView::QComponentItem::QComponentItem(aw::sdk::Component* _comp, QTreeWidgetItem* parent) :
		QTreeWidgetItem(),
		comp{ _comp }
	{
		parent->addChild(this);
	}

	ComponentView::ComponentView(Window& win) :
		QDockWidget("Component", &win),
		m_inspec{ win.inspec }
	{
		setObjectName("Component View");

		QTreeWidget* tree = new QComponentTree(*this);
		setWidget(tree);

		QTreeWidget::connect(tree, &QTreeWidget::clicked, this, &ComponentView::Click);
	}

	void ComponentView::Clear()
	{
		reinterpret_cast<QTreeWidget*>(widget())->clear();
		m_inspec.Clear();
	}

	void ComponentView::Click(const QModelIndex &index)
	{
		m_inspec.Inspect(reinterpret_cast<QComponentItem*>(index.internalPointer())->comp);
	}

	void ComponentView::RenameComponent(aw::sdk::Component* comp, const std::string& name)
	{
		QTreeWidgetItem* item = FindItemFromComponent(comp);

		if (item)
			item->setText(1, name.c_str());
	}

	void ComponentView::SetOnGame(bool b)
	{
		m_isPlaying = b;

		if (b)
		{
			aw::sdk::Actor::onAddComponent = [this](aw::sdk::Actor* actor) { if (actor == m_actor) SetSelectedActor(actor); };
			aw::sdk::Actor::onDestroyComponent = [this](aw::sdk::Actor* actor) { if (actor == m_actor) SetSelectedActor(actor); };
		}
		else
		{
			aw::sdk::Actor::onAddComponent = nullptr;
			aw::sdk::Actor::onDestroyComponent = nullptr;
		}
	}

	QTreeWidgetItem* ComponentView::FindItemRecur(aw::sdk::Component* comp, QComponentItem* item)
	{
		if (comp == item->comp)
			return item;

		for (int i = 0; i < item->childCount(); i++)
		{
			auto res = FindItemRecur(comp, dynamic_cast<QComponentItem*>(item->child(i)));

			if (res)
				return res;
		}

		return nullptr;
	}

	QTreeWidgetItem* ComponentView::FindItemFromComponent(aw::sdk::Component* comp)
	{
		if (!comp)
			return nullptr;

		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		for (auto it = tree->topLevelItem(0); it != nullptr; it = tree->itemBelow(it))
		{
			auto res = FindItemRecur(comp, dynamic_cast<QComponentItem*>(it));

			if (res)
				return res;
		}

		return nullptr;
	}

	QTreeWidgetItem* ComponentView::AddTreeNode(aw::sdk::Component* comp, QTreeWidgetItem* parent)
	{
		QTreeWidgetItem* treeItem = parent ? new QComponentItem(comp, parent) : new QComponentItem(comp, reinterpret_cast<QTreeWidget*>(widget()));
		treeItem->setText(0, comp->GetComponentTypeName().c_str());
		treeItem->setText(1, comp->name.c_str());

		return treeItem;
	}

	void ComponentView::SetSelectedActor(aw::sdk::Actor* actor, aw::sdk::Component* selectedComp)
	{
		if (!actor)
			return;

		m_actor = actor;

		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());
		tree->clear();

		if (actor->GetRootComponent())
		{
			ProcessComponents(actor->GetRootComponent());

			if (selectedComp)
			{
				m_inspec.Inspect(selectedComp);
				FindItemFromComponent(selectedComp)->setSelected(true);
			}
			else
			{
				m_inspec.Inspect(actor->GetRootComponent());
				tree->topLevelItem(0)->setSelected(true);
			}

			tree->expandAll();
		}
		else
			m_inspec.Clear();

		raise();
	}

	void ComponentView::ProcessComponents(aw::sdk::Component* comp, QTreeWidgetItem* parent)
	{
		if (comp->isImplicit)
			return;

		parent = AddTreeNode(comp, parent);

		for (auto it = comp->GetFirstChild(); it != comp->GetLastChild(); it++)
			ProcessComponents(*it, parent);
	}

	void ComponentView::RemoveComponent(QTreeWidgetItem* node)
	{
		if (node->parent())
			node->parent()->removeChild(node);
		else
		{
			QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());
			delete tree->takeTopLevelItem(tree->indexOfTopLevelItem(node));
		}
	}

	void ComponentView::Remove()
	{
		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() != 0)
		{
			m_inspec.Clear();

			QComponentItem* compIt = dynamic_cast<QComponentItem*>(tree->currentItem());
			editor->Destroy(m_actor, compIt->comp); // delete component before
			RemoveComponent(compIt);
		}
	}

	void ComponentView::Copy()
	{
		QTreeWidget* tree = dynamic_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() != 0)
			dynamic_cast<EditorApplication*>(QApplication::instance())->copiedComponent = dynamic_cast<QComponentItem*>(tree->currentItem())->comp;
	}
}