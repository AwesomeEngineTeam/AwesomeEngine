#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_COLOR_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_COLOR_LAYOUT_GUARD

#include <functional>

#include <Awesome/Core/Color.hpp>

#include <Editor/Layouts/FloatLayout.hpp>

namespace ed
{
	class ColorLayout : public QFormLayout
	{
		aw::Color3f m_value;

		FloatLayout* m_rLayout = nullptr;
		FloatLayout* m_gLayout = nullptr;
		FloatLayout* m_bLayout = nullptr;

	public:
		std::function<void(aw::Color3f)> onValueChange;

		ColorLayout(const QString& name = "Position:", const aw::Color3f& value = aw::Color3f(), QWidget* parent = nullptr);

		void Update(const aw::Color3f& value);
	};
}

#endif // GUARD