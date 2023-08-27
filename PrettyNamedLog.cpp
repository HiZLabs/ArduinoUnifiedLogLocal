#include "PrettyNamedLog.h"

PrettyNamedLog::PrettyNamedLog(LogHandler& handler, String name): PrettyGenericLog(handler),logName(name) { }

void PrettyNamedLog::level(int logLevel, String message) {
    if(!fmtBufAvailable)
        return;

	uint32_t now = millis();
	uint32_t days = now/1000/60/60/24;
	uint32_t hours = now/1000/60/60 % 24;
	uint32_t mins = now/1000/60 % 60;
	uint32_t secs = now/1000 % 60;
	uint32_t mils = now %1000;

	String prefix = formatString("%d:%02d:%02d:%02d.%03d ",days,hours,mins,secs,mils);
	const char* logLevelName = nameForLogLevel(logLevel);
	if(logLevelName)
		prefix += formatString("%-5s %-9s ", logLevelName, logName.c_str());
	else
		prefix += formatString("LVL%02d %-9s ",logLevel, logName.c_str());

	logHandler.write_message(logLevel, prefix + message);
}

PrettyNamedLog::~PrettyNamedLog() {}