namespace aw
{
	LuaPushable::LuaPushable() :
		pushable{ int(0) },
		pushableTypeDescriptor{ reflect::TypeResolver<int>::Get() }
	{
		getValue = [](const void* luaPushablePtr) -> const void*
		{
			const auto& luaPushable = *(const aw::LuaPushable*)(luaPushablePtr);
			return &(dynamic_cast<luwra::internal::PushableT<int>*>(luaPushable.pushable.interface.get())->value);
		};
	}

	template <typename Type>
	LuaPushable::LuaPushable(Type&& value) :
		pushable{ std::forward<Type>(value) },
		pushableTypeDescriptor{ reflect::TypeResolver<Type>::Get() }
	{
		getValue = [](const void* luaPushablePtr) -> const void*
		{
			const auto& luaPushable = *(const aw::LuaPushable*)(luaPushablePtr);
			return &(dynamic_cast<luwra::internal::PushableT<Type>*>(luaPushable.pushable.interface.get())->value);
		};
	}

	LuaPushable::LuaPushable(LuaPushable&& other) :
		pushable{ std::move(other.pushable) }
	{
		pushableTypeDescriptor = other.pushableTypeDescriptor;	other.pushableTypeDescriptor = nullptr;
		getValue = other.getValue; other.getValue = nullptr;
	}

	LuaPushable& LuaPushable::operator=(LuaPushable&& rhs)
	{
		pushable = std::move(rhs.pushable);
		pushableTypeDescriptor = rhs.pushableTypeDescriptor; rhs.pushableTypeDescriptor = nullptr;
		getValue = rhs.getValue; rhs.getValue = nullptr;

		return *this;
	}

	bool LuaPushable::operator<(const LuaPushable& rhs) const
	{
		return pushable.interface < rhs.pushable.interface;
	}
}