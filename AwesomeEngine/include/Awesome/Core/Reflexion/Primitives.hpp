#pragma once

#ifndef AWESOME_PRIMITIVES_GUARD
#define AWESOME_PRIMITIVES_GUARD

#include <Awesome/Core/Reflexion/Reflect.hpp>

namespace aw
{
	namespace reflect
	{
		/// _____________________________________int8_________________________________________________

		struct TypeDescriptor_Int8 : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_Int8();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// _____________________________________uint8________________________________________________

		struct TypeDescriptor_UInt8 : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_UInt8();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// _____________________________________int16________________________________________________

		struct TypeDescriptor_Int16 : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_Int16();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// _____________________________________uint16_______________________________________________

		struct TypeDescriptor_UInt16 : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_UInt16();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// _____________________________________int32________________________________________________

		struct TypeDescriptor_Int32 : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_Int32();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// _____________________________________uint32_______________________________________________

		struct TypeDescriptor_UInt32 : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_UInt32();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// _____________________________________int64________________________________________________

		struct TypeDescriptor_Int64 : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_Int64();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// _____________________________________uint64_______________________________________________

		struct TypeDescriptor_UInt64 : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_UInt64();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// ______________________________________bool________________________________________________

		struct TypeDescriptor_Bool : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_Bool();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// ______________________________________float_______________________________________________

		struct TypeDescriptor_Float : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_Float();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// _____________________________________double_______________________________________________

		struct TypeDescriptor_Double : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_Double();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// ___________________________________std::string____________________________________________

		struct TypeDescriptor_StdString : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_StdString();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// ________________________________luwra::Pushable___________________________________________

		struct TypeDescriptor_LuaPushable : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_LuaPushable();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		

		/// __________________________________________________________________________________________
		/// _________________________________vk::Viewport_____________________________________________

		struct TypeDescriptor_VkViewport : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_VkViewport();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};

		/// __________________________________________________________________________________________
		/// __________________________________vk::Rect2D______________________________________________

		struct TypeDescriptor_VkRect2D : TypeDescriptor
		{
			AWESOME_CORE_API TypeDescriptor_VkRect2D();
			AWESOME_CORE_API void Dump(const void* obj, std::ostream& os = std::cout) const override;
			AWESOME_CORE_API void Undump(void* obj, std::istream& is = std::cin) const override;
		};
	}
}

#endif // !GUARD