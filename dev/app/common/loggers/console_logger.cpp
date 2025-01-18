#include "console_logger.h"
#include <iostream> 

using namespace app::log;

CLogger::CLogger()
{
}

CLogger::~CLogger() 
{
}

void CLogger::write(const std::string& message)
{
  std::cout << message;
}

