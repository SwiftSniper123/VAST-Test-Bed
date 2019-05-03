#pragma once
#include "TraCIAPI.h"
#include "Environment.h"

class SumoEnvironment : public Environment
{
public:
	SumoEnvironment(string fileLocation, int AVid, string SUMOexeLocation, int isRandom)
	{
		_fileLocation = fileLocation;
		random = isRandom;
		_AVid = AVid;
		_SUMOexeLocation = SUMOexeLocation;
	}

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

private:
	string _fileLocation;
	int random;
	string _SUMOexeLocation;
	string _AVid;
	TraCIAPI traci;
	dataMap currentData;
};