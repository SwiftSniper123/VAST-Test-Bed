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
	TraciControlToSumo();
	/*Initialize calls ZeroMemory on StartupInfo, sets Startup.cb to sizeof Startup, and initializes cmdArgs*/
	void initialize();
	/*Boots up Sumo using CreateProcess and CmdArgs*/
	void runSumo();
	/*Runs a client that connects to Sumo and gives it commands*/
	void runClient();
	/*Returns sumoCfgString*/
	std::string getSumoCfgString();
	/* Returns sumoCmd*/
	std::string getSumoCmd();
	/*Returns whether Sumo uses a random seed or not*/
	bool getRandom();
	/*Returns random flag*/
	bool getRandomFlag();
	/*Returns the seed*/
	int getSeed();
	/*Returns whether CreateProcess worked or not*/
	bool getCreateProcessWorked();

private:

	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	STARTUPINFO StartupInfo; //This is an [in] parameter
	std::string sumoCmd; //Used to construct cmdArgs
	std::string sumoCfgString; //Used to construct sumoCmd
	LPWSTR cmdArgs; //Commands passed to the console during CreateProcess
	Client client; //Sends commands to Sumo
	bool random; //Value for whether the user wants random routes or not.
	bool randomFlag; //Shows if the user entered a value for random. Used for unit testing.
	bool CreateProcessWorked;
	int seed; //Seed for random route generation
};
