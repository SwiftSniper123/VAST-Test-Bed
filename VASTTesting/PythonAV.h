#pragma once
#include "AV.h"
#include <iostream>
#include <fstream>

using namespace std;

class PythonVehicle : public AV
{
public:
	PythonVehicle(string vehicleID)
	{
		_vehicleID = vehicleID;
	}

	void retrieveInformation();

	void sendInformation();

	//calls this child classes functions
	dataMap callUpdateFunctions();


private:
	ofstream _output;
	ifstream _input;

	string _vehicleID;
	dataMap currentData;
};