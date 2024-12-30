#include "console_logger.h"
#include <ctime>
#include <thread> 

using namespace app::log;

CLogger::CLogger()
{
}

CLogger::~CLogger() 
{
}


void CLogger::write(LogLevel level, 
                    const uint64_t id, 
                    const std::string& message,
                    const std::source_location location /*= std::source_location::current() */)
{
   if( level > m_level )
    return;
      
  time_t now{ time(0) };
  tm* timeinfo{ localtime(&now) };
  char timestamp[20];
  strftime( timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo );
              
  std::cout << level_tostr( level ) << 
               location.file_name() << ':' <<
               location.line() << ':' <<
               location.column() << " [" <<
               location.function_name() << "] : " <<
               "[" << timestamp << "] " <<
               "("<< id << ")" << ": " << message << std::endl;
}

