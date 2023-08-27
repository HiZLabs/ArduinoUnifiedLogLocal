#pragma once

#include "PrettyGenericLog.h"

class PrettyNamedLog : public PrettyGenericLog {
private:
	const String logName;
public:
	PrettyNamedLog(LogHandler& handler, String name);

	virtual void level(int logLevel, String message);
	virtual ~PrettyNamedLog();
};

#define LOGGER(loggerVar, moduleName) static PrettyNamedLog loggerVar(logging_handler, moduleName);