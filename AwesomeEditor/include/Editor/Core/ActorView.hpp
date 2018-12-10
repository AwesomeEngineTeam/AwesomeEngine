#pragma once

#ifndef AWESOME_EDITOR_CORE_ACTOR_VIEW_GUARD
#define AWESOME_EDITOR_CORE_ACTOR_VIEW_GUARD

#include <Editor/Core/ComponentView.hpp>

namespace aw
{
	namespace sdk
	{
		class Scene;
	}
}

namespace ed
{
	class Window;
	class FolderView;

	class ActorView : public QDockWidget
	{
		struct QActorTree : public QTreeWidget
		{
			ActorView& m_actView;

			QActorTree(ActorView& actview);
			~QActorTree() = default;

		protected:
			void mousePressEvent(QMouseEvent* event) override;
			void contextMenuEvent(QContextMenuEvent* event) override;
		};

		struct QActorItem : public QTreeWidgetItem
		{
			aw::sdk::Actor* actor = nullptr;

			QActorItem(aw::sdk::Actor* _actor, QTreeWidget* view);
			QActorItem(aw::sdk::Actor* _actor, QTreeWidgetItem* parent);
			~QActorItem() = default;
		};

		void DoubleClick(const QModelIndex &index);

		QTreeWidgetItem* FindItemRecur(aw::sdk::Actor* actor, QActorItem* item);
		QTreeWidgetItem* FindItemFromActor(aw::sdk::Actor* actor);
		void ProcessActors(aw::sdk::Actor* actor, QTreeWidgetItem* parent = nullptr);
		QTreeWidgetItem* AddTreeNode(aw::sdk::Actor* actor, QTreeWidgetItem* parent = nullptr);
		void AddActor(aw::sdk::Actor* actor, aw::sdk::Actor* parent);
		void RemoveActor(aw::sdk::Actor* actor);
		void RemoveActor(QTreeWidgetItem* node);

		FolderView& m_folderView;
		ComponentView& m_compView;
		bool m_isPlaying = false;

	public:
		ActorView(Window& win);
		~ActorView() = default;

		void SetSelectedScene(aw::sdk::Scene* scene);
		void SetSelectedActor(aw::sdk::Actor* actor);

		void SetOnGame(bool b);

		void Add();
		void AddQuad();
		void AddCube();
		void AddSphere();
		void AddCapsule();
		void AddCylinder();
		void AddCone();
		void Remove();

		void RenameActor(aw::sdk::Actor* actor, const std::string& str);

	protected:
		void dragEnterEvent(QDragEnterEvent* event) override;
		void dropEvent(QDropEvent* event) override;
	};
}

#endif // GUARD