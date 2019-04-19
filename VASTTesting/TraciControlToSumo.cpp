#include "TraciControlToSumo.h"


TraciControlToSumo::TraciControlToSumo()
{
	random = false;
	randomFlag = false;
	CreateProcessWorked = false;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo)); //Fills StartupInfo with zeros
	StartupInfo.cb = sizeof StartupInfo; //Only parameter of StartupInfo that needs to be configured

}
void TraciControlToSumo::initialize()
{
	std::cout << "Enter 0 for predetermined seed mode, 1 for random mode: ";
	std::cin >> random;
	randomFlag = true;

	std::cout << "Input path to SUMO config file: ";
	std::cin >> sumoCfgString;


	if (random == true)
	{
		srand(time(NULL)); //Provides a seed for the random number generator

		seed = rand() % 10000; //Generates a random seed for Sumo

		sumoCmd = "sumo -c " + sumoCfgString + " --remote-port 1337" + " --seed " + std::to_string(seed);

	}

	else
		sumoCmd = "sumo -c " + sumoCfgString + " --remote-port 1337";

	//Converting sumoCmd from a string into LPWSTR and storing it in cmdArgs
	wchar_t wtext[100];
	mbstowcs(wtext, sumoCmd.c_str(), sumoCmd.length() + 1);//Plus null
	cmdArgs = wtext;
}

void TraciControlToSumo::runSumo()
{
	//Spawns a process and it's own separate thread
	if(CreateProcess(NULL, cmdArgs,
		NULL, NULL, FALSE, 0, NULL,
		NULL, &StartupInfo, &ProcessInfo));
	{
		CreateProcessWorked = true;
	}

	Sleep(2500); //SUMO needs to start entirely before runClient() is called. This assures there's enough time for SUMO to boot up.
}

void TraciControlToSumo::runClient()
{
	//Connects the client, outputs time, runs for 5000 time steps, closes the client, and outputs time once more
	client.connect("localhost", 1337);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	std::cout << "run 500 steps ...\n";
	client.simulationStep(500);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	std::cout << "Command line: " << sumoCmd << std::endl;
	client.close();
	system("PAUSE");
}

std::string TraciControlToSumo::getSumoCmd()
{
	return sumoCmd;
}

bool TraciControlToSumo::getRandom()
{
	return random;
}

std::string TraciControlToSumo::getSumoCfgString()
{
	return sumoCfgString;
}

int TraciControlToSumo::getSeed()
{
	return seed;
}

bool TraciControlToSumo::getRandomFlag()
{
	return randomFlag;
}

bool TraciControlToSumo::getCreateProcessWorked()
{
	return CreateProcessWorked;
}