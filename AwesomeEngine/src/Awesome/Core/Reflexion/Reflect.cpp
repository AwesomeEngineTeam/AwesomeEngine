#include <Awesome/Core/Reflexion/Reflect.hpp>

namespace aw
{
	std::stringstream CutString(std::string& s, size_t pos, size_t count)
	{
		std::stringstream ss;
		ss << s.substr(pos, count);
		s = s.substr(count, std::string::npos);

		return ss;
	}

	namespace reflect
	{
		void TypeDescriptor_Struct::Dump(const void* obj, std::ostream& os) const
		{
			size_t memberNb = members.size();

			if (memberNb == 0)
				return;

			size_t i = 0;

			for (; i < memberNb - 1; ++i)
			{
				members[i].type->Dump(reinterpret_cast<const void*>(static_cast<const char*>(obj) + members[i].offset), os);
				os << '-';
			}

			members[i].type->Dump(reinterpret_cast<const void*>(static_cast<const char*>(obj) + members[i].offset), os);
		}

		void TypeDescriptor_Struct::Undump(void* obj, std::istream& is) const
		{
			// TODO : Add Parsing

			size_t memberNb = members.size();

			if (memberNb == 0)
				return;

			size_t i = 0;

			for (; i < memberNb - 1; ++i)
				members[i].type->Undump(reinterpret_cast<void*>(static_cast<char*>(obj) + members[i].offset), is);

			members[i].type->Undump(reinterpret_cast<void*>(static_cast<char*>(obj) + members[i].offset), is);
		}
	}
}
		