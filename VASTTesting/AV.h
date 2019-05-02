#pragma once
#include "VComponent.h"
#include <iostream>
#include <fstream>

using namespace std;

class AV : public VComponent
{
public:
	/*default constructor*/
	AV();

	/* Constructor for setting the object data.*/
	AV(string name, dataMap avData);

	/*Base AV class destructor.  Clears data map.*/
	~AV();

	/* Inherited Function from VComponent.  Called by a component
	external to the Obstacle in order to update data important to the Obstacle.
	time		timestamp for the update
	updateMap	data that changed for this update*/
	void update(timestamp t, dataMap dataMap);

	//called by the update function to run all of the functions of the child classes
	virtual dataMap callUpdateFunctions();

	/*return the type of VComponent as AV*/
	virtual VComponent::VCType getVCType();

	/* Overridden from VComponent.  Returns ID name.*/
	virtual string getName();

	/* Overridden from VComponent.  Returns a copy of the internal data map.*/
	virtual dataMap getDataMap();
	
	/* Gets the data the sensor sends over */
	virtual dataMap getSensorDataMap();

private:
	string _name = "AV";

	/*AV map that stores all variable information*/
	dataMap _dataMap;

	/*AV map that stores the sensor data*/
	dataMap _sensorDataMap;
};


