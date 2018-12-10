#pragma once

#ifndef AWESOME_EDITOR_LAYOUT_UNIFORM_TRANSFORM_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUT_UNIFORM_TRANSFORM_LAYOUT_GUARD

#include <functional>

#include <Awesome/Maths/UniformTransform.hpp>

#include <Editor/Layouts/Vector3Layout.hpp>

namespace ed
{
	class UniformTransformLayout : public QFormLayout
	{
		aw::UniformTransform m_value;

		Vector3Layout* m_posLayout = nullptr;
		Vector3Layout* m_rotLayout = nullptr;
		FloatLayout* m_scLayout = nullptr;

	public:
		std::function<void(const aw::UniformTransform&)> onValueChange;

		UniformTransformLayout(const aw::UniformTransform& tr, QWidget* parent = nullptr);

		void Update(const aw::UniformTransform& tr);
	};
}

#endif // GUARD