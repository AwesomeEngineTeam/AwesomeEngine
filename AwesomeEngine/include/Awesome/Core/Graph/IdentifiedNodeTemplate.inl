namespace aw
{
	inline uint32 Identifier::Generate()
	{
		m_mutex.lock();
		if (m_freeIds.size() == 0u)
		{
			m_mutex.unlock();
			return m_currId++;
		}

		uint32 id = m_freeIds.front();
		m_freeIds.pop_front();
		m_mutex.unlock();

		return id;
	}

	inline void Identifier::Reserve(uint32 id)
	{
		m_mutex.lock();
		if (id > m_currId)
		{
			for(; m_currId < id; m_currId++)
				m_freeIds.push_back(m_currId);

			m_currId++;
		}
		else
		{
			for (auto it = m_freeIds.begin(); it != m_freeIds.end(); it++)
			{
				if (*it == id)
				{
					m_freeIds.erase(it);
					m_mutex.unlock();

					return;
				}
			}

			THROW("Identifier: Reserve ID already used!");
		}
		m_mutex.unlock();
	}

	inline void Identifier::Free(uint32 id)
	{
		m_mutex.lock();
		m_freeIds.push_back(id);
		m_mutex.unlock();
	}

	template <typename Node>
	inline IdentifiedNodeTemplate<Node>::IdentifiedNodeTemplate() : id{ s_identifier.Generate() }
	{
	}

	template <typename Node>
	inline IdentifiedNodeTemplate<Node>::IdentifiedNodeTemplate(IdentifiedNodeTemplate<Node>&& other) :
		GraphNodeTemplate<Node>(std::move(other)),
		id{ other.id }
	{
	}

	template <typename Node>
	inline IdentifiedNodeTemplate<Node>::IdentifiedNodeTemplate(const IdentifiedNodeTemplate<Node>& other) :
		GraphNodeTemplate<Node>(other),
		id{ s_identifier.Generate() }
	{
	}

	template <typename Node>
	inline IdentifiedNodeTemplate<Node>::~IdentifiedNodeTemplate()
	{
		s_identifier.Free(id);
	}

	template <typename Node>
	inline Node* IdentifiedNodeTemplate<Node>::FindChildById(uint32 id) const
	{
		for (Node* node : this->m_children)
		{
			if (node->id == id)
				return node;

			if (Node* child = node->FindChildById(id))
				return child;
		}

		return nullptr;
	}
}