namespace aw
{
	namespace reflect
	{
		inline TypeDescriptor::TypeDescriptor(const char* name, size_t size) :
			name{ name },
			size{ size }
		{
		}

		inline ::std::string TypeDescriptor::GetFullName() const
		{
			return name;
		}

		inline TypeDescriptor_Struct::TypeDescriptor_Struct(void(*init)(TypeDescriptor_Struct*)) : TypeDescriptor{ nullptr, 0 }
		{
			init(this);
		}

		inline TypeDescriptor_Struct::TypeDescriptor_Struct(const char* name, size_t size, const std::initializer_list<Member>& init) : TypeDescriptor{ nullptr, 0 }, members{ init }
		{
		}
	}
}