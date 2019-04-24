#include "Environment.h"

Environment::Environment()
{

}

Environment::Environment(string name, dataMap environmentData)
{
	_name = name;
	_dataMap = environmentData;
}

Environment::~Environment()
{
	_dataMap.clear();
}

void Environment::update(timestamp t, dataMap dataMap)
{
	EventTree *temp = getEventTree();

	temp->addEvent(this, t, dataMap);
}

VComponent::VCType Environment::getVCType()
{
	return Environment_Avatar;
}

string Environment::getName()
{
	return _name + "Environment";
}

dataMap Environment::getDataMap()
{
	return _dataMap;
}

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
		Integer vehicleID;
		Vector3 position;
		
		//checks if the ID is that of the Autonomous vehicle and if so get the information about that
		if (IDlist.data == _AVid)
		{
			vehicleID = IDlist.data;
			Double acceleration(traci.vehicle.getAcceleration(IDlist.data));
			Double velocity(traci.vehicle.getSpeed(IDlist.data));
			libsumo::TraCIPosition XYZ(traci.vehicle.getPosition(IDlist.data));

			position.x = XYZ.x;
			position.y = XYZ.y;
			position.z = XYZ.z;

			currentData[IDlist.data] = ("Vehicle ID", vehicleID );
			currentData[IDlist.data] = ("Acceleration", acceleration);
			currentData[IDlist.data] = ("Velocity", velocity);
			currentData[IDlist.data] = ("XYZ position", position);
		}

		//will get the information needed for other vehicles in the system
		else
		{
			vehicleID = IDlist.data;
			Double velocity(traci.vehicle.getSpeed(IDlist.data));
			libsumo::TraCIPosition XYZ(traci.vehicle.getPosition(IDlist.data));

			position.x = XYZ.x;
			position.y = XYZ.y;
			position.z = XYZ.z;

			currentData[IDlist.data] = ("Vehicle ID", vehicleID);
			currentData[IDlist.data] = ("Velocity", velocity);
			currentData[IDlist.data] = ("XYZ position", position);
		}

		update(location->getCurrentSimTime(), currentData);
	}

}

