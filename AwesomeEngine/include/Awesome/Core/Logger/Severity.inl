namespace aw
{
	inline constexpr const char* SeverityToString(Severity severity)
	{
		switch (severity)
		{
		case Severity::Log:
			return "[Log] ";
		case Severity::Warning:
			return "[Warning] ";
		case Severity::Error:
			return "[Error] ";

		case Severity::None:
		default:
			return "";
		}
	}
}