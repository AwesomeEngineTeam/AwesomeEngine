#pragma once

#ifndef AWESOME_EDITOR_CORE_COMPONENT_VIEW_GUARD
#define AWESOME_EDITOR_CORE_COMPONENT_VIEW_GUARD

#include <QtWidgets/qmenu.h>
#include <QtWidgets/qtreewidget.h>
#include <QtWidgets/qdockwidget.h>

#include <Awesome/SDK/Scene/Actor.hpp>

#include <Editor/Core/Inspector.hpp>

namespace ed
{
	class EditorApplication;
	class Window;

	class ComponentView : public QDockWidget
	{
		class QComponentTree : public QTreeWidget
		{
			ComponentView& m_view;

		public:
			QComponentTree(ComponentView& view);

		protected:
			void contextMenuEvent(QContextMenuEvent* event) override;
		};

		struct QComponentItem : public QTreeWidgetItem
		{
			aw::sdk::Component* comp;

			QComponentItem(aw::sdk::Component* _comp, QTreeWidget* view);
			QComponentItem(aw::sdk::Component* _comp, QTreeWidgetItem* parent);
			~QComponentItem() = default;
		};

		QTreeWidgetItem* FindItemRecur(aw::sdk::Component* comp, QComponentItem* item);
		QTreeWidgetItem* FindItemFromComponent(aw::sdk::Component* comp);
		void ProcessComponents(aw::sdk::Component* comp, QTreeWidgetItem* parent = nullptr);
		QTreeWidgetItem* AddTreeNode(aw::sdk::Component* comp, QTreeWidgetItem* parent);
		void RemoveComponent(QTreeWidgetItem* node);

		void Click(const QModelIndex &index);

		Inspector& m_inspec;

		aw::sdk::Actor* m_actor = nullptr;

		bool m_isPlaying = false;

	public:
		ComponentView(Window& win);
		~ComponentView() = default;

		void SetSelectedActor(aw::sdk::Actor* actor, aw::sdk::Component* selectedComp = nullptr);

		void Clear();
		
		void RenameComponent(aw::sdk::Component* comp, const std::string& name);

		void SetOnGame(bool b);

		template <typename T>
		void Add();
		void Remove();
		void Copy();
	};
}

#include <Editor/Core/ComponentView.inl>

#endif // GUARD