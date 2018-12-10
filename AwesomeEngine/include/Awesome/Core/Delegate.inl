namespace aw
{
	#pragma region Event

	template <typename... Args>
	inline void Event<Args...>::Add(std::function<void(Args...)> func)
	{
		m_funcs.push_back(std::move(func));
	}

	template <typename... Args>
	inline void Event<Args...>::Remove(std::function<void(Args...)> func)
	{
		using fnType = void(*)(Args...);

		for (auto it = m_funcs.begin(); it != m_funcs.end(); it++)
		{
			if ((*it).template target<fnType*>() == func.template target<fnType*>())
			{
				m_funcs.erase(it);
				return;
			}
		}
	}

	template <typename... Args>
	inline void Event<Args...>::Invoke(Args... args)
	{
		for (auto it = m_funcs.begin(); it != m_funcs.end(); it++)
			(*it)(args...);
	}

	template <typename... Args>
	inline Event<Args...>& Event<Args...>::operator+=(const std::function<void(Args...)>& rhs)
	{
		Add(rhs);
		return *this;
	}

	template <typename... Args>
	inline Event<Args...>& Event<Args...>::operator-=(const std::function<void(Args...)>& rhs)
	{
		Remove(rhs);
		return *this;
	}

	template <typename... Args>
	inline void Event<Args...>::operator()(Args... args)
	{
		Invoke(std::forward<Args...>(args)...);
	}

	#pragma endregion

	#pragma region Delegate
	template<typename ...ArgumentTypes>
	inline void Delegate<ArgumentTypes...>::Add(FunctionPointer functionPointer)
	{
		m_callers.push_back(functionPointer);
	}

	template<typename ...ArgumentTypes>
	inline void  Delegate<ArgumentTypes...>::RemoveFirstOf(FunctionPointer functionPointer)
	{
		for (auto it = m_callers.begin(); it != m_callers.end(); ++it)
		{
			if ((*it) == functionPointer)
			{
				m_callers.erase(it);
				return;
			}
		}
	}

	template<typename ...ArgumentTypes>
	inline void  Delegate<ArgumentTypes...>::RemoveLastOf(FunctionPointer functionPointer)
	{
		for (auto it = m_callers.rbegin(); it != m_callers.rend(); ++it)
		{
			if ((*it) == functionPointer)
			{
				m_callers.erase(std::next(it).base());
				return;
			}
		}
	}

	template<typename ...ArgumentTypes>
	inline void  Delegate<ArgumentTypes...>::RemoveAllOf(FunctionPointer functionPointer)
	{
		m_callers.remove(functionPointer);
	}

	template<typename ...ArgumentTypes>
	inline void  Delegate<ArgumentTypes...>::RemoveFirst()
	{
		m_callers.pop_front();
	}

	template<typename ...ArgumentTypes>
	inline void  Delegate<ArgumentTypes...>::RemoveLast()
	{
		m_callers.pop_back();
	}

	template<typename ...ArgumentTypes>
	inline void  Delegate<ArgumentTypes...>::RemoveAll()
	{
		m_callers.clear();
	}

	template<typename ...ArgumentTypes>
	inline void  Delegate<ArgumentTypes...>::Invoke(ArgumentTypes... arguments)
	{
		for (auto it = m_callers.begin(); it != m_callers.end(); ++it)
		{
			(*it)(arguments...);
		}
	}

	template<typename ...ArgumentTypes>
	inline void  Delegate<ArgumentTypes...>::InvokeOnce(ArgumentTypes... arguments)
	{
		for (auto it = m_callers.begin(); it != m_callers.end(); it = m_callers.begin())
		{
			(*it)(arguments...);
			m_callers.erase(it);
		}
	}

	template<typename ...ArgumentTypes>
	Delegate<ArgumentTypes...>&  Delegate<ArgumentTypes...>::operator+=(FunctionPointer functionPointer)
	{
		Add(functionPointer);
		return *this;
	}

	template<typename ...ArgumentTypes>
	Delegate<ArgumentTypes...>&  Delegate<ArgumentTypes...>::operator-=(FunctionPointer functionPointer)
	{
		RemoveFirstOf(functionPointer);
		return *this;
	}
	#pragma endregion

	#pragma region NamedDelegate
	template<typename ...ArgumentTypes>
	inline void NamedDelegate<ArgumentTypes...>::Add(std::string functionName, FunctionPointer functionPointer)
	{
		m_callers[functionName] = functionPointer;
	}

	template<typename ...ArgumentTypes>
	inline void NamedDelegate<ArgumentTypes...>::Remove(std::string functionName)
	{
		m_callers.erase(functionName);
	}

	template<typename ...ArgumentTypes>
	inline void NamedDelegate<ArgumentTypes...>::RemoveAll()
	{
		m_callers.clear();
	}

	template<typename ...ArgumentTypes>
	inline void NamedDelegate<ArgumentTypes...>::Invoke(ArgumentTypes ...arguments)
	{
		for (auto it = m_callers.begin(); it != m_callers.end(); ++it)
		{
			it->second(arguments...);
		}
	}

	template<typename ...ArgumentTypes>
	inline void NamedDelegate<ArgumentTypes...>::InvokeOnce(ArgumentTypes ...arguments)
	{
		for (auto it = m_callers.begin(); it != m_callers.end(); it = m_callers.begin())
		{
			it->second(arguments...);
			m_callers.erase(it);
		}
	}
	#pragma endregion

	#pragma region MemberDelegate
	template<typename MemeberType, typename ...ArgumentTypes>
	inline void MemberDelegate<MemeberType, ArgumentTypes...>::Add(MemeberType* member, FunctionPointer functionPtr)
	{
		m_callers.push_back(std::pair<MemeberType*, FunctionPointer>(member, functionPtr));
	}

	template<typename MemeberType, typename ...ArgumentTypes>
	inline void MemberDelegate<MemeberType, ArgumentTypes...>::RemoveAll()
	{
		m_callers.clear();
	}

	template<typename MemeberType, typename ...ArgumentTypes>
	inline void MemberDelegate<MemeberType, ArgumentTypes...>::Invoke(ArgumentTypes... arguments)
	{
		for (auto it = m_callers.begin(); it != m_callers.end(); ++it)
		{
			if (it->first != nullptr)
				((it->first)->*(it->second))(arguments...);
		}
	}

	template<typename MemeberType, typename ...ArgumentTypes>
	inline void MemberDelegate<MemeberType, ArgumentTypes...>::InvokeOnce(ArgumentTypes... arguments)
	{
		for (auto it = m_callers.begin(); it != m_callers.end(); it = m_callers.begin())
		{
			((it->first)->*(it->second))(arguments...);
			m_callers.erase(it);
		}
	}
	#pragma endregion

	#pragma region NamedMemberDelegate
	template<typename MemeberType, typename ...ArgumentTypes>
	inline void NamedMemberDelegate<MemeberType, ArgumentTypes...>::Add(std::string functionName, MemeberType* member, FunctionPointer functionPtr)
	{
		m_callers[functionName] = MemeberDelegateData(member, functionPtr);
	}

	template<typename MemeberType, typename ...ArgumentTypes>
	inline void NamedMemberDelegate<MemeberType, ArgumentTypes...>::Remove(std::string functionName)
	{
		m_callers.erase(functionName);
	}

	template<typename MemeberType, typename ...ArgumentTypes>
	inline void NamedMemberDelegate<MemeberType, ArgumentTypes...>::RemoveAll()
	{
		m_callers.clear();
	}

	template<typename MemeberType, typename ...ArgumentTypes>
	inline void NamedMemberDelegate<MemeberType, ArgumentTypes...>::Invoke(ArgumentTypes... arguments)
	{
		for (auto it = m_callers.begin(); it != m_callers.end(); ++it)
		{
			it->second(arguments...);
		}
	}

	template<typename MemeberType, typename ...ArgumentTypes>
	inline void NamedMemberDelegate<MemeberType, ArgumentTypes...>::InvokeOnce(ArgumentTypes... arguments)
	{
		for (auto it = m_callers.begin(); it != m_callers.end(); it = m_callers.begin())
		{
			it->second(arguments...);
			m_callers.erase(it);
		}
	}
	#pragma endregion
}