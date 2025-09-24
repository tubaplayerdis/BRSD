/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     LoggerStream.h                                              */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    27 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <locale>
#include <chrono>
#include <time.h>
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
	std::streambuf* targetf;
	std::streambuf* targetc;
	LogLevel lLevel;
	bool bLineStart;
	void WriteTimestampAndPrefix();
public:
	/// <summary>
	/// Constructs a LoggerStream object with specified stream buffers for file and console output.
	/// </summary>
	/// <param name="targetf">Pointer to the stream buffer used for file output.</param>
	/// <param name="targetc">Pointer to the stream buffer used for console output.</param>
	LoggerStream(std::streambuf* targetF, std::streambuf* targetC) : targetf(targetF), targetc(targetC), bLineStart(true), lLevel(INFO) {}

	void SetLogLevel(LogLevel level);
protected:

	virtual int overflow(int c) override {
		if (c != EOF) {
			if (bLineStart) {
				WriteTimestampAndPrefix();
				bLineStart = false;
			}
			if (targetf) targetf->sputc(static_cast<char>(c));
			if (targetc) targetc->sputc(static_cast<char>(c));

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
	std::ostringstream oss;
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	tm temi = tm(); localtime_s(&temi, &now_c);
	oss << "[" << std::put_time(&temi, "%F %T") << "] ";
	switch (lLevel)
	{
		case DEBUG:
			oss << "[DEBUG] ";
			break;
		case INFO:
			oss << "[INFO] ";
			break;
		case WARNING:
			oss << "[WARNING] ";
			break;
		case CRITICAL:
			oss << "[CRITICAL] ";
			break;
		default:
			oss << "[INFO] ";
			break;
	}
	std::string prefix = oss.str();
	if (targetf) targetf->sputn(prefix.data(), prefix.size());
}

inline void LoggerStream::SetLogLevel(LogLevel level)
{
	lLevel = level;
}

