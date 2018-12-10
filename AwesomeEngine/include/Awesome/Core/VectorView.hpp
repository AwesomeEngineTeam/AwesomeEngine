#pragma once

#ifndef AWESOME_CORE_VECTOR_VIEW_GUARD
#define AWESOME_CORE_VECTOR_VIEW_GUARD

#include <vector>

namespace aw
{
	template<typename T>
	class VectorView
	{
		T* m_data{ nullptr };
		size_t m_size{ 0 };

	public:
		VectorView() = default;
		template<typename U>
		inline explicit VectorView(std::vector<U>& vector);
		template<typename U>
		inline VectorView(U* data, size_t size);
		template<typename U>
		inline explicit VectorView(VectorView<U>& other);
		template<typename U>
		inline explicit VectorView(VectorView<U>&& other) noexcept;
		~VectorView() = default;

		template<typename U>
		inline void Assign(std::vector<U>& vector);
		template<typename U>
		inline void Assign(U* data, size_t size);

		inline T* Data();
		inline const T* Data() const;
		inline size_t Size() const;

		inline T* begin();
		inline const T* cbegin() const;
		inline T* end();
		inline const T* cend() const;

		inline T& operator[](size_t index);
		inline const T& operator[](size_t index) const;

		template<typename U>
		inline VectorView& operator=(VectorView<U>& other);
		template<typename U>
		inline VectorView& operator=(VectorView<U>&& other) noexcept;
	};
}

#include <Awesome/Core/VectorView.inl>

#endif //GUARD