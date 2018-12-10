#pragma once

#ifndef AWESOME_CORE_IDENTIFIED_NODE_TEMPLATE_GUARD
#define AWESOME_CORE_IDENTIFIED_NODE_TEMPLATE_GUARD

#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Debug.hpp>

#include <Awesome/Core/Graph/GraphNodeTemplate.hpp>

#include <atomic>
#include <mutex>

namespace aw
{
	class Identifier
	{
		std::atomic_uint32_t m_currId = 0u;
		std::list<uint32> m_freeIds;

		std::mutex m_mutex;

	public:
		inline uint32 Generate();
		inline void Reserve(uint32 id);
		inline void Free(uint32 id);
	};

	template <typename Node>
	class IdentifiedNodeTemplate : public GraphNodeTemplate<Node>
	{
		static Identifier s_identifier;

	public:
		const uint32 id;

		inline IdentifiedNodeTemplate();
		inline IdentifiedNodeTemplate(IdentifiedNodeTemplate&& other);
		inline IdentifiedNodeTemplate(const IdentifiedNodeTemplate& other);
		inline ~IdentifiedNodeTemplate() override;

		inline Node* FindChildById(uint32 id) const;

		IdentifiedNodeTemplate& operator=(IdentifiedNodeTemplate&&) = delete;
		IdentifiedNodeTemplate& operator=(const IdentifiedNodeTemplate&) = delete;

		REFLECT_BEGIN(IdentifiedNodeTemplate)
			REFLECT_MEMBER_INHERITANCE(GraphNodeTemplate<Node>)
			REFLECT_MEMBER(id)
		REFLECT_END()
	};

	template <typename Node>
	Identifier IdentifiedNodeTemplate<Node>::s_identifier;
}

#include <Awesome/Core/Graph/IdentifiedNodeTemplate.inl>

#endif // GUARD