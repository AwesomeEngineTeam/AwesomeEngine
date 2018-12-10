#pragma once

#ifndef AWESOME_CORE_FILE_GUARD
#define AWESOME_CORE_FILE_GUARD

#include <Awesome/Core/NonCopyable.hpp>
#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Config.hpp>

#include <cstdio>
#include <string>
#include <fstream>
#include <string_view>

namespace aw
{
	enum class OpenMethodBit : uint8
	{
		None = 0,
		Read = 0x1,
		Write = 0x2,
		Binary = 0x4,
		AtEnd = 0x8,
		Append = 0x10,
		Truncate = 0x20,
		ReadPlus = 0x40
	};

	constexpr inline OpenMethodBit operator|(OpenMethodBit lhs, OpenMethodBit rhs)
	{
		return static_cast<OpenMethodBit>(
			static_cast<uint8>(lhs) |
			static_cast<uint8>(rhs)
			);
	}
	constexpr inline OpenMethodBit operator&(OpenMethodBit lhs, OpenMethodBit rhs)
	{
		return static_cast<OpenMethodBit>(
			static_cast<uint8>(lhs) &
			static_cast<uint8>(rhs)
			);
	}

	//enum class WatchNotifyBit : uint32
	//{
	//	None = 0,
	//	FileName = 0x00000001,
	//	DirName = 0x00000002,
	//	Attributes = 0x00000004,
	//	Size = 0x00000008,
	//	LastWrite = 0x00000010,
	//	LastAccess = 0x00000020,
	//	Creation = 0x00000040,
	//	Security = 0x00000100,
	//};

	//static WatchNotifyBit operator|(WatchNotifyBit lhs, WatchNotifyBit rhs);
	//static WatchNotifyBit operator&(WatchNotifyBit lhs, WatchNotifyBit rhs);

	class File : NonCopyable
	{
		FILE* m_fileHandle = nullptr;

		std::string m_name;
		//const char* m_openmode{ nullptr };

		static std::string workingDir;
		static std::string GetCurrentWorkingDirectory();

		std::string OpenMethodToStr(OpenMethodBit openMethod);

	public:
		File() = default;
		inline File(std::string filePath, OpenMethodBit openMethod);
		inline ~File() override;

		AWESOME_CORE_API void Open(std::string filePath, OpenMethodBit openMethod);
		AWESOME_CORE_API void Close();

		/**
		* Write in file a printf-like string parsed.
		*
		* @param A string to write
		* @param B va_args to put in string
		*/
		template<typename... Args>
		inline void Write(const char* str, Args&&... args);

		/**
		* Write in file a printf-like string parsed (no arg version, here for format string security).
		*
		* @param A string to write
		*/
		inline void Write(const char* str);

		/**
		* Write in file any data type.
		*
		* @param A data to write
		* @param B size of one element
		* @param C number of elements
		*/
		AWESOME_CORE_API void Write(const void* data, size_t elemSize, size_t count);

		AWESOME_CORE_API void Read(void * ptr, long elemSize, long count);

		/**
		* Set cursor for read/write position.
		*
		* @param A cursor position
		* @param B SEEK_END / SEEK_SET / SEEK_CUR respectively for position relative to end / begin / current (default = SEEK_SET)
		*/
		AWESOME_CORE_API void SetCursorPosition(long pos, uint8 relativeTo = SEEK_SET);

		/**
		* Get cursor for read/write position.
		*
		* @param A SEEK_END / SEEK_SET respectively for position relative to end / begin (default = SEEK_SET)
		*/
		inline long GetCursorPosition(uint8 relativeTo = SEEK_SET);

		inline static std::string GetWorkingDirectory();
		inline static std::string GetRelativePath(const char* path);
		inline static std::string GetRelativePath(std::string path);

		inline std::string_view GetName();

		inline bool IsReadable();
		inline bool IsWritable();

		inline bool Good();
		inline bool IsOpen();

		AWESOME_CORE_API long GetSize() const;
	};
}

#include <Awesome/Core/File.inl>

#endif //GUARD