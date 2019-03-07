//Virtual Autonomous System Testbed
//ODU Capstone 2018-2019

#include "..\h\VComponent.h"
#include <iostream>
#include <map>
#include<variant>
#include "..\h\VType.h"
#include "..\h\AV.h"
#include "..\h\Environment.h"
#include <Windows.h>

using namespace std;

class ExtendedVComponent
{
public:
	ExtendedVComponent(int id)
	{
		this_id = id;
	};

	void update()
	{
		cout << "\nExtendedVComponent" << this_id << " is updating." << endl;  
	};

private:
	int this_id;
};

int main(int argc, char **argv1)
{
	/*create the map
	need to extract information from configuration file
	user will input variable name, type, and intial value*/
	std::map<std::string, AVDataTypes> AVmap; //string is the name
	//declare variables
	AVDataTypes vehicleType("vehicleType","std::string"); //std::string vehicleType;
	AVDataTypes maxVelocity("maxVelocity","float"); // float maxVelocity;
	AVDataTypes currentVelocity("currentVelocity","float"); // float currentVelocity;
	AVDataTypes maxAltitude("maxAltitude","float"); // float maxAltitude;
	AVDataTypes currentAltitude("currentAltitude","float"); // float currentAltitude;
	AVDataTypes currentPosition("currentPosition","vector3"); // vector3 currentPosition;
	AVDataTypes initialPosition("initialPosition","vector3"); // vector3 initialPosition;

	//initialize variables
	//NEED TO CREATE A NUMBER TYPE
	vehicleType.set("car");
	maxVelocity.set(10);
	currentVelocity.set(0);
	maxAltitude.set(100);
	currentAltitude.set(0);
	currentPosition.set(new vector3(0, 0, 0));
	initialPosition.set(new vector3(0, 0, 0));

	//add variables to AV map
	AVmap["vehicleType"] = vehicleType;
	AVmap["maxVelocity"] = maxVelocity;
	AVmap["currentVelocity"] = currentVelocity;
	AVmap["maxAltitude"] = maxAltitude;
	AVmap["currentAltitude"] = currentAltitude;
	AVmap["currentPosition"] = currentPosition;
	AVmap["initialPosition"] = initialPosition;

	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	STARTUPINFO StartupInfo; //This is an [in] parameter
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof StartupInfo; //Only compulsory field

	//Need relative path for config file somehow 
	/*
	int bytes = GetModuleFileName(NULL, pBuf, len);
	if (bytes == 0)
		return -1;
	else
		return bytes; */
	string sumoCfgString;
	cout << "Input path to SUMO config file: ";
	cin >> sumoCfgString;

	//string sumoCmd = "sumo -c " + sumoCfgString + " --remote-port 1337";
	string sumoCmd = "sumo-gui -c " + sumoCfgString + " --remote-port 1337";

	LPSTR cmdArgs = const_cast<char *>(sumoCmd.c_str()); //"sumo -c C:\\Users\\PeterM\\Desktop\\SUMO\\hello.sumocfg --remote-port 1337"

	CreateProcess(NULL, cmdArgs,
		NULL, NULL, FALSE, 0, NULL,
		NULL, &StartupInfo, &ProcessInfo);


	//CreateProcess("cmd.exe", "sumo -c C:\\Users\\PeterM\\Desktop\\SUMO\\hello.sumocfg --remote-port 1337", )
	Client client;
	//client.connect("localhost", 1337);
	//std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	//std::cout << "run 5 steps ...\n";
	//client.simulationStep(5 * 1000);
	//std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	//client.close();

	return 0;
}

/*
void sumoConnection() {
	Client client;
	client.connect("localhost", 1337);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	std::cout << "run 5 steps ...\n";
	client.simulationStep(5 * 1000);
	std::cout << "time in ms: " << client.simulation.getCurrentTime() << "\n";
	client.close();
} */