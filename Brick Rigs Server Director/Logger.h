#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <codecvt>
#include <locale>
#include <chrono>
#include <ostream>
#include <fstream>

//needs to work like std::cout. with endl's and continus operator chains of <<

enum LogLevel
{
	DEBUG = 0,
	INFO = 1,
	WARNING = 2,
	CRITICAL = 3
};

class LoggerStream : public std::streambuf
{
private:
	std::ostream& targetf;
	std::ostream& targetc;
	LogLevel lLevel;
	bool bLineStart;
	void WriteTimestampAndPrefix();
public:
	static std::string ConvertWSTR(std::wstring input);
	static std::wstring ConvertSTR(std::string input);
public:
	LoggerStream(std::ostream& target1, std::ostream& target2) : targetf(target1), targetc(target2), bLineStart(true), lLevel(INFO) {}

	void SetLogLevel(LogLevel level);
protected:

	virtual int overflow(int c) override {
		if (c != EOF) {
			if (bLineStart) {
				WriteTimestampAndPrefix();
				bLineStart = false;
			}
			targetf.put(static_cast<char>(c));
			targetc.put(static_cast<char>(c));

			if (c == '\n') {
				bLineStart = true;
			}
		}
		return c;
	}

	virtual std::streamsize xsputn(const char* s, std::streamsize n) override {
		std::streamsize total = 0;
		for (std::streamsize i = 0; i < n; ++i) {
			if (overflow(s[i]) == EOF) {
				break;
			}
			total++;
		}
		return total;
	}
};

inline void LoggerStream::WriteTimestampAndPrefix()
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	targetf << "[" << std::put_time(std::localtime(&now_c), "%F %T") << "] ";
	switch (lLevel)
	{
	case DEBUG:
		targetf << "[DEBUG] ";
		break;
	case INFO:
		targetf << "[INFO] ";
		break;
	case WARNING:
		targetf << "[WARNING] ";
		break;
	case CRITICAL:
		targetf << "[CRITICAL] ";
		break;
	default:
		targetf << "[INFO] ";
		break;
	}
	targetf << "[INFO] ";
}

inline std::string LoggerStream::ConvertWSTR(std::wstring input)
{
	std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > convert;
	return convert.to_bytes(input);
}

inline std::wstring LoggerStream::ConvertSTR(std::string input)
{
	std::wstring_convert< std::codecvt_utf8<wchar_t>, wchar_t > convert;
	return convert.from_bytes(input);
}

inline void LoggerStream::SetLogLevel(LogLevel level)
{
	lLevel = level;
}

class Logger : public std::ostream
{
public:
	Logger(std::ostream& target1, std::ostream& target2) : std::ostream(&buf), buf(target1, target2) {}

private:
	LoggerStream buf;
};


