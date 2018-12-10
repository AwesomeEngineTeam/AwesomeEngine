namespace aw
{
	namespace ph
	{
		inline void DebugCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
		{
			if (code == physx::PxErrorCode::Enum::eNO_ERROR)
				return;

			std::string str = "PhysX ErrorCode: " + std::to_string(code) + "\tin File: " + file + "\tat Line: " + std::to_string(line) + '\n' + message;

			switch (code)
			{
			case physx::PxErrorCode::Enum::eDEBUG_INFO:
				LOG_LOG(str);
				break;
			case physx::PxErrorCode::Enum::eDEBUG_WARNING:
				LOG_WARNING(str);
				break;
			default:
				LOG_ERROR(str);
				break;
			}
		}
	}
}