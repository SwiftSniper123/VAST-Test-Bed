#pragma once
#include "TraCIAPI.h"
#include "VC_HEADERS.h"
#include <Windows.h>
#include <cstdlib>

class SumoEnvironment : public Environment
{
public:
	SumoEnvironment(dataMap envConfigData, dataMap envRunData);

	SumoEnvironment(string configFileLocation, string SUMOexeLocation, Integer port, Vector3 bounds);

	/* Inherited Function from VComponent.  Called by a component
	external to the Obstacle in order to update data important to the Obstacle.
	time		timestamp for the update
	updateMap	data that changed for this update*/
	virtual void update(timestamp t, dataMap dataMap);

	//Opens the Sumo Environment with the file location
	void openEnvironment();

	//Gets the information from Sumo via traci commands
	void getMapInformation();

	//sends the new command to the AV in sumo if this is required
	void changeAVCommand();

	//called by the update function to run all of the functions of the child classes
	virtual dataMap callUpdateFunctions();

	void stopReplication(bool another, string runID);

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
	dataMap _runData;
	dataMap _configData;
	Vector3 _bounds;

	LPWSTR cmdArgs;
	PROCESS_INFORMATION ProcessInfo; //This is what we get as an [out] parameter
	STARTUPINFO StartupInfo; //This is an [in] parameter
};