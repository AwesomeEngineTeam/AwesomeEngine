#include <Awesome/Core/Logger/Logger.hpp>

#include <vector>
#include <set>

#include <sys/types.h>
#include <sys/stat.h>

//ported version by Tronkko
#include <dirent.h>

namespace aw
{
	std::map<const void*, std::string> Logger::s_addressMap;
	std::vector<std::stringstream*> Logger::s_streams;

	std::unique_ptr<Logger> Logger::m_instance = nullptr;

	Severity Logger::s_minSeverity = Severity::None;

	std::mutex Logger::s_streamMutex;

	Logger::Logger()
	{
		if (LOG_TO_FILE)
		{
            struct stat info;
            stat( LOG_DIR, &info );

            if( (info.st_mode & S_IFDIR) == 0 )
                MKDIR(LOG_DIR);

            DIR *dir;
            struct dirent *ent;

            dir = opendir(LOG_DIR);

            std::set<std::string> files;

            //clear . and .. considered as files..
            ent = readdir(dir);
            ent = readdir(dir);

            while ((ent = readdir (dir)) != nullptr)
                files.emplace(std::string(LOG_DIR) + "/" + ent->d_name);

            closedir (dir);

            while(files.size() >= MAX_LOG_FILES)
            {
                remove(files.begin()->c_str());
                files.erase(files.begin());
            }

            m_logFile.Open(LOG_FILE_PATH, OpenMethodBit::Write);
		}
	}

	std::string Logger::GetFormattedTime()
	{
		struct tm* t;
		time_t time = std::time(nullptr);

		#ifdef _WIN32
		struct tm tw;
		localtime_s(&tw, &time);
		t = &tw;
		#else
		t = std::localtime(&time);
		#endif

		char dst[18];

		std::strftime(dst, 18, "%y-%m-%d_%H-%M-%S", t);

		return dst;
	}
}