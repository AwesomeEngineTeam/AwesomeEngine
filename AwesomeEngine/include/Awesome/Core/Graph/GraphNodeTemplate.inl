namespace aw
{
	template <typename Node>
	inline GraphNodeTemplate<Node>::GraphNodeTemplate(GraphNodeTemplate<Node>&& other) noexcept : m_children{ std::move(other.m_children) }
	{
		if (other.m_parent)
		{
			other.m_parent->AddChild(this);
			other.Pop();
		}

		for (Node* node : m_children)
			node->m_parent = this;
	}

	template <typename Node>
	inline GraphNodeTemplate<Node>::GraphNodeTemplate(const GraphNodeTemplate<Node>& other)
	{
		for (Node* node : m_children)
			AddChild(new Node(*node));
	}

	template <typename Node>
	inline GraphNodeTemplate<Node>::~GraphNodeTemplate()
	{
		for (Node* node : m_children)
			delete node;
	}

	template <typename Node>
	inline Node* GraphNodeTemplate<Node>::GetParent()
	{
		return m_parent;
	}

	template <typename Node>
	inline const Node* GraphNodeTemplate<Node>::GetParent() const
	{
		return m_parent;
	}

	template <typename Node>
	inline typename std::list<Node*>::iterator GraphNodeTemplate<Node>::GetFirstChild()
	{
		return m_children.begin();
	}

	template <typename Node>
	inline typename std::list<Node*>::const_iterator GraphNodeTemplate<Node>::GetFirstChild() const
	{
		return m_children.begin();
	}

	template <typename Node>
	inline typename std::list<Node*>::iterator GraphNodeTemplate<Node>::GetLastChild()
	{
		return m_children.end();
	}

	template <typename Node>
	inline typename std::list<Node*>::const_iterator GraphNodeTemplate<Node>::GetLastChild() const
	{
		return m_children.end();
	}

	template <typename Node>
	inline void GraphNodeTemplate<Node>::PopChild(Node* child)
	{
		for (auto it = m_children.begin(); it != m_children.end(); it++)
		{
			if (*it == child)
			{
				m_children.erase(it);
				return;
			}
		}
	}

	template <typename Node>
	inline void GraphNodeTemplate<Node>::FastRemoveChild(Node* child)
	{
		for (auto it = m_children.begin(); it != m_children.end(); it++)
		{
			if (*it == child)
			{
				delete *it;
				m_children.erase(it);
				return;
			}

			(*it)->FastRemoveChild(child);
		}
	}

	template <typename Node>
	inline void GraphNodeTemplate<Node>::AddChild(Node* child)
	{
		if (!child || child->m_parent != nullptr)
			return;

		// Already Child ?
		for (Node* node : m_children)
		{
			if (node == child)
				return;
		}

		child->m_parent = reinterpret_cast<Node*>(this);
		m_children.push_back(child);
	}

	template <typename Node>
	inline void GraphNodeTemplate<Node>::RemoveChild(Node* child)
	{
		if (!child || child->m_parent == nullptr || m_children.empty())
			return;

		FastRemoveChild(child);
	}

	template <typename Node>
	inline void GraphNodeTemplate<Node>::Pop()
	{
		if (!m_parent)
			return;

		m_parent->PopChild(dynamic_cast<Node*>(this));
		m_parent = nullptr;
	}

	template <typename Node>
	inline void GraphNodeTemplate<Node>::Swap(GraphNodeTemplate<Node>& other)
	{
		GraphNodeTemplate<Node>* parent1 = m_parent;
		GraphNodeTemplate<Node>* parent2 = other.m_parent;

		Pop();
		other.Pop();

		parent2->AddChild(dynamic_cast<Node*>(this));
		parent1->AddChild(dynamic_cast<Node*>(&other));
	}
}