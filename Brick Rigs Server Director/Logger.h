/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Copyright (c) Aaron Wilk 2025, All rights reserved.                     */
/*                                                                            */
/*    Module:     logger.h                                                    */
/*    Author:     Aaron Wilk                                                  */
/*    Created:    28 June 2025                                                */
/*                                                                            */
/*    Revisions:  V0.1                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#pragma once
#include "LoggerStream.h"
#include <fstream>
#include <iostream>

#ifndef _DEBUG
	#define InitLogging() logger::Initalize()
	#define DestroyLogging() logger::Destroy();
#else
	#define InitLogging()
	#define DestroyLogging()
#endif // !_DEBUG

namespace logger
{
	inline std::ofstream logFile;
	inline std::streambuf* originalstdout = nullptr;
	inline LoggerStream* streamtee = nullptr;

	inline void Initalize()
	{
		logFile = std::ofstream("brsdlog.log", std::ios::trunc);
		originalstdout = std::cout.rdbuf();
		streamtee = new LoggerStream(logFile.rdbuf(), originalstdout);
		std::cout.rdbuf(streamtee);
	}

	inline void Destroy()
	{
		std::cout << "Destroying Logging System!" << std::endl;
		std::cout << "Uninjecting BRSD!" << std::endl;
		std::cout.rdbuf(originalstdout);
		logFile.close();
		if (!streamtee) return;
		delete streamtee;
		streamtee = nullptr;
	}
}