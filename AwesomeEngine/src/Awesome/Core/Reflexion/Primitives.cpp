#include <Awesome/Core/Reflexion/Primitives.hpp>

#include <atomic>

#include <Awesome/Core/Hex.hpp>

#include <Awesome/Core/Types.hpp>

#include <Awesome/Lua/Pushable.hpp>

#include <Awesome/Renderer/Framegraph/RenderArea.hpp>

REFLECT_ENUM(aw::PushableTypeID)

namespace aw
{
	namespace reflect
	{
		/// _____________________________________int8_________________________________________________

		TypeDescriptor_Int8::TypeDescriptor_Int8() : TypeDescriptor{ "int8", sizeof(int8) }
		{
		}

		void TypeDescriptor_Int8::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_Int8::Undump(void* obj, std::istream& is) const 
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<int8>()
		{
			static TypeDescriptor_Int8 typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// _____________________________________uint8________________________________________________

		TypeDescriptor_UInt8::TypeDescriptor_UInt8() : TypeDescriptor{ "unsigned int8", sizeof(uint8) }
		{
		}

		void TypeDescriptor_UInt8::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_UInt8::Undump(void* obj, std::istream& is) const 
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<uint8>()
		{
			static TypeDescriptor_UInt8 typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// _____________________________________int16________________________________________________

		TypeDescriptor_Int16::TypeDescriptor_Int16() : TypeDescriptor{ "int16", sizeof(int16) }
		{
		}

		void TypeDescriptor_Int16::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_Int16::Undump(void* obj, std::istream& is) const
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<int16>()
		{
			static TypeDescriptor_Int16 typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// _____________________________________uint16_______________________________________________

		TypeDescriptor_UInt16::TypeDescriptor_UInt16() : TypeDescriptor{ "unsigned int16", sizeof(uint16) }
		{
		}

		void TypeDescriptor_UInt16::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_UInt16::Undump(void* obj, std::istream& is) const
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<uint16>()
		{
			static TypeDescriptor_UInt16 typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// _____________________________________int32________________________________________________

		TypeDescriptor_Int32::TypeDescriptor_Int32() : TypeDescriptor{ "int32", sizeof(int32) }
		{
		}

		void TypeDescriptor_Int32::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_Int32::Undump(void* obj, std::istream& is) const
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<int32>()
		{
			static TypeDescriptor_Int32 typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// _____________________________________uint32_______________________________________________

		TypeDescriptor_UInt32::TypeDescriptor_UInt32() : TypeDescriptor{ "unsigned int32", sizeof(uint32) }
		{
		}

		void TypeDescriptor_UInt32::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_UInt32::Undump(void* obj, std::istream& is) const
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<uint32>()
		{
			static TypeDescriptor_UInt32 typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// _____________________________________int64________________________________________________

		TypeDescriptor_Int64::TypeDescriptor_Int64() : TypeDescriptor{ "int64", sizeof(int64) }
		{
		}

		void TypeDescriptor_Int64::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_Int64::Undump(void* obj, std::istream& is) const
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<int64>()
		{
			static TypeDescriptor_Int64 typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// _____________________________________uint64_______________________________________________

		TypeDescriptor_UInt64::TypeDescriptor_UInt64() : TypeDescriptor{ "unsigned int64", sizeof(uint64) }
		{
		}

		void TypeDescriptor_UInt64::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_UInt64::Undump(void* obj, std::istream& is) const
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<uint64>()
		{
			static TypeDescriptor_UInt64 typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// ______________________________________bool________________________________________________

		TypeDescriptor_Bool::TypeDescriptor_Bool() : TypeDescriptor{ "bool", sizeof(bool) }
		{
		}

		void TypeDescriptor_Bool::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_Bool::Undump(void* obj, std::istream& is) const
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<bool>()
		{
			static TypeDescriptor_Bool typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// ______________________________________float_______________________________________________

		TypeDescriptor_Float::TypeDescriptor_Float() : TypeDescriptor{ "float", sizeof(float) }
		{
		}

			void TypeDescriptor_Float::Dump(const void* obj, std::ostream& os) const
		{
				os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_Float::Undump(void* obj, std::istream& is) const
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<float>()
		{
			static TypeDescriptor_Float typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// _____________________________________double_______________________________________________

		TypeDescriptor_Double::TypeDescriptor_Double() : TypeDescriptor{ "double", sizeof(double) }
		{
		}

		void TypeDescriptor_Double::Dump(const void* obj, std::ostream& os) const
		{
			os << HexConverter::ToHexString(HexConverter::ToByte(obj, size));
		}

		void TypeDescriptor_Double::Undump(void* obj, std::istream& is) const
		{
			std::string s;
			is >> s;

			HexConverter::FromHexString(s, obj);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<double>()
		{
			static TypeDescriptor_Double typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// _____________________________________string_______________________________________________

		TypeDescriptor_StdString::TypeDescriptor_StdString() : TypeDescriptor{ "std::string", sizeof(std::string) }
		{
		}

		void TypeDescriptor_StdString::Dump(const void* obj, std::ostream& os) const
		{
			os << *static_cast<const std::string*>(obj);
		}

		void TypeDescriptor_StdString::Undump(void* obj, std::istream& is) const
		{
			std::string& s = *static_cast<std::string*>(obj);
			is >> s;
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<std::string>()
		{
			static TypeDescriptor_StdString typeDesc;
			return &typeDesc;
		}
		/// __________________________________________________________________________________________
		/// ________________________________luwra::Pushable___________________________________________

		TypeDescriptor_LuaPushable::TypeDescriptor_LuaPushable() : TypeDescriptor{ "LuaPushable", sizeof(aw::LuaPushable) }
		{
		}

		void TypeDescriptor_LuaPushable::Dump(const void* obj, std::ostream& os) const
		{
			const aw::LuaPushable* pshPtr = reinterpret_cast<const aw::LuaPushable*>(obj);
			pshPtr->pushableTypeDescriptor->Dump(pshPtr->getValue(obj), os);
		}

		void TypeDescriptor_LuaPushable::Undump(void* obj, std::istream& is) const 
		{
			aw::LuaPushable* pshPtr = reinterpret_cast<aw::LuaPushable*>(obj);
			pshPtr->pushableTypeDescriptor->Undump(const_cast<void*>(pshPtr->getValue(obj)), is);
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<aw::LuaPushable>()
		{
			static TypeDescriptor_LuaPushable typeDesc;
			return &typeDesc;
		}

		template<>
		inline aw::LuaPushable TypeResolver<aw::LuaPushable>::FromDump(std::istream& is)
		{
			std::string s;
			is >> s;

			std::stringstream ss;

			ss = CutString(s, 0, sizeof(size_t) * 2);
			size_t typeDescriptorID = TypeResolver<size_t>::FromDump(ss);

			ss.clear();
			ss << s;

			//TypeDescriptor* td = TypeManager::Get(typeDescriptorID);
			//Type value = TypeResolver<Type>::FromDump(ss);

			//return aw::LuaPushable(value);
		}

		/// __________________________________________________________________________________________
		/// _________________________________vk::Viewport_____________________________________________

		TypeDescriptor_VkViewport::TypeDescriptor_VkViewport() : TypeDescriptor{ "vk::Viewport", sizeof(vk::Viewport) }
		{
		}

		void TypeDescriptor_VkViewport::Dump(const void* obj, std::ostream& os) const
		{
			const vk::Viewport& viewport = *static_cast<const vk::Viewport*>(obj);

			os << HexConverter::ToHexString(HexConverter::ToByte(&viewport.x, sizeof(float)));
			os << HexConverter::ToHexString(HexConverter::ToByte(&viewport.y, sizeof(float)));
			os << HexConverter::ToHexString(HexConverter::ToByte(&viewport.width, sizeof(float)));
			os << HexConverter::ToHexString(HexConverter::ToByte(&viewport.height, sizeof(float)));
			os << HexConverter::ToHexString(HexConverter::ToByte(&viewport.minDepth, sizeof(float)));
			os << HexConverter::ToHexString(HexConverter::ToByte(&viewport.maxDepth, sizeof(float)));
		}

		void TypeDescriptor_VkViewport::Undump(void* obj, std::istream& is) const
		{
			vk::Viewport& viewport = *static_cast<vk::Viewport*>(obj);

			std::string s;
			is >> s;
			
			size_t pos = 0;

			HexConverter::FromHexString(s.substr(pos, sizeof(float) * 2), &viewport.x); pos += sizeof(float) * 2;
			HexConverter::FromHexString(s.substr(pos, sizeof(float) * 2), &viewport.y); pos += sizeof(float) * 2;
			HexConverter::FromHexString(s.substr(pos, sizeof(float) * 2), &viewport.width); pos += sizeof(float) * 2;
			HexConverter::FromHexString(s.substr(pos, sizeof(float) * 2), &viewport.height); pos += sizeof(float) * 2;
			HexConverter::FromHexString(s.substr(pos, sizeof(float) * 2), &viewport.minDepth); pos += sizeof(float) * 2;
			HexConverter::FromHexString(s.substr(pos, sizeof(float) * 2), &viewport.maxDepth); pos += sizeof(float) * 2;
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<vk::Viewport>()
		{
			static TypeDescriptor_VkViewport typeDesc;
			return &typeDesc;
		}

		/// __________________________________________________________________________________________
		/// __________________________________vk::Rect2D______________________________________________

		TypeDescriptor_VkRect2D::TypeDescriptor_VkRect2D() : TypeDescriptor{ "vk::Rect2D", sizeof(vk::Rect2D) }
		{
		}

		void TypeDescriptor_VkRect2D::Dump(const void* obj, std::ostream& os) const 
		{
			const vk::Rect2D& rect = *static_cast<const vk::Rect2D*>(obj);

			os << HexConverter::ToHexString(HexConverter::ToByte(&rect.offset.x, sizeof(int32_t)));
			os << HexConverter::ToHexString(HexConverter::ToByte(&rect.offset.y, sizeof(int32_t)));
			os << HexConverter::ToHexString(HexConverter::ToByte(&rect.extent.width, sizeof(int32_t)));
			os << HexConverter::ToHexString(HexConverter::ToByte(&rect.extent.height, sizeof(int32_t)));
		}

		void TypeDescriptor_VkRect2D::Undump(void* obj, std::istream& is) const 
		{
			vk::Rect2D& rect = *static_cast<vk::Rect2D*>(obj);

			std::string s;
			is >> s;

			size_t pos = 0;

			HexConverter::FromHexString(s.substr(pos, sizeof(int32_t) * 2), &rect.offset.x); pos += sizeof(int32_t) * 2;
			HexConverter::FromHexString(s.substr(pos, sizeof(int32_t) * 2), &rect.offset.y); pos += sizeof(int32_t) * 2;
			HexConverter::FromHexString(s.substr(pos, sizeof(int32_t) * 2), &rect.extent.width); pos += sizeof(int32_t) * 2;
			HexConverter::FromHexString(s.substr(pos, sizeof(int32_t) * 2), &rect.extent.height); pos += sizeof(int32_t) * 2;
		}

		template <>
		AWESOME_CORE_API TypeDescriptor* GetPrimitiveDescriptor<vk::Rect2D>()
		{
			static TypeDescriptor_VkRect2D typeDesc;
			return &typeDesc;
		}
	}
}