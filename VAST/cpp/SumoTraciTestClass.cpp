#include "..\h\SumoTraciTestClass.h"

void SumoTraciTestClass::initialize()
{
	//Add content here later
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof StartupInfo;

	std::cout << "Input path to SUMO config file: ";
	std::cin >> sumoCfgString;

	sumoCmd = "sumo -c " + sumoCfgString + " --remote-port 1337";

	cmdArgs = const_cast<char *>(sumoCmd.c_str());
}

void SumoTraciTestClass::runSumo()
{
	//Add content here later
	CreateProcess(NULL, cmdArgs,
		NULL, NULL, FALSE, 0, NULL,
		NULL, &StartupInfo, &ProcessInfo);

	Sleep(2500); //SUMO needs to start entirely before runClient() is called. This assures there's enough time for SUMO to boot up.
}

void SumoTraciTestClass::runClient()
{
	//Add content here later
	client.connect("localhost", 1337);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	std::cout << "run 5000 steps ...\n";
	client.simulationStep(5 * 1000); //One step in simulation time
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	client.close();
}

void SumoTraciRandomTestClass::initialize()
{
	//Add content here later
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof StartupInfo;

	std::cout << "Input path to SUMO config file: ";
	std::cin >> sumoCfgString;
	srand(time(NULL));

	seed = rand() % 10000;

	sumoCmd = "sumo -c " + sumoCfgString + " --remote-port 1337" + " --seed " + std::to_string(seed);

	cmdArgs = const_cast<char *>(sumoCmd.c_str());
}

void SumoTraciRandomTestClass::runClient()
{
	//Add content here later
	client.connect("localhost", 1337);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	std::cout << "run 5 steps ...\n";
	client.simulationStep(5 * 1000);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	std::cout << "Command line: " << sumoCmd << std::endl;
	client.close();
	system("PAUSE");
}