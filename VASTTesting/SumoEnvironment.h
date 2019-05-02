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

	//sends the new command to the AV in sumo if this is required
	void changeAVCommand();

	//calls this child classes functions
	dataMap callUpdateFunctions();

private:
	string _fileLocation;
	string _AVid;
	TraCIAPI traci;
	dataMap currentData;
};