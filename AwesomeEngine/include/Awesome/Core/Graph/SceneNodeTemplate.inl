namespace aw
{
	template <typename Node>
	inline SceneNodeTemplate<Node>::SceneNodeTemplate(const std::string& name, bool isActive) :
		NamedNodeTemplate<Node>(name),
		m_isActive{ isActive }
	{
	}

	template <typename Node>
	inline SceneNodeTemplate<Node>::SceneNodeTemplate(SceneNodeTemplate<Node>&& other) noexcept :
		NamedNodeTemplate<Node>(std::move(other)),
		m_isHierarchyActive{ other.m_isHierarchyActive.load() },
		m_isActive{ other.m_isActive.load() }
	{
	}

	template <typename Node>
	inline SceneNodeTemplate<Node>::SceneNodeTemplate(const SceneNodeTemplate<Node>& other) :
		NamedNodeTemplate<Node>(other),
		m_isHierarchyActive{ other.m_isHierarchyActive.load() },
		m_isActive{ other.m_isActive.load() }
	{
	}

	template <typename Node>
	inline bool SceneNodeTemplate<Node>::IsActive() const
	{
		return m_isActive && m_isHierarchyActive;
	}

	template <typename Node>
	inline void SceneNodeTemplate<Node>::SetActive(bool b)
	{
		m_isActive = b;

		for (Node* node : this->m_children)
			node->SetParentActive(IsActive());
	}

	template <typename Node>
	inline void SceneNodeTemplate<Node>::SetParentActive(bool b)
	{
		m_isHierarchyActive = b;

		for (Node* node : this->m_children)
			node->SetParentActive(IsActive());
	}

	template <typename Node>
	inline void SceneNodeTemplate<Node>::AddChild(Node* child)
	{
		NamedNodeTemplate<Node>::AddChild(child);

		child->SetParentActive(IsActive());
	}
}