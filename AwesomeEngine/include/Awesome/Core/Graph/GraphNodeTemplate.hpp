#pragma once

#ifndef AWESOME_CORE_GRAPH_NODE_TEMPLATE_GUARD
#define AWESOME_CORE_GRAPH_NODE_TEMPLATE_GUARD

#include <list>

#include <Awesome/Core/Reflexion/Reflect.hpp>

namespace aw
{
	template <typename Node>
	class GraphNodeTemplate
	{
	protected:
		Node* m_parent = nullptr;
		std::list<Node*> m_children;

		inline void PopChild(Node* child);
		inline void FastRemoveChild(Node* child);

	public:
		GraphNodeTemplate() = default;
		inline GraphNodeTemplate(GraphNodeTemplate&& other) noexcept;
		inline GraphNodeTemplate(const GraphNodeTemplate&);
		inline virtual ~GraphNodeTemplate();

		inline Node* GetParent();
		inline const Node* GetParent() const;
		inline typename std::list<Node*>::iterator GetFirstChild();
		inline typename std::list<Node*>::const_iterator GetFirstChild() const;
		inline typename std::list<Node*>::iterator GetLastChild();
		inline typename std::list<Node*>::const_iterator GetLastChild() const;

		inline void AddChild(Node* child);
		inline void RemoveChild(Node* child);

		inline void Pop();
		inline void Swap(GraphNodeTemplate& other);

		GraphNodeTemplate& operator=(GraphNodeTemplate&&) = delete;
		GraphNodeTemplate& operator=(const GraphNodeTemplate&) = delete;

		REFLECT_BEGIN(GraphNodeTemplate)
			REFLECT_MEMBER(m_children)
		REFLECT_END()
	};
}

#include <Awesome/Core/Graph/GraphNodeTemplate.inl>

#endif // GUARD