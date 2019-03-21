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


/* Traci Control Unit. Sends commands to SUMO*/
class TraciControlToSumo
{
public:

	/*Constructor for TraciControlToSumo class*/
	TraciControlToSumo() {};
	/*Destructor for TraciControlToSumo class*/
	~TraciControlToSumo() {};
	/*Initialize calls ZeroMemory on StartupInfo, sets Startup.cb to sizeof Startup, and initializes cmdArgs*/
	void initialize();
	/*Boots up Sumo using CreateProcess and CmdArgs*/
	void runSumo();
	/*Runs a client that connects to Sumo and gives it commands*/
	void runClient();

private:

	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	STARTUPINFO StartupInfo; //This is an [in] parameter
	std::string sumoCmd; //Used to construct sumoCfgString
	std::string sumoCfgString; //Used to construct cmdArgs
	LPSTR cmdArgs; //Commands passed to the console during CreateProcess
	Client client; //Sends commands to Sumo
	bool random;
	int seed;
};

class TraciControlToSumoRandom : public TraciControlToSumo
{
public:
	/* Same as TraciControlToSumo, except it intializes a seed and creates an argument for it in cmdArgs*/
	void initialize();
	/*Same as TraciControlToSumo, except it also outputs cmdArgs then waits*/
	void runClient();

private:
	int seed;
	
};