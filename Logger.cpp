#include "Logger.h"
#include <Intsafe.h>

Logger::Logger(char* logName)
{
	nameOfLog = logName;
	writeCount = 0;
}

Logger::Logger()
{
	nameOfLog = "Debug.txt";
	writeCount = 0;
	Create();

	procHandle = GetProcessHeap();
}

void Logger::OpenStream()
{
	if (writeCount > 5)
	{
		if (logFile)
		{
			fclose(logFile);
		}


		logFile = fopen(nameOfLog, "a");
		writeCount = 0;
	}
	else
	{
		writeCount++;

	}
		
}
void Logger::Create()
{
	FILE* fp = fopen(nameOfLog, "r");
	if (fp)
	{
		fclose(fp);
		DeleteFile(nameOfLog);
	}

	logFile = fopen(nameOfLog, "w");
}

Logger::~Logger()
{
	fclose(logFile);
}
Logger* Logger::log;
void Logger::LogIt(LOGTYPE t, char* msg)
{
	return;
	int theLen = strlen(msg) + 16;
	char* debugMessage = new char[theLen];
	memset(debugMessage, 0, theLen);
	sprintf(debugMessage, "%s\n", msg);
	OutputDebugString(debugMessage);
	if (logFile)
	{
		OpenStream();
		fseek(logFile, 0, SEEK_END);
		fputs(debugMessage, logFile);
	}
	delete[] debugMessage;
}