#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <time.h>
#include <sstream>
#include <codecvt>
#include <locale>

//needs to work like std::cout. with endl's and continus operator chains of <<

enum LogLevel
{
	DEBUG = 0,
	INFO = 1,
	WARNING = 2,
	CRITICAL = 3
};

class Logger
{
private:
	HANDLE hFile = nullptr;
	LogLevel lLevel = INFO;
	bool bDisabled = false;
	static std::wstring WGetTimestamp();
	static std::string GetTimestamp();
	static std::wstring WFormatMessage(std::wstring message, LogLevel level);
	static std::string SFormatMessage(std::string message, LogLevel level);
public:
	static std::string ConvertWSTR(std::wstring input);
	static std::wstring ConvertSTR(std::string input);
public:
	Logger(const wchar_t* filename);
	~Logger();

	void SetLogLevel(LogLevel level);

	void operator<<(const char* message);
	void operator<<(std::string message);
	void operator<<(const wchar_t* message);
	void operator<<(std::wstring message);

	template<typename R>
	void operator<<(R message);
protected:

	/// <summary>
	/// Logging system specific feedback implementation, for things like bad buffers.
	/// </summary>
	/// <param name="message">message sent by the logging system</param>
	virtual void LoggingFeedback(std::string message) = 0;

	/// <summary>
	/// CRITICAL logging level feedback implemtation
	/// </summary>
	/// <param name="message">The error message to be reported.</param>
	virtual void CriticalFeedback(std::string message) = 0;
};

inline std::wstring Logger::WGetTimestamp()
{
	time_t now = time(0);
	tm* timeinfo = localtime(&now);
	wchar_t timestamp[20];
	wcsftime(timestamp, 20, L"%Y-%m-%d %H:%M:%S", timeinfo);
	return std::wstring(timestamp);
}

inline std::string Logger::GetTimestamp()
{
	time_t now = time(0);
	tm* timeinfo = localtime(&now);
	char timestamp[20];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
	return std::string(timestamp);
}

inline std::wstring Logger::WFormatMessage(std::wstring message, LogLevel level)
{
	std::wstring mlevel = L"";
	switch (level)
	{
		case DEBUG:
			mlevel = L"DEBUG";
			break;
		case INFO:
			mlevel = L"INFO";
			break;
		case WARNING:
			mlevel = L"WARNING";
			break;
		case CRITICAL:
			mlevel = L"CRITICAL";
			break;
		default:
			mlevel = L"INFO";
			break;
	}
	std::wstringstream stream;
	stream << L"[" << WGetTimestamp() << L"]" << L" " << mlevel << L": " << message << std::endl;
	return stream.str();
}

inline std::string Logger::SFormatMessage(std::string message, LogLevel level)
{
	std::string mlevel = "";
	switch (level)
	{
	case DEBUG:
		mlevel = "DEBUG";
		break;
	case INFO:
		mlevel = "INFO";
		break;
	case WARNING:
		mlevel = "WARNING";
		break;
	case CRITICAL:
		mlevel = "CRITICAL";
		break;
	default:
		mlevel = "INFO";
		break;
	}
	std::stringstream stream;
	stream << "[" << GetTimestamp() << "]" << " " << mlevel << ": " << message << std::endl;
	return stream.str();
}

inline std::string Logger::ConvertWSTR(std::wstring input)
{
	std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > convert;
	return convert.to_bytes(input);
}

inline std::wstring Logger::ConvertSTR(std::string input)
{
	std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > convert;
	return convert.from_bytes(input);
}

inline Logger::Logger(const wchar_t* filename)
{
	hFile = CreateFile(filename, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		bDisabled = true;
		LoggingFeedback("File HANDLE failed to initalize. Disabling logging system!");
	}
}

inline Logger::~Logger()
{
	CloseHandle(hFile);
	hFile = nullptr;
}

inline void Logger::SetLogLevel(LogLevel level)
{
	lLevel = level;
}

inline void Logger::operator<<(const char* message)
{
	this->operator<<(std::string(message));
}

inline void Logger::operator<<(std::string message)
{
	if (bDisabled) return;
	std::string nmessage = SFormatMessage(message, this->lLevel);
	if (lLevel == CRITICAL) CriticalFeedback(message);
	if (!WriteFile(hFile, nmessage.c_str(), sizeof(message), NULL, NULL)) { LoggingFeedback("Failed to write file! Last Error: " + std::to_string(GetLastError())); }
	if (!FlushFileBuffers(hFile)) { LoggingFeedback("Failed to flush file buffer! Last Error: " + std::to_string(GetLastError())); }
}

inline void Logger::operator<<(const wchar_t* message)
{
	this->operator<<(std::wstring(message));
}

inline void Logger::operator<<(std::wstring message)
{
	if (bDisabled) return;
	std::wstring nmessage = WFormatMessage(message, this->lLevel);
	if (lLevel == CRITICAL) CriticalFeedback(ConvertWSTR(message));
	if (!WriteFile(hFile, nmessage.c_str(), sizeof(message), NULL, NULL)) { LoggingFeedback("Failed to write file! Last Error: " + std::to_string(GetLastError())); }
	if (!FlushFileBuffers(hFile)) { LoggingFeedback("Failed to flush file buffer! Last Error: " + std::to_string(GetLastError())); }
}

template<typename R>
inline void Logger::operator<<(R message)
{
	this->operator<<(std::to_string(R));
}
