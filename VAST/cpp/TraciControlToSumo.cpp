#include "..\h\TraciControlToSumo.h"

void TraciControlToSumo::initialize()
{
	random = false;
	/*Initializes StartupInfo, sumoCfgString, sumoCmd, and cmdArgs*/

	ZeroMemory(&StartupInfo, sizeof(StartupInfo)); //Fills StartupInfo with zeros
	StartupInfo.cb = sizeof StartupInfo; //Only parameter of StartupInfo that needs to be configured

	std::cout << "Enter 0 for normal mode, 1 for random mode: ";
	std::cin >> random;

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

	cmdArgs = const_cast<char *>(sumoCmd.c_str());
}

void TraciControlToSumo::runSumo()
{
	//Spawns a process and it's own separate thread
	CreateProcess(NULL, cmdArgs,
		NULL, NULL, FALSE, 0, NULL,
		NULL, &StartupInfo, &ProcessInfo);

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