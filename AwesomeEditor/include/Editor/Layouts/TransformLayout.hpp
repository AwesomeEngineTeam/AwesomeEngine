#pragma once

#ifndef AWESOME_EDITOR_LAYOUT_TRANSFORM_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUT_TRANSFORM_LAYOUT_GUARD

#include <functional>

#include <Awesome/Maths/Transform.hpp>

#include <Editor/Layouts/Vector3Layout.hpp>

namespace ed
{
	class TransformLayout : public QFormLayout
	{
		aw::Transform m_value;

		Vector3Layout* m_posLayout = nullptr;
		Vector3Layout* m_rotLayout = nullptr;
		Vector3Layout* m_scLayout = nullptr;

	public:
		std::function<void(const aw::Transform&)> onValueChange;

		TransformLayout(const aw::Transform& tr, QWidget* parent = nullptr);

		void Update(const aw::Transform& tr);
	};
}

#endif // GUARD