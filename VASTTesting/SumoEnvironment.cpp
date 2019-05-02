#include "SumoEnvironment.h"


void SumoEnvironment::openEnvironment()
{
	string str = "gcc ";

	str = str + _fileLocation;

	const char *command = str.c_str();

	system(command);

	traci.connect("SUMO Connection", 1338);

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

	for (vector<string>::iterator it = IDlist.begin(); it != IDlist.end(); it++)
	{
		//checks if the ID is that of the Autonomous vehicle and if so get the information about that
		if (*it == _AVid)
		{
			vehicleID[i].value = it;
			acceleration = traci.vehicle.getAcceleration(*it);

			velocity[i].value = traci.vehicle.getSpeed(*it);
			libsumo::TraCIPosition XYZ(traci.vehicle.getPosition(*it));

			position[i].x = XYZ.x;
			position[i].y = XYZ.y;
			position[i].z = XYZ.z;
		}

		//will get the information needed for other vehicles in the system
		else
		{
			vehicleID[i].value = it;
			velocity[i].value = traci.vehicle.getSpeed(*it);
			libsumo::TraCIPosition XYZ(traci.vehicle.getPosition(*it));

			position[i].x = XYZ.x;
			position[i].y = XYZ.y;
			position[i].z = XYZ.z;
		}

		i++;
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
	dataMap currentMap = getDataMap();
	String one("1");
	String zero("0");

	double speed, duration;

	speed = stod(currentMap["TargetVelocity"]->s_value());
	duration = stod(currentMap["Duration"]->s_value());

	if (currentMap["AVLogic"] == &one)
	{
		traci.vehicle.slowDown(_AVid, speed, duration);
		return;
	}
	if(currentMap["AVLogic"] == &zero)
	{
		return;
	}
}

dataMap SumoEnvironment::callUpdateFunctions()
{
	changeAVCommand();
	getMapInformation();
	return currentData;
}