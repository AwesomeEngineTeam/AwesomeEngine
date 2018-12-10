#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_FLOAT_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_FLOAT_LAYOUT_GUARD

#include <functional>

#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qformlayout.h>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace ed
{
	class FloatLayout : public QFormLayout
	{
	protected:
		QDoubleSpinBox* m_spinBox = nullptr;

		void OnValueChange(double d);

		FloatLayout(float value, float min, float max, float step, QWidget* parent);

	public:
		std::function<void(float)> onValueChange;

		FloatLayout(const QString& name, float value = 0.0f, float min = std::numeric_limits<float>::lowest(), float max = std::numeric_limits<float>::max(), float step = 1.0f, QWidget* parent = nullptr);
		FloatLayout(QLineEdit* fieldName, float value = 0.0f, float min = std::numeric_limits<float>::lowest(), float max = std::numeric_limits<float>::max(), float step = 1.0f, QWidget* parent = nullptr);

		void Update(float value);

		QSize minimumSize() const override;
	};
}

#endif // GUARD