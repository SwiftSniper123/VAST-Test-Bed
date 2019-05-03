#pragma once
#include "TraCIAPI.h"
#include "Environment.h"
#include "AV.h"

class SumoEnvironment : public Environment
{
public:
	SumoEnvironment(string configFileLocation, string SUMOexeLocation, Integer port, Vector3 bounds)
	{
		_fileLocation = configFileLocation;
		//random = isRandom;
		//_AVid = AVid;
		_SUMOexeLocation = SUMOexeLocation;
		_port = port;
		_bounds = bounds;
	}

	//Opens the Sumo Environment with the file location
	void openEnvironment();

	//Gets the information from Sumo via traci commands
	void getMapInformation();

	//sends the new command to the AV in sumo if this is required
	void changeAVCommand();

	//calls this child classes functions
	dataMap callUpdateFunctions();

	void setSeed(string seed);

	void addAV(AV *AV);

private:
	string _fileLocation;
	int random; //Replace with function for setting the seed?
	string _seed;
	string _SUMOexeLocation;
	Integer _port;
	vector<string> _AVid;
	TraCIAPI traci;
	dataMap currentData;
	Vector3 _bounds;
};