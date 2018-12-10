namespace aw
{
	inline Logger* Logger::GetInstance()
	{
		if(m_instance == nullptr)
            m_instance.reset(new Logger());

		return m_instance.get();
	}

	inline void Logger::SetMinSeverity(Severity severity)
	{
		s_minSeverity = severity;
	}

	template<class... Args>
	inline void Logger::Log(Severity severity, std::string logMsg, Args&&... args)
	{
		Log(severity, logMsg.data(), args...);
	}

	inline void Logger::Log(Severity severity, std::string logMsg)
	{
		Log(severity, logMsg.data());
	}

	template<class... Args>
	inline void Logger::Log(Severity severity, const char* logMsg, Args&&... args)
	{
		if (severity < s_minSeverity)
			return;

		std::string msg = SeverityToString(severity); msg += logMsg; msg += '\n';//because operator+= is way faster than operator+

		m_logFile.Write(msg.data(), args...);
		
		char buffer[LOG_OPT_STREAMS_BUFFER_SIZE];
		sprintf_s(buffer, msg.c_str(), args...);

		s_streamMutex.lock();

		std::cout << buffer;
		for (auto& stream : s_streams)
			*stream << buffer;

		s_streamMutex.unlock();
	}

	inline void Logger::Log(Severity severity, const char* logMsg)
	{
		if (severity < s_minSeverity)
			return;

		std::string msg = SeverityToString(severity); msg += logMsg; msg += '\n';//because operator+= is way faster than operator+

		m_logFile.Write(msg.data());

		char buffer[LOG_OPT_STREAMS_BUFFER_SIZE];
		sprintf_s(buffer, "%s", msg.data());

		s_streamMutex.lock();

		std::cout << buffer;
		for (auto& stream : s_streams)
			*stream << buffer;

		s_streamMutex.unlock();
	}

	template<class... Args>
	inline void Logger::Throw(const char* errorMsg, Args&&... args)
	{
		std::string msg = "[FATAL ERROR] ";

		#ifdef ENABLE_STACK_TRACE
		const char* indent = "\n              ";

		auto stackTrace = StackTrace::GetStackTrace();
		stackTrace.pop_front(); //We don't need the Throw stackFrame

		msg += ParseStackTrace(stackTrace, indent);
		msg += indent;
		#endif

		if (m_logFile.IsOpen())
		{
			msg += "Exception: ";

			m_logFile.Write(msg.data(), sizeof(char), msg.size());
			m_logFile.Write(errorMsg, args...);
		}

		msg += errorMsg; msg += "\n";

		std::function<void()> f = [msg, args...]()
		{
			printf(msg.data(), args...);
		};

		throw f;
	}

	inline void Logger::Throw(const char* errorMsg)
	{
		std::string msg = "[FATAL ERROR] ";

		#ifdef ENABLE_STACK_TRACE
		const char* indent = "\n              ";

		auto stackTrace = StackTrace::GetStackTrace();
		stackTrace.pop_front(); //We don't need the Throw stackFrame

		msg += ParseStackTrace(stackTrace, indent);
		msg += indent;
		#endif

		if (m_logFile.IsOpen())
		{
			msg += "Exception: ";

			m_logFile.Write(msg.data(), sizeof(char), msg.size());
			m_logFile.Write(errorMsg);
		}

		msg += errorMsg; msg += "\n";

		std::function<void()> f = [msg]()
		{
			printf("%s", msg.data());
		};

		throw f;
	}

	inline void Logger::AddStream(std::stringstream* stream)
	{
		s_streamMutex.lock();
		s_streams.push_back(stream);
		s_streamMutex.unlock();
	}

	inline void Logger::LogStackTrace(Severity severity)
	{
		auto stackTrace = StackTrace::GetStackTrace();
		stackTrace.pop_front(); //We don't need the Throw stackFrame

		std::string sevIndent = SeverityToString(severity);
		sevIndent.insert(0, "\n");

		for (uint32 i = 1; i < sevIndent.size(); ++i)
			sevIndent[i] = ' ';

		Log(severity, ParseStackTrace(stackTrace, sevIndent.c_str()));
	}

	inline std::string Logger::ParseStackTrace(std::deque<StackTrace::StackFrameInfo>& stackTrace, const char* indent)
	{
		std::string subIndent = "\t\t";

		std::string result;
		StackTrace::StackFrameInfo trace;

		while(stackTrace.size())
		{
			trace = stackTrace.front();
			stackTrace.pop_front();

			std::stringstream stream;
			stream << std::hex << trace.functionAddress;

			result += std::string("In function ") + trace.functionName;
			result += indent;

			result += subIndent + "address = 0x" + stream.str();
			result += indent;

			result += subIndent + "module = " + trace.moduleName;
			result += indent;

			result += subIndent + "file = " + trace.file + ":" + std::to_string(trace.line);
			result += indent;
		}

		stackTrace.shrink_to_fit();

		return result;
	}

	inline void Logger::RegisterAddressName(const void* address, const std::string& name)
	{
		s_addressMap[address] = name;
	}

	inline std::string Logger::AddressToString(const void* address)
	{
		std::stringstream ss;
		if (s_addressMap.count(address) == 0)
			ss << std::hex << address;
		else
			ss << s_addressMap[address];// << ", " << std::hex << address/* << ", " << *reinterpret_cast<uint32*>(address)*/;

		return ss.str();
	}
}