#include "TraCIAPI.h"
#include <string>
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <time.h>

class Client : public TraCIAPI {
public:
	Client() {};
	~Client() {};
};

class SumoTraciTestClass
{
public:
	SumoTraciTestClass() {};
	~SumoTraciTestClass() {};
	/*Initialize calls ZeroMemory on StartupInfo, sets Startup.cb to sizeof Startup, and initializes cmdArgs*/
	void initialize();
	void runSumo();
	void runClient();

protected:

	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	STARTUPINFO StartupInfo; //This is an [in] parameter
	std::string sumoCmd;
	std::string sumoCfgString;
	LPSTR cmdArgs;
	Client client;
};

class SumoTraciRandomTestClass : public SumoTraciTestClass
{
public:
	void initialize();
	void runClient();

private:
	int seed;
	
};