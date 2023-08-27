#ifndef LogSerialModule_h
#define LogSerialModule_h

#include <Arduino.h>
#include "LogModule.h"


class LogSerialModule : public LogModule {
public:
	LogSerialModule(int baud);

	void write_message(String message);
};

#endif
