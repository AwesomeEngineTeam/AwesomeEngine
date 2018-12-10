#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_INT_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_INT_LAYOUT_GUARD

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
	class IntLayout : public QFormLayout
	{
	protected:
		QSpinBox* m_spinBox = nullptr;

		void OnValueChange(int i);

		IntLayout(int value, int min, int max, int step, QWidget* parent);

	public:
		std::function<void(int)> onValueChange;

		IntLayout(const QString& name, int value = 0, int min = std::numeric_limits<int>::lowest(), int max = std::numeric_limits<int>::max(), int step = 1, QWidget* parent = nullptr);
		IntLayout(QLineEdit* fieldName, int value = 0, int min = std::numeric_limits<int>::lowest(), int max = std::numeric_limits<int>::max(), int step = 1, QWidget* parent = nullptr);

		void Update(int value);

		QSize minimumSize() const override;
	};
}

#endif // GUARD