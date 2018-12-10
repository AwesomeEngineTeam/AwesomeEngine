#pragma once

#ifndef AWESOME_CORE_NAMED_NODE_TEMPLATE_GUARD
#define AWESOME_CORE_NAMED_NODE_TEMPLATE_GUARD

#include <vector>
#include <string>

#include <Awesome/Core/Graph/IdentifiedNodeTemplate.hpp>

#include <Awesome/Core/Config.hpp>

namespace aw
{
	AWESOME_CORE_API extern const std::string defaultName;

	template <typename Node>
	class NamedNodeTemplate : public IdentifiedNodeTemplate<Node>
	{
		inline void FindChildrenWithName(const std::string& name, std::vector<Node*>& buff) const;

	public:
		std::string name;

		inline NamedNodeTemplate(const std::string& _name = defaultName);
		inline NamedNodeTemplate(NamedNodeTemplate&& other) noexcept;
		inline NamedNodeTemplate(const NamedNodeTemplate& other);
		~NamedNodeTemplate() override = default;

		inline Node* FindChildWithName(const std::string& name) const;
		inline std::vector<Node*> FindChildrenWithName(const std::string& name) const;

		NamedNodeTemplate& operator=(NamedNodeTemplate&&) = delete;
		NamedNodeTemplate& operator=(const NamedNodeTemplate&) = delete;

		REFLECT_BEGIN(NamedNodeTemplate)
			REFLECT_MEMBER_INHERITANCE(IdentifiedNodeTemplate<Node>)
			REFLECT_MEMBER(name)
		REFLECT_END()
	};
}

#include <Awesome/Core/Graph/NamedNodeTemplate.inl>

#endif // GUARD