namespace aw
{
	template<typename T>
	inline PermutableDoublet<T>::PermutableDoublet(const T& _first, const T& _second) :
		first{_first},
		second{_second}
	{
	}

	template<typename T>
	inline bool PermutableDoublet<T>::operator==(const PermutableDoublet<T>& rhs) const
	{
		if ((first == rhs.first && second == rhs.second) ||
			(first == rhs.second && second == rhs.first))
			return true;

		return false;
	}

	template<typename T>
	inline bool PermutableDoublet<T>::operator!=(const PermutableDoublet<T>& rhs) const
	{
		return !(*this == rhs);
	}
}