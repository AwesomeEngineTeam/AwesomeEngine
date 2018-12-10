#include <Awesome/Core/Logger/Logger.hpp>

#include <cstring>

#if defined(_WIN32) && !defined(__MINGW32__)
	#include <direct.h>
	#define OPENFILE(file, filePath, mode) fopen_s(&file, filePath, mode);
	#define GETCWD _getcwd
#else
    #include <unistd.h>
	#define OPENFILE(file, filePath, mode) file = fopen(filePath, mode);
	#define GETCWD getcwd
#endif

namespace aw
{
	std::string File::workingDir = File::GetCurrentWorkingDirectory();

	std::string File::OpenMethodToStr(OpenMethodBit openMethod)
	{
		switch (openMethod)
		{
		case OpenMethodBit::Read | OpenMethodBit::Binary:
			return "rb";
		case OpenMethodBit::Read:
			return "r";
		case OpenMethodBit::ReadPlus:
			return "r+";
		case OpenMethodBit::Write:
			return "w";
		case OpenMethodBit::AtEnd:
			return "a";
		case OpenMethodBit::Append:
		default:
			return "a+";
		}
	}

	inline std::string File::GetCurrentWorkingDirectory()
	{
		char buffer[MAX_PATH];
		GETCWD(buffer, MAX_PATH);

		return std::string(buffer);
	}

	void File::Open(std::string filePath, OpenMethodBit openMethod)
	{
		Close();

		OPENFILE(m_fileHandle, filePath.c_str(), OpenMethodToStr(openMethod).c_str());

		if (m_fileHandle == nullptr)
		    LOG_WARNING("Could not open file %s", filePath.c_str());

		m_name = filePath;
	}

	void File::Close()
	{
		if (m_fileHandle != nullptr)
		{
			fclose(m_fileHandle);
			m_fileHandle = nullptr;
		}
	}

	void File::Read(void * ptr, long elemSize, long count)
	{
		if (!IsOpen())
		{
			LOG_ERROR("Trying to read file %s but is not opened", m_name.c_str());
			return;
		}

		if (!Good())
		{
			LOG_ERROR("Read failed because the file stream is probably not valid anymore (check File::Good()).");
			return;
		}

		if (!IsReadable())
		{
			LOG_ERROR("Trying to read file %s but it is not opened as readable", m_name.c_str());
			return;
		}

		long currentPosition = ftell(m_fileHandle);
		long lSize = GetSize();

		long nbToRead;
		long sizeLeft = lSize - currentPosition;
		if (sizeLeft < elemSize * count)
		{
			LOG_WARNING("Specified read size is larger than filesize - cursor position, only %ld bytes will be read", sizeLeft);
			nbToRead = sizeLeft / elemSize;
		}
		else
			nbToRead = count;

		size_t result = fread(ptr, static_cast<size_t>(elemSize), static_cast<size_t>(nbToRead), m_fileHandle);
		if (result != nbToRead)
		{
			int eof = feof(m_fileHandle);
			int error = ferror(m_fileHandle);
			LOG_ERROR("Read failed. Only %u bytes have been read. %u intended. (file = %s) (eof = %d, error = %d)", result * elemSize, elemSize * nbToRead, m_name.c_str(), eof, error);
			clearerr(m_fileHandle);
		}
	}

	void File::Write(const void* data, size_t elemSize, size_t count)
	{
		if (!IsOpen())
		{
			printf("Trying to write in file %s but is not opened", m_name.c_str());
			return;
		}

		if (!Good())
		{
			printf("Write failed because the file stream is probably not valid anymore (check File::Good()).");
			return;
		}

		if (!IsWritable())
		{
			printf("Trying to write in file %s but it is not opened as writable", m_name.c_str());
			return;
		}

		size_t result = fwrite(data, elemSize, count, m_fileHandle);
		if (result != count)
			printf("Write failed. Only %llu bytes have been read. %llu intended.", result * elemSize, elemSize * count);
	}

	void File::SetCursorPosition(long pos, uint8 relativeTo)
	{
		long size = GetSize();
		if (pos > size)
		{
			LOG_ERROR("SetCursor position failed ! Position %llu was given but the file size is only %llu bytes", pos, size);
			return;
		}
		else
		{
			long curPos = ftell(m_fileHandle);
			if ((relativeTo == SEEK_CUR && curPos + pos > size))
			{
				LOG_ERROR("SetCursor position failed ! Offset %llu was given but there is only %llu bytes left", pos, size - curPos);
				return;
			}
		}

		fseek(m_fileHandle, pos, relativeTo);
	}

	long File::GetSize() const
	{
		long currentPosition = ftell(m_fileHandle);

		fseek(m_fileHandle, 0, SEEK_END);
		long lSize = ftell(m_fileHandle);

		fseek(m_fileHandle, currentPosition, SEEK_SET);

		return lSize;
	}
}