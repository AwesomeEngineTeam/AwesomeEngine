namespace aw
{
	template<typename T>
	template<typename U>
	inline VectorView<T>::VectorView(std::vector<U>& vector)
	{
		Assign(vector);
	}

	template<typename T>
	template<typename U>
	inline VectorView<T>::VectorView(U* data, size_t size)
	{
		Assign(data, size);
	}

	template<typename T>
	template<typename U>
	inline VectorView<T>::VectorView(VectorView<U>&& other) noexcept
	{
		Assign(other.Data(), other.Size());
	}

	template<typename T>
	template<typename U>
	inline VectorView<T>::VectorView(VectorView<U>& other)
	{
		Assign(other.Data(), other.Size());
	}

	template<typename T>
	template<typename U>
	inline void VectorView<T>::Assign(std::vector<U>& vector)
	{
		Assign(vector.data(), vector.size());
	}

	template<typename T>
	template<typename U>
	inline void VectorView<T>::Assign(U* data, size_t size)
	{
		m_data = reinterpret_cast<T*>(data);
		m_size = size * sizeof(U) / sizeof(T);
	}

	template<typename T>
	inline T* VectorView<T>::Data()
	{
		return m_data;
	}

	template<typename T>
	inline const T* VectorView<T>::Data() const
	{
		return m_data;
	}

	template<typename T>
	inline size_t VectorView<T>::Size() const
	{
		return m_size;
	}

	template<typename T>
	inline T* VectorView<T>::begin()
	{
		return m_data;
	}

	template<typename T>
	inline const T* VectorView<T>::cbegin() const
	{
		return m_data;
	}

	template<typename T>
	inline T* VectorView<T>::end()
	{
		return &m_data[m_size];
	}

	template<typename T>
	inline const T* VectorView<T>::cend() const
	{
		return &m_data[m_size];
	}

	template<typename T>
	inline T& VectorView<T>::operator[](size_t index)
	{
		return m_data[index];
	}

	template<typename T>
	inline const T& VectorView<T>::operator[](size_t index) const
	{
		return m_data[index];
	}

	template<typename T>
	template<typename U>
	inline VectorView<T>& VectorView<T>::operator=(VectorView<U>& other)
	{
		Assign(other.Data(), other.Size());

		return *this;
	}

	template<typename T>
	template<typename U>
	inline VectorView<T>& VectorView<T>::operator=(VectorView<U>&& other) noexcept
	{
		Assign(other.Data(), other.Size());

		return *this;
	}
}