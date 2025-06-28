#pragma once
#include "Logger.h"
#include <fstream>
#include <iostream>

class Log : public Logger
{
	std::ofstream logFile;
public:
	Log() : Logger(std::cout, logFile)
	{
		logFile = std::ofstream(L"BRSDLOG.log", std::ios::app);
	}
};

inline Log log = Log();