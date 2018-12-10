namespace ed
{
	template <typename T>
	void ComponentView::Add()
	{
		if (!m_actor)
			return;

		EditorApplication* editor = dynamic_cast<EditorApplication*>(QApplication::instance());
		QTreeWidget* tree = reinterpret_cast<QTreeWidget*>(widget());

		if (tree->selectedItems().size() == 0)
		{
			T* comp = editor->InstantiateComponent<T>(m_actor);

			if (!comp)
				return;
			else if (tree->topLevelItemCount() > 0)
				AddTreeNode(comp, tree->topLevelItem(0));
			else
				AddTreeNode(comp, nullptr);
		}
		else
		{
			T* comp = editor->InstantiateComponent<T>(m_actor, dynamic_cast<QComponentItem*>(tree->currentItem())->comp);

			if (comp)
				AddTreeNode(comp, tree->currentItem());
		}
	}
}