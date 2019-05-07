#include "VC_HEADERS.h"
#include "PythonAV.h"

 

PythonVehicle::PythonVehicle(dataMap AVConfigData, dataMap AVRunData)
{
	_configData = AVConfigData;
	_runData = AVRunData;

	_vehicleID = _configData[AV_NAME]->s_value();
	_AVexeLocation = _configData[AV_EXE_LOCATION]->s_value();
	_port = Integer(_configData[AV_MOVEMENT_PORT]);
	_sensors = Array(_configData[SENSORS]);

	_bounds = Vector3(_runData[AV_BOUNDS]);
	_location = Vector3(_runData[AV_LOCATION]);
	_orientation = Vector3(_runData[AV_ORIENTATION]);


}

void PythonVehicle::update(timestamp t, dataMap dataMap)
{
	// used to get VehicleID and speed here
	currentData[OBSTACLE_IDS] = dataMap[OBSTACLE_IDS]; 
	currentData[OBSTACLE_POS] = dataMap[OBSTACLE_POS];
	currentData["Position"] = dataMap["Position"];

	currentData = callUpdateFunctions();

	this->getEventTree()->addEvent(this, t, currentData);
}

void PythonVehicle::stopReplication(bool another, string runID)
{

}

void PythonVehicle::sendInformation()
{
	_output.open("AVInput.csv");

	_output << "RangeFinder" << "," << "Speedometer" << endl;
	_output << currentData["Range"] << "," << currentData["Speed"] << endl;

	return;
}

void PythonVehicle::retrieveInformation()
{
	string holder;
	string holder2;
	
	string targetVelocityholder;
	string durationholder;
	_input.open("AVOutput.csv");

	getline(_input, holder, ',');
	getline(_input, holder2, ',');
	getline(_input, targetVelocityholder, ',');
	getline(_input, durationholder, ',');

	Integer targetVelocity(stoi(targetVelocityholder));
	Integer duration(stoi(durationholder));
	
	currentData["TargetVelocity"] = ("Target Velocity", &targetVelocity);
	currentData["Duration"] = ("Duration", &duration);

	return;

}

dataMap PythonVehicle::callUpdateFunctions()
{
	sendInformation();
	retrieveInformation();
	return currentData;
}