#include "console_logger.h"
#include <ctime>
#include <filesystem> 
#include <thread> 

using namespace app::log;

CLogger::CLogger()
{
}

CLogger::~CLogger() 
{
}

void CLogger::write(LogLevel level, const std::string file, const int line, const uint64_t id, const std::string& message)
{
   if( level > m_level )
    return;
      
  time_t now{ time(0) };
  tm* timeinfo{ localtime(&now) };
  char timestamp[20];
  strftime( timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo );

  std::filesystem::path p(file); 
  std::string s{p.filename()};

  std::cout << level_tostr( level ) << 
               " (" << s.substr(0, s.size() -1 ) << ":" << line << ")" 
               "[" << timestamp << "] " <<
               "("<< id << ")" << ": " << message << std::endl;
}

