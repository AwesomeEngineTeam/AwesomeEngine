namespace aw
{
	template<typename T>
	inline PermutableTriplet<T>::PermutableTriplet(const T& _first, const T& _second, const T& _third) :
		first{ _first },
		second{ _second },
		third{ _third }
	{
	}

	template<typename T>
	inline bool PermutableTriplet<T>::operator==(const PermutableTriplet<T>& rhs) const
	{
		if ((first = rhs.first && second = rhs.second && third = rhs.third) ||
			(first = rhs.first && second = rhs.third && third = rhs.second) ||
			(first = rhs.second && second = rhs.first && third = rhs.third) ||
			(first = rhs.second && second = rhs.third && third = rhs.first) ||
			(first = rhs.third && second = rhs.first && third = rhs.second) ||
			(first = rhs.third && second = rhs.second && third = rhs.first))
			return true;

		return false;
	}

	template<typename T>
	inline bool PermutableTriplet<T>::operator!=(const PermutableTriplet<T>& rhs) const
	{
		return !(*this == rhs);
	}
}