#pragma once
//TODO Add LogLayers like UE
#ifndef AWESOME_CORE_LOGGER_GUARD
#define AWESOME_CORE_LOGGER_GUARD

#include <Awesome/Core/Types.hpp>
#include <Awesome/Core/Macro.hpp>
#include <Awesome/Core/Logger/Severity.hpp>
#include <Awesome/Core/File.hpp>
#include <Awesome/Core/Logger/StackTrace.hpp>

#include <iostream>
#include <ctime>
#include <iomanip>

#include <algorithm>
#include <map>

#include <functional>
#include <sstream>

#include <memory>

#include <mutex>

#ifdef _WIN32
	#include <experimental/filesystem>
#else
	#include <experimental/filesystem>
#endif

#if defined(_WIN32) && !defined(__MINGW32__)
    #define MKDIR(dirPath) system((std::string("mkdir ") + dirPath).c_str());
#else
    #define MKDIR(dirPath) system((std::string("mkdir -p ") + dirPath).c_str());
#endif

namespace fs = std::experimental::filesystem::v1;

#define LOG_OPT_STREAMS_BUFFER_SIZE (1024 * 3)

#define LOG_DIR "logs"
#define LOG_FILE_PATH (std::string(LOG_DIR) + "/" + aw::Logger::GetFormattedTime() + ".log").data()

#define LOG_TO_FILE true
#define MAX_LOG_FILES 5

#ifdef _DEBUG
#define ENABLE_STACK_TRACE
#endif

#ifdef ENABLE_STACK_TRACE
/**
* Logs the stack trace in the severity of your choice. Note that this macro is only effective in debug mode
*
* @param A Severity = (None, Log, Warning, Error).
*/
#define LOG_STACK_TRACE(severity) aw::Logger::GetInstance()->LogStackTrace(severity);
#else
/**
* DISABLED IN RELEASE MODE
* Logs the stack trace in the severity of your choice.
*
* @param A Severity = (None, Log, Warning, Error).
*/
#define LOG_STACK_TRACE(severity)
#endif

/**
* Log macro
*
* @param A Severity = (None, Log, Warning, Error).
* @param B The string to be logged.
*/
#define LOG(severity, logMsg, ...) aw::Logger::GetInstance()->Log(severity, logMsg, ## __VA_ARGS__);
//#define LOG(severity, logMsg, ...) ;

#define LOG_NONE(LogMsg, ...) LOG(aw::Severity::None, LogMsg, ## __VA_ARGS__)
#define LOG_LOG(LogMsg, ...) LOG(aw::Severity::Log, LogMsg, ## __VA_ARGS__)
#define LOG_WARNING(LogMsg, ...) LOG(aw::Severity::Warning, LogMsg, ## __VA_ARGS__)
#define LOG_ERROR(LogMsg, ...) LOG(aw::Severity::Error, LogMsg, ## __VA_ARGS__)

#define SET_LOG_MIN_SEVERITY(severity) aw::Logger::SetMinSeverity(severity);

/**
* Throws exceptions (use it like a printf). Use this instead of throw to get a track of it in the log file. Gets the stackTrace if ENABLE_STACK_TRACE is defined
*
* @param A The message to be thrown.
* @param B va_args.
*/
#define THROW(errorMsg, ...) aw::Logger::GetInstance()->Throw(errorMsg, ## __VA_ARGS__);

//#define THROW(errorMsg, ...) aw::Logger::GetInstance()->Throw(std::string(__func__) + " (" + __FILE__ + ":" + std::to_string(__LINE__) + "):\n              Error: ", errorMsg, __VA_ARGS__);

/**
* Throws an exception if the predicate is true
*
* @param A predicate being evaluated.
* @param B error to be thrown.
* @param C va_args.
*/
#define ASSERT(predicate, errorMsg, ...) if(predicate) THROW(errorMsg, ## __VA_ARGS__);

#ifdef _DEBUG
#define ASSERT_DEBUG(predicate, errorMsg, ...) if(predicate) THROW(errorMsg, ## __VA_ARGS__);
#else
#define ASSERT_DEBUG(predicate, errorMsg, ...)
#endif

/**
* Registers a variable's address and maps it with its name
*
* @param A variable to register.
*/
#define REGISTER_UNNAMED_VAR(x) do { aw::Logger::RegisterAddressName( &(x), (#x) ); } while(false);

/**
* Registers a variable's address and maps it with a name
*
* @param A variable to register.
* @param B name linked to the variable.
*/
#define REGISTER_NAMED_VAR(x, y) do { aw::Logger::RegisterAddressName( &(x), (y) ); } while(false);

#define ID(x) x
//#define GET_MACRO(_1,_2,NAME,...) NAME
#define REGISTER_VAR(...) ID(GET_MACRO(__VA_ARGS__, REGISTER_NAMED_VAR, REGISTER_UNNAMED_VAR)(__VA_ARGS__))

/**
* Converts a std::string returned by aw::Logger::AddressToString to a null-terminated array of char
*
* @param A variable (not address) to pass to aw::Logger::AddressToString.
*/
#define VAR_TO_C_STR(x) aw::Logger::AddressToString(&(x)).c_str()

namespace aw
{
	class Logger : NonCopyable
	{
		static std::map<const void*, std::string> s_addressMap;
		static AWESOME_CORE_API std::vector<std::stringstream*> s_streams;

		static AWESOME_CORE_API std::unique_ptr<Logger> m_instance;

		static AWESOME_CORE_API Severity s_minSeverity;

		File m_logFile;
		static AWESOME_CORE_API std::mutex s_streamMutex;

		AWESOME_CORE_API Logger();

		inline std::string ParseStackTrace(std::deque<StackTrace::StackFrameInfo>&, const char* indent = "\n");

	public:
        ~Logger() = default;

		inline static Logger* GetInstance();

		static std::string GetFormattedTime();

		static inline void SetMinSeverity(Severity severity);

		template<class... Args>
		inline void Log(Severity severity, std::string logMsg, Args&&... args);

		template<class... Args>
		inline void Log(Severity severity, const char* logMsg, Args&&... args);

		inline void Log(Severity severity, std::string logMsg);

		inline void Log(Severity severity, const char* logMsg);

		inline void LogStackTrace(Severity);

		template<class... Args>
		inline void Throw(const char* errorMsg, Args&&... args);

		inline void Throw(const char* errorMsg);

		inline static void AddStream(std::stringstream* stream);

		inline static void RegisterAddressName(const void* address, const std::string& name);

		inline static std::string AddressToString(const void* address);
	};
}

#include <Awesome/Core/Logger/Logger.inl>

#endif //GUARD