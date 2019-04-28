#pragma once
#include "TraCIAPI.h"
#include "Environment.h"

class SumoEnvironment : public Environment
{
public:
	SumoEnvironment(string fileLocation, int AVid)
	{
		_fileLocation = fileLocation;
		_AVid = AVid;
	}

	//Opens the Sumo Environment with the file location
	void openEnvironment();

	//Gets the information from Sumo via traci commands
	void getMapInformation();

private:
	string _fileLocation;
	string _AVid;
	TraCIAPI traci;
};