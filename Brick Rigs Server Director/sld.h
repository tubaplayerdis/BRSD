#pragma once
#include "Logger.h"
#include "messages.h"

class SDLogger : public Logger
{
	virtual void LoggingFeedback(std::string message) override
	{
		MessageHost(message, L"LOGGING SYS");
	}

	virtual void CriticalFeedback(std::string message) override
	{
		MessageHost(message, L"CRITICAL");
	}

public:
	SDLogger() : Logger(L"BRSDLOG.log") {}
};

//inline SDLogger sld = SDLogger();