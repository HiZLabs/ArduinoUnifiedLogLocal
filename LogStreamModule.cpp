#include "LogStreamModule.h"
#include "LogLevels.h"

LogStreamModule::LogStreamModule(Stream& stream) {
	_stream = &stream;
}

void LogStreamModule::write_message(String message) {
	_stream->println(message);
}