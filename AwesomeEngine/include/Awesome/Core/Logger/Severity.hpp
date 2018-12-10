#pragma once

#ifndef AWESOME_CORE_SEVERITY_GUARD
#define AWESOME_CORE_SEVERITY_GUARD

#include <string>

namespace aw
{
	enum class Severity
	{
		None,
		Log,
		Warning,
		Error
	};

	inline constexpr const char* SeverityToString(Severity severity);

	//const Severity None = Severity::None;
	//const Severity Log = Severity::Log;
	//const Severity Warning = Severity::Warning;
	//const Severity Error = Severity::Error;
}

#include <Awesome/Core/Logger/Severity.inl>

#endif //GUARD