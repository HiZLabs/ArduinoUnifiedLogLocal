#pragma once

#include <Arduino.h>
#include "LogModule.h"


class LogStreamModule : public LogModule {
private:
	Stream* _stream;
public:
	LogStreamModule(Stream& stream);

	void write_message(String message);
};
