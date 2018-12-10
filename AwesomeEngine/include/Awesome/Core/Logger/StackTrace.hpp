#pragma once

#ifndef AWESOME_CORE_STACKTRACE_GUARD
#define AWESOME_CORE_STACKTRACE_GUARD

#include <Awesome/Core/Config.hpp>

#include <Windows.h>
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp.lib")
#pragma warning( disable : 4251 )

#include <deque>
#include <iostream>

#define MAX_TRACE_DEPTH 20

namespace aw
{
	class AWESOME_CORE_API StackTrace
	{
	public:
		struct AWESOME_CORE_API StackFrameInfo
		{
			DWORD64 functionAddress;

			std::string moduleName;
			std::string functionName;

			std::string file;
			unsigned int line;
		};

	private:
		struct AWESOME_CORE_API StackContext
		{
			DWORD machine;

			HANDLE process;
			HANDLE thread;

			CONTEXT context = {};
			STACKFRAME rootFrame = {};

			bool isValid;

			inline StackContext();

			inline void Refresh();
			inline void Cleanup();
		};

		static std::string GetFunctionName(HANDLE process, DWORD64 address);
		static std::string GetModuleName(HANDLE process, DWORD64 address);
		static const IMAGEHLP_LINE GetLine(HANDLE process, DWORD64 address);

	public:
		StackTrace() = delete;
		~StackTrace() = delete;

		static std::deque<StackFrameInfo> GetStackTrace();
	};
}

#include <Awesome/Core/Logger/StackTrace.inl>

#endif //GUARD