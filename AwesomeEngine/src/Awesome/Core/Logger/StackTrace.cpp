#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Logger/Logger.hpp>

#include <string>
#include <iostream>

namespace aw
{
	std::string StackTrace::GetFunctionName(HANDLE process, DWORD64 address)
	{
		char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
		PIMAGEHLP_SYMBOL64 symbol = (PIMAGEHLP_SYMBOL64)symbolBuffer;

		symbol->SizeOfStruct = (sizeof(PIMAGEHLP_SYMBOL64)) + 255;
		symbol->MaxNameLength = 254;

		if (SymGetSymFromAddr64(process, address, NULL, symbol))
			return symbol->Name;
		else
		{
			DWORD err = GetLastError();
			LOG_WARNING(std::string("[StackTrace] ") + __func__ + ": Failed to resolve address 0x%X: %u", address, err);

			return "undefined";
		}
	}

	std::string StackTrace::GetModuleName(HANDLE process, DWORD64 address)
	{
		#if _WIN64
		DWORD64 moduleBase = 0;
		#else
		DWORD moduleBase = 0;
		#endif

		moduleBase = SymGetModuleBase(process, address);
		char moduleBuff[MAX_PATH];

		if (moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduleBuff, MAX_PATH))
			return strrchr(moduleBuff, '\\') + 1;
		else
			return "undefined";
	}

	const IMAGEHLP_LINE StackTrace::GetLine(HANDLE process, DWORD64 address)
	{
		IMAGEHLP_LINE line;
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		DWORD offset = 0;
		if (SymGetLineFromAddr(process, address, &offset, &line) == FALSE)
		{
			line.FileName = const_cast<char*>("undefined");
			line.LineNumber = 0;

			DWORD err = GetLastError();
			LOG_WARNING(std::string("[StackTrace] ") + __func__ + ": Failed to resolve address 0x%X: %u", address, err);

			return line;
		}

		std::string relativeFileName = strrchr(line.FileName, '\\') + 1;
		memcpy(line.FileName, relativeFileName.c_str(), relativeFileName.size() + 1);

		return line;
	}

	std::deque<StackTrace::StackFrameInfo> StackTrace::GetStackTrace()
	{
		std::deque<StackFrameInfo> stackFrames;

		static StackContext stackContext;
		stackContext.process = GetCurrentProcess();
		stackContext.thread = GetCurrentThread();

		RtlCaptureContext(&stackContext.context);

		stackContext.Refresh();

		if (stackContext.isValid == FALSE)
			return stackFrames;

		uint8 count = 0;
		while (StackWalk(stackContext.machine, stackContext.process, stackContext.thread, &stackContext.rootFrame, &stackContext.context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
		{
			StackFrameInfo stackFrame = {};
			stackFrame.functionAddress = stackContext.rootFrame.AddrPC.Offset;

			stackFrame.moduleName = GetModuleName(stackContext.process, stackFrame.functionAddress);
			stackFrame.functionName = GetFunctionName(stackContext.process, stackFrame.functionAddress);
			
			IMAGEHLP_LINE line = GetLine(stackContext.process, stackFrame.functionAddress);

			stackFrame.file = line.FileName;
			stackFrame.line = line.LineNumber;

			if (count++)
				stackFrames.push_back(stackFrame);

			if (stackFrame.functionName == "main" || count == MAX_TRACE_DEPTH)
				break;
		}

		stackContext.Cleanup();

		return stackFrames;
	}
}