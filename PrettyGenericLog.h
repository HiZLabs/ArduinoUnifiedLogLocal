#pragma once

#include "LogHandler.h"
#include "LogLevels.h"

class PrettyGenericLogCriticalSection {
public:
    PrettyGenericLogCriticalSection();
    ~PrettyGenericLogCriticalSection();
private:
    static long refCount;
};

class PrettyGenericLog {
protected:
	LogHandler& logHandler;
    static bool fmtBufAvailable() { return fmtBuf && fmtBufSize; }
public:
	PrettyGenericLog(LogHandler& handler);

	virtual void level(int logLevel, String message);

	void trace(String message);
	void trace(const char * fmt, ...);
    void debug(String message);
	void debug(const char * fmt, ...);
	void info(String message);
	void info(const char * fmt, ...);
	void warn(String message);
	void warn(const char * fmt, ...);
	void error(String message);
	void error(const char * fmt, ...);

	void operator()(int logLevel, String message);
	void operator()(int logLevel, const char* fmt, ...);
	void operator()(String message);

	virtual ~PrettyGenericLog();

	static const char* nameForLogLevel(int level);
    static bool setBufferSize(size_t size);
    static String formatString(const char * fmt, ...);
    static String formatString(const char* fmt, va_list args);
    static String formatStringWithBuf(char* buf, const size_t bufSize, const char * fmt, ...);
    static String formatStringWithBuf(char* buf, const size_t bufSize, const char* fmt, va_list args);
private:
    static char* fmtBuf;
    static size_t fmtBufSize;
};