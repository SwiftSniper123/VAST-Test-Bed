#pragma once
#include "AV.h"
#include <iostream>
#include <fstream>

using namespace std;

class PythonVehicle : public AV
{
public:
	PythonVehicle(dataMap AVConfigData, dataMap AVRunData);

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