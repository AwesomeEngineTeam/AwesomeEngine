#pragma once

#ifndef AWESOME_CORE_SCENE_NODE_TEMPLATE_GUARD
#define AWESOME_CORE_SCENE_NODE_TEMPLATE_GUARD

#include <Awesome/Core/Graph/NamedNodeTemplate.hpp>

#include <atomic>

namespace aw
{
	template <typename Node>
	class SceneNodeTemplate : public NamedNodeTemplate<Node>
	{
		inline void SetParentActive(bool b);

	protected:
		std::atomic_bool m_isHierarchyActive = true;
		std::atomic_bool m_isActive = true;

	public:
		inline SceneNodeTemplate(const std::string& name = defaultName, bool isActive = true);
		inline SceneNodeTemplate(SceneNodeTemplate&& other) noexcept;
		inline SceneNodeTemplate(const SceneNodeTemplate& other);
		~SceneNodeTemplate() override = default;

		inline virtual bool IsActive() const;
		inline virtual void SetActive(bool b = true);

		inline void AddChild(Node* child);

		SceneNodeTemplate& operator=(SceneNodeTemplate&&) = delete;
		SceneNodeTemplate& operator=(const SceneNodeTemplate&) = delete;

		REFLECT_BEGIN(SceneNodeTemplate)
			REFLECT_MEMBER_INHERITANCE(NamedNodeTemplate<Node>)
			REFLECT_MEMBER(m_isHierarchyActive)
			REFLECT_MEMBER(m_isActive)
		REFLECT_END()
	};
}

#include <Awesome/Core/Graph/SceneNodeTemplate.inl>

#endif // GUARD