#include "PrettyGenericLog.h"


PrettyGenericLogCriticalSection::PrettyGenericLogCriticalSection() 
{
	////TODO: figure out how to make this work. Maybe a mutex? This strategy doesn't work on multiple cores.
	// noInterrupts();
	// ++refCount;
}
PrettyGenericLogCriticalSection::~PrettyGenericLogCriticalSection()
{
	////TODO: figure out how to make this work. Maybe a mutex? This strategy doesn't work on multiple cores.
	// --refCount;
	// if(!refCount)
	// 	interrupts();
}
long PrettyGenericLogCriticalSection::refCount = 0;

char* PrettyGenericLog::fmtBuf = NULL;
size_t PrettyGenericLog::fmtBufSize = 0;

String PrettyGenericLog::formatString(const char * fmt, ...) {
	if(!fmtBufAvailable())
		return "";

    va_list args;
    va_start(args, fmt);
    String ret = formatString(fmt, args);
    va_end(args);
    return ret;
}

PrettyGenericLog::PrettyGenericLog(LogHandler& handler) : logHandler(handler) { }

String PrettyGenericLog::formatString(const char* fmt, va_list args) {
	return formatStringWithBuf(fmtBuf, fmtBufSize, fmt, args);
}

String PrettyGenericLog::formatStringWithBuf(char* buf, const size_t bufSize, const char * fmt, ...) {
	if(!buf || !bufSize)
		return String("");

    va_list args;
    va_start(args, fmt);
    String ret = formatStringWithBuf(buf, bufSize, fmt, args);
    va_end(args);
    return ret;
}

String PrettyGenericLog::formatStringWithBuf(char* buf, const size_t bufSize, const char* fmt, va_list args) {
	PrettyGenericLogCriticalSection cs;

	if(!buf || !bufSize)
		return String("");

    vsnprintf(buf, bufSize / sizeof(fmtBuf[0]) - 1, fmt, args);
    buf[bufSize-1] = '\0';
    return String((const char *)buf);
}

bool PrettyGenericLog::setBufferSize(size_t size) 
{
	PrettyGenericLogCriticalSection cs;
	if(fmtBuf)
		free(fmtBuf);

	fmtBuf = (char *)malloc(size);
	if(fmtBuf) 
	{
		fmtBufSize = size;
		return true;
	}

	fmtBufSize = 0;
	return false;
}

void PrettyGenericLog::level(int logLevel, String message) {
	if(!fmtBufAvailable())
		return;

	uint32_t now = millis();
	uint32_t days = now/1000/60/60/24;
	uint32_t hours = now/1000/60/60 % 24;
	uint32_t mins = now/1000/60 % 60;
	uint32_t secs = now/1000 % 60;
	uint32_t mils = now % 1000;

	String prefix;
	const char* logLevelName = nameForLogLevel(logLevel);
	if(logLevelName)
		prefix = formatString(fmtBuf, fmtBufSize, "%d:%02d:%02d:%02d.%03d %s ",days,hours,mins,secs,mils,logLevelName);
	else
		prefix = formatString(fmtBuf, fmtBufSize, "%d:%02d:%02d:%02d.%03d LVL%d ",days,hours,mins,secs,mils,logLevel);


	logHandler.write_message(logLevel, prefix + message);
}

void PrettyGenericLog::trace(String message) {
	level(LOG_LEVEL_TRACE, message);
}

void PrettyGenericLog::trace(const char* fmt, ...) {
	if(!fmtBufAvailable())
		return;

	va_list args;
	va_start(args, fmt);
	level(LOG_LEVEL_TRACE, formatString(fmt, args));
	va_end(args);
}

void PrettyGenericLog::debug(String message) {
	level(LOG_LEVEL_DEBUG, message);
}

void PrettyGenericLog::debug(const char* fmt, ...) {
	if(!fmtBufAvailable())
		return;

	va_list args;
	va_start(args, fmt);
	level(LOG_LEVEL_DEBUG, formatString(fmt, args));
	va_end(args);
}

void PrettyGenericLog::info(String message) {
	level(LOG_LEVEL_INFO, message);
}

void PrettyGenericLog::info(const char* fmt, ...) {
	if(!fmtBufAvailable())
		return;

	va_list args;
	va_start(args, fmt);
	level(LOG_LEVEL_INFO, formatString(fmt, args));
	va_end(args);
}

void PrettyGenericLog::warn(String message) {
	level(LOG_LEVEL_WARN, message);
}

void PrettyGenericLog::warn(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	level(LOG_LEVEL_WARN, formatString(fmt, args));
	va_end(args);
}

void PrettyGenericLog::error(String message) {
	level(LOG_LEVEL_ERROR, message);
}

void PrettyGenericLog::error(const char* fmt, ...) {
	if(!fmtBufAvailable())
		return;

	va_list args;
	va_start(args, fmt);
	level(LOG_LEVEL_ERROR, formatString(fmt, args));
	va_end(args);
}


void PrettyGenericLog::operator()(int logLevel, String message) {
	level(logLevel, message);
}

void PrettyGenericLog::operator()(int logLevel, const char* fmt, ...) {
	if(!fmtBufAvailable())
		return;

	va_list args;
	va_start(args, fmt);
	level(logLevel, formatString(fmt, args));
	va_end(args);
}

void PrettyGenericLog::operator()(String message) {
	level(LOG_LEVEL_INFO, message);
}

PrettyGenericLog::~PrettyGenericLog() {}

const char* PrettyGenericLog::nameForLogLevel(int level) {
	switch(level) {
		case LOG_LEVEL_TRACE:
			return "TRACE";
		case LOG_LEVEL_DEBUG:
			return "DEBUG";
		case LOG_LEVEL_INFO:
			return "INFO";
		case LOG_LEVEL_WARN:
			return "WARN";
		case LOG_LEVEL_ERROR:
			return "ERROR";
		default:
			return NULL;
	}
}