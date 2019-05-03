#include "VC_HEADERS.h"
#include "SumoEnvironment.h"
#include <Windows.h>

void SumoEnvironment::update(timestamp t, dataMap dataMap)
{
	currentData["Duration"] = dataMap["Duration"];
	currentData["TargetVelocity"] = dataMap["TargetVelocity"];

	currentData = callUpdateFunctions();

	this->getEventTree()->addEvent(this, t, currentData);
}

void SumoEnvironment::openEnvironment()
{
	string str = _SUMOexeLocation;

	ZeroMemory(&StartupInfo, sizeof(StartupInfo)); //Fills StartupInfo with zeros
	StartupInfo.cb = sizeof StartupInfo; //Only parameter of StartupInfo that needs to be configured

	if (_seed == "r" || _seed == "R")
	{
		srand(time(NULL)); //Provides a seed for the random number generator

		int seed = rand() % 10000; //Generates a random seed for Sumo

		str = "\"" + _SUMOexeLocation + "\" -c " + _fileLocation + " --remote-port 1337" + " --seed " + std::to_string(seed);

	}

	else if (!_seed.empty())
	{
		str = "\"" + _SUMOexeLocation + "\" -c " + _fileLocation + " --remote-port 1337" + " --seed " + _seed;
	}

	else
		str = "\"" + _SUMOexeLocation + "\" -c " + _fileLocation + " --remote-port 1337 --full-output localhost:1338";

	LPWSTR cmdArgs = const_cast<char *>(str.c_str());

	CreateProcess(NULL, cmdArgs,
		NULL, NULL, FALSE, 0, NULL,
		NULL, &StartupInfo, &ProcessInfo);

	system(command);

	std::this_thread::sleep_for(std::chrono::milliseconds(50000));

	traci.connect("localhost", 1337);
	std::cout << "TEST";
	this->getEventTree()->start();

	return;
}

void SumoEnvironment::getMapInformation()
{
	int IDcount = traci.vehicle.getIDCount();

	vector<string> IDlist = traci.vehicle.getIDList();

	String vehicleID[100];
	Vector3 position[100];
	Double velocity[100];
	Double acceleration;

	int i = 0;
	for (vector<string>::iterator av = _AVid.begin(); av != _AVid.end(); av++)
	{
		for (vector<string>::iterator it = IDlist.begin(); it != IDlist.end(); it++)
		{
			//checks if the ID is that of an Autonomous vehicle and if so get the information about that

			if (*it == *av)
			{
				vehicleID[i].value(*it);
				acceleration = traci.vehicle.getAcceleration(*it);

				velocity[i].value(traci.vehicle.getSpeed(*it));
				libsumo::TraCIPosition XYZ(traci.vehicle.getPosition(*it));

				position[i].value(XYZ.x, XYZ.y, XYZ.z);
			}

			//will get the information needed for other vehicles in the system
			else
			{
				vehicleID[i].value(*it);
				velocity[i].value(traci.vehicle.getSpeed(*it));
				libsumo::TraCIPosition XYZ(traci.vehicle.getPosition(*it));

				position[i].value(XYZ.x, XYZ.y, XYZ.z);
			}
			i++;
		}
	}

	Array id(vehicleID);
	Array speed(vehicleID);
	Array pos(vehicleID);

	currentData["VehicleID"] = &id;
	currentData["Speed"] = &speed;
	currentData["Acceleration"] = &acceleration;
	currentData["Position"] = &pos;

	return;
}

void SumoEnvironment::changeAVCommand()
{
	String one("1");
	String zero("0");

	double speed, duration;

	speed = stod(currentData["TargetVelocity"]->s_value());
	duration = stod(currentData["Duration"]->s_value());

	for (vector<string>::iterator av = _AVid.begin(); av != _AVid.end(); av++)
	{
		if (currentData["AVLogic"] == &one)
		{
			traci.vehicle.slowDown(*av, speed, duration);
			return;
		}
		if (currentData["AVLogic"] == &zero)
		{
			return;
		}
	}
}

void SumoEnvironment::setSeed(string seed)
{
	_seed = seed;
}

void SumoEnvironment::addAV(AV *AV)
{
	_AVid.push_back(AV->getName());
}

dataMap SumoEnvironment::callUpdateFunctions()
{
	changeAVCommand();
	getMapInformation();
	return currentData;
}

void SumoEnvironment::stopReplication(bool another, string runID)
{

}