#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_VECTOR3_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_VECTOR3_LAYOUT_GUARD

#include <functional>

#include <Awesome/Maths/Vector3.hpp>

#include <Editor/Layouts/FloatLayout.hpp>

namespace ed
{
	class Vector3Layout : public QFormLayout
	{
		aw::Vector3 m_value;

		FloatLayout* m_xLayout = nullptr;
		FloatLayout* m_yLayout = nullptr;
		FloatLayout* m_zLayout = nullptr;

	public:
		std::function<void(aw::Vector3)> onValueChange;

		Vector3Layout(const QString& name = "Position:", const aw::Vector3& value = aw::Vector3(), float min = std::numeric_limits<float>::lowest(), float max = std::numeric_limits<float>::max(), QWidget* parent = nullptr);

		void Update(const aw::Vector3& value);
	};
}

#endif // GUARD