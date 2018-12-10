namespace aw
{
	inline StackTrace::StackContext::StackContext()
	{
		#if _WIN64
		machine = IMAGE_FILE_MACHINE_AMD64;
		#else
		machine = IMAGE_FILE_MACHINE_I386;
		#endif

		rootFrame.AddrPC.Mode = AddrModeFlat;
		rootFrame.AddrFrame.Mode = AddrModeFlat;
		rootFrame.AddrStack.Mode = AddrModeFlat;

		context.ContextFlags = CONTEXT_FULL;
	}

	inline void StackTrace::StackContext::Refresh()
	{
		if ((isValid = SymInitialize(process, NULL, TRUE)) == FALSE)
			return;

		SymSetOptions(SYMOPT_LOAD_LINES);

		#if _WIN64
		rootFrame.AddrPC.Offset = context.Rip;
		rootFrame.AddrFrame.Offset = context.Rbp;
		rootFrame.AddrStack.Offset = context.Rsp;
		#else
		rootFrame.AddrPC.Offset = context.Eip;
		rootFrame.AddrFrame.Offset = context.Ebp;
		rootFrame.AddrStack.Offset = context.Esp;
		#endif
	}

	inline void StackTrace::StackContext::Cleanup()
	{
		SymCleanup(process);
	}
}