#pragma once

#ifndef AWESOME_EDITOR_LAYOUTS_UINT_LAYOUT_GUARD
#define AWESOME_EDITOR_LAYOUTS_UINT_LAYOUT_GUARD

#include <Awesome/Core/Types.hpp>

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
	class UIntLayout : public QFormLayout
	{
	protected:
		QDoubleSpinBox* m_spinBox = nullptr;

		void OnValueChange(double d);

		UIntLayout(aw::uint32 value, aw::uint32 min, aw::uint32 max, aw::uint32 step, QWidget* parent);

	public:
		std::function<void(aw::uint32)> onValueChange;

		UIntLayout(const QString& name, aw::uint32 value = 0u, aw::uint32 min = 0u, aw::uint32 max = std::numeric_limits<aw::uint32>::max(), aw::uint32 step = 1u, QWidget* parent = nullptr);
		UIntLayout(QLineEdit* fieldName, aw::uint32 value = 0u, aw::uint32 min = 0u, aw::uint32 max = std::numeric_limits<aw::uint32>::max(), aw::uint32 step = 1u, QWidget* parent = nullptr);

		void Update(aw::uint32 value);

		QSize minimumSize() const override;
	};
}

#endif // GUARD