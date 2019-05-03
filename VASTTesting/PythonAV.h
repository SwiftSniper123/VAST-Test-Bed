#pragma once
#include "AV.h"
#include <iostream>
#include <fstream>

using namespace std;

class PythonVehicle : public AV
{
public:
	PythonVehicle(dataMap AVConfigData, dataMap AVRunData)
	{
		_configData = AVConfigData;
		_runData = AVRunData;

		_vehicleID = _configData["av_name"]->s_value();
		_AVexeLocation = _configData["exe_location"]->s_value();
		_port = Integer(_configData["av_movement_port"]);
		_sensors = Array(_configData["sensors"]);

		_bounds = Vector3(_runData["AV_bounds"]);
		_location = Vector3(_runData["AV_location"]);
		_orientation = Vector3(_runData["AV_orientation"]);
		

	}

	void update(timestamp t, dataMap dataMap);

	void retrieveInformation();

	void sendInformation();

	//calls this child classes functions
	dataMap callUpdateFunctions();

	void stopReplication(bool another, string runID);

private:
	ofstream _output;
	ifstream _input;

	string _AVexeLocation;
	Integer _port;
	Vector3 _bounds;
	Vector3 _location;
	Vector3 _orientation;
	Array _sensors;

	string _vehicleID;
	dataMap currentData;
	dataMap _runData;
	dataMap _configData;
};