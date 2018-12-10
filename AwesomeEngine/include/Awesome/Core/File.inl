namespace aw
{
	inline File::File(std::string filePath, OpenMethodBit openMethod)
	{
		Open(filePath, openMethod);
	}

	inline File::~File()
	{
		Close();
	}

	template<typename... Args>
	inline void File::Write(const char* str, Args&&... args)
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

		if (fprintf(m_fileHandle, str, args...) < 0)
			printf("Could not write to file %s.", m_name.c_str());

		//ONLY ACTIVATE THIS FOR DEBUG PURPOSE
//		fflush(m_fileHandle);
	}

	inline void File::Write(const char* str)
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

		if (fprintf(m_fileHandle, "%s", str) < 0)
			printf("Could not write to file %s.", m_name.c_str());

		//ONLY ACTIVATE THIS FOR DEBUG PURPOSE
		//fflush(file);
	}

	inline long File::GetCursorPosition(uint8 relativeTo)
	{
		long pos = ftell(m_fileHandle);

		if (relativeTo == SEEK_END)
			return GetSize() - pos;

		return pos;
	}

	inline std::string File::GetWorkingDirectory()
	{
		return workingDir;
	}

	inline std::string File::GetRelativePath(const char* path)
	{
		if (workingDir.compare(path) == 0)
			return std::string(path);

		std::string result = path;
		result.erase(0, workingDir.size() + 1);

		return result;
	}

	inline std::string File::GetRelativePath(std::string path)
	{
		return GetRelativePath(path.c_str());
	}

	inline std::string_view File::GetName()
	{
		return std::string_view(m_name);
	}

	inline bool File::IsReadable()
	{
		return true;
		//return strchr(openmode, '+') || strchr(openmode, 'r');
	}

	inline bool File::IsWritable()
	{
		return true;
		//return strchr(openmode, '+') || strchr(openmode, 'w') || strchr(openmode, 'a');
	}

	inline bool File::Good()
	{
		return !ferror(m_fileHandle);
	}

	inline bool File::IsOpen()
	{
		return m_fileHandle != nullptr;
	}
}