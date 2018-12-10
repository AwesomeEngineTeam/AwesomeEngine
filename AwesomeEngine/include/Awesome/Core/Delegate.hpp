#pragma once

#ifndef AWESOME_CORE_DELEGATE_GUARD
#define AWESOME_CORE_DELEGATE_GUARD

#include <unordered_map>
#include <list>
#include <functional>

namespace aw
{
	template <typename... Args>
	class Event
	{
		std::list<std::function<void(Args...)>> m_funcs;

	public:
		Event() = default;
		Event(const Event&) = delete;
		Event(Event&&) = delete;
		~Event() = default;

		inline void Add(std::function<void(Args...)> func);
		inline void Remove(std::function<void(Args...)> func);
		inline void Invoke(Args... args);

		Event& operator=(Event&&) = delete;
		Event& operator=(const Event&) = delete;

		inline Event& operator+=(const std::function<void(Args...)>& rhs);
		inline Event& operator-=(const std::function<void(Args...)>& rhs);

		inline void operator()(Args... args);
	};


	template<typename ...ArgumentTypes>
	class Delegate
	{
		typedef void(*FunctionPointer)(ArgumentTypes...);

		std::list<FunctionPointer> m_callers;

	public:

		Delegate() = default;
		Delegate(const Delegate& other) = delete;
		Delegate(Delegate&& other) = delete;
		~Delegate() = default;

		inline void Add(FunctionPointer functionPointer);

		inline void RemoveFirstOf(FunctionPointer functionPointer);
		inline void RemoveLastOf(FunctionPointer functionPointer);
		inline void RemoveAllOf(FunctionPointer functionPointer);
		inline void RemoveFirst();
		inline void RemoveLast();
		inline void RemoveAll();

		inline void Invoke(ArgumentTypes... arguments);
		inline void InvokeOnce(ArgumentTypes... arguments);

		Delegate& operator+=(FunctionPointer functionPointer);
		Delegate& operator-=(FunctionPointer functionPointer);

		Delegate& operator=(const Delegate& other) = delete;
		Delegate& operator=(Delegate&& other) = delete;
	};

	template<typename ...ArgumentTypes>
	class NamedDelegate
	{
		typedef void(*FunctionPointer)(ArgumentTypes...);

		std::unordered_map<std::string, FunctionPointer> m_callers;

	public:

		inline NamedDelegate() = default;
		inline NamedDelegate(const NamedDelegate& other) = delete;
		inline NamedDelegate(NamedDelegate&& other) = delete;
		inline ~NamedDelegate() = default;

		inline void Add(std::string functionName, FunctionPointer functionPointer);

		inline void Remove(std::string functionName);
		inline void RemoveAll();

		inline void Invoke(ArgumentTypes... arguments);
		inline void InvokeOnce(ArgumentTypes... arguments);

		inline NamedDelegate& operator=(const NamedDelegate& other) = delete;
		inline NamedDelegate& operator=(NamedDelegate&& other) = delete;
	};

	template<typename MemeberType, typename ...ArgumentTypes>
	class MemberDelegate
	{
		typedef void(MemeberType::*FunctionPointer)(ArgumentTypes...);

		std::list<std::pair<MemeberType*, FunctionPointer>> m_callers;

	public:

		inline MemberDelegate() = default;
		inline MemberDelegate(const MemberDelegate& other) = delete;
		inline MemberDelegate(MemberDelegate&& other) = delete;
		inline ~MemberDelegate() = default;

		inline void Add(MemeberType* member, FunctionPointer functionPtr);

		inline void RemoveAll();

		inline void Invoke(ArgumentTypes... arguments);
		inline void InvokeOnce(ArgumentTypes... arguments);

		inline MemberDelegate& operator=(const MemberDelegate& other) = delete;
		inline MemberDelegate& operator=(MemberDelegate&& other) = delete;
	};

	template<typename MemeberType, typename ...ArgumentTypes>
	class NamedMemberDelegate
	{
		typedef void(MemeberType::*FunctionPointer)(ArgumentTypes...);

		struct MemeberDelegateData
		{
			FunctionPointer m_functionPtr;
			MemeberType* m_member;

			inline MemeberDelegateData() = default;

			inline MemeberDelegateData(MemeberType* member, FunctionPointer functionPtr)
			{
				m_functionPtr = functionPtr;
				m_member = member;
			}

			inline MemeberDelegateData(const MemeberDelegateData& other)
			{
				m_functionPtr = other.m_functionPtr;
				m_member = other.m_member;
			}

			inline MemeberDelegateData(MemeberDelegateData&& other)
			{
				m_functionPtr = std::move(other.m_functionPtr);
				m_member = std::move(other.m_member);
			}

			inline ~MemeberDelegateData() = default;

			inline void operator()(ArgumentTypes... arguments)
			{
				if (m_member)
					((m_member)->*(m_functionPtr))(arguments...);
			}

			inline MemeberDelegateData& operator=(const MemeberDelegateData& other)
			{
				m_functionPtr = other.m_functionPtr;
				m_member = other.m_member;

				return *this;
			}

			inline MemeberDelegateData& operator=(MemeberDelegateData&& other)
			{
				m_functionPtr = std::move(other.m_functionPtr);
				m_member = std::move(other.m_member);

				return *this;
			}
		};

		std::unordered_map<std::string, MemeberDelegateData> m_callers;

	public:

		inline NamedMemberDelegate() = default;
		inline NamedMemberDelegate(const NamedMemberDelegate& other) = delete;
		inline NamedMemberDelegate(NamedMemberDelegate&& other) = delete;
		inline ~NamedMemberDelegate() = default;

		inline void Add(std::string functionName, MemeberType* member, FunctionPointer functionPtr);

		inline void Remove(std::string functionName);
		inline void RemoveAll();

		inline void Invoke(ArgumentTypes... arguments);
		inline void InvokeOnce(ArgumentTypes... arguments);

		inline NamedMemberDelegate& operator=(const NamedMemberDelegate& other) = delete;
		inline NamedMemberDelegate& operator=(NamedMemberDelegate&& other) = delete;
	};
}

#include <Awesome/Core/Delegate.inl>

#endif //GUARD