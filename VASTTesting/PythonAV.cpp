#include "PythonAV.h"

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