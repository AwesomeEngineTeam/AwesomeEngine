namespace aw
{
	template <typename Node>
	inline NamedNodeTemplate<Node>::NamedNodeTemplate(const std::string& _name) : name{ _name }
	{
	}

	template <typename Node>
	inline NamedNodeTemplate<Node>::NamedNodeTemplate(NamedNodeTemplate&& other) noexcept :
		IdentifiedNodeTemplate<Node>(std::move(other)),
		name{std::move(other.name)}
	{
	}

	template <typename Node>
	inline NamedNodeTemplate<Node>::NamedNodeTemplate(const NamedNodeTemplate& other) :
		IdentifiedNodeTemplate<Node>(other),
		name{ other.name }
	{
	}

	template <typename Node>
	inline Node* NamedNodeTemplate<Node>::FindChildWithName(const std::string& name) const
	{
		for (Node* node : this->m_children)
		{
			if (node->name == name)
				return node;

			node->FindChildWithName(name);
		}

		return nullptr;
	}

	template <typename Node>
	inline std::vector<Node*> NamedNodeTemplate<Node>::FindChildrenWithName(const std::string& name) const
	{
		std::vector<Node*> buff;
		FindChildrenWithName(name, buff);

		return buff;
	}

	template <typename Node>
	inline void NamedNodeTemplate<Node>::FindChildrenWithName(const std::string& name, std::vector<Node*>& buff) const
	{
		for (Node* node : this->m_children)
		{
			if (node->name == name)
				buff.push_back(node);

			node->FindChildrenWithName(name, buff);
		}
	}
}