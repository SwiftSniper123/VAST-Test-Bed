#include "SumoEnvironment.h"


void SumoEnvironment::openEnvironment()
{
	string str = "gcc ";

	str = str + _fileLocation;

	const char *command = str.c_str();

	system(command);

	traci.connect("SUMO Connection", 1338);
}

void SumoEnvironment::getMapInformation()
{
	int IDcount = traci.vehicle.getIDCount();

	//gets the location of the event tree to add events
	EventTree *location = getEventTree();

	vector<string> IDlist = traci.vehicle.getIDList();

	for (vector<string>::iterator it = IDlist.begin(); it != IDlist.end(); it++)
	{
		dataMap currentData;
		String vehicleID;
		Vector3 position;

		//checks if the ID is that of the Autonomous vehicle and if so get the information about that
		if (*it == _AVid)
		{
			vehicleID.value = it;
			Double acceleration(traci.vehicle.getAcceleration(*it));
			Double velocity(traci.vehicle.getSpeed(*it));
			libsumo::TraCIPosition XYZ(traci.vehicle.getPosition(*it));

			position.x = XYZ.x;
			position.y = XYZ.y;
			position.z = XYZ.z;

			currentData[*it] = ("Vehicle ID", &vehicleID);
			currentData[*it] = ("Acceleration", &acceleration);
			currentData[*it] = ("Velocity", &velocity);
			currentData[*it] = ("XYZ position", &position);
		}

		//will get the information needed for other vehicles in the system
		else
		{
			vehicleID.value = it;
			Double velocity(traci.vehicle.getSpeed(*it));
			libsumo::TraCIPosition XYZ(traci.vehicle.getPosition(*it));

			position.x = XYZ.x;
			position.y = XYZ.y;
			position.z = XYZ.z;

			currentData[*it] = ("Vehicle ID", &vehicleID);
			currentData[*it] = ("Velocity", &velocity);
			currentData[*it] = ("XYZ position", &position);
		}

		update(location->getCurrentSimTime(), currentData);
	}

}