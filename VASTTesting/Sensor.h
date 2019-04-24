#pragma once
#include "VComponent.h"


class Sensor : public VComponent
{
public:
	/*default constructor*/
	Sensor();

	/* Constructor for setting the object data.*/
	Sensor(string name, dataMap sensorData);

	/*destructor. Clears data map.*/
	~Sensor();

	/* Inherited Function from VComponent.  Called by a component
	external to the Obstacle in order to update data important to the Obstacle.
	time		timestamp for the update
	updateMap	data that changed for this update*/
	void update(timestamp t, dataMap dataMap);	

	/*return the type of VComponent as AV*/
	virtual VComponent::VCType getVCType();

	/* Overridden from VComponent.  Returns ID name.*/
	virtual string getName();

	/* Overridden from VComponent.  Returns a copy of the internal data map.*/
	virtual dataMap getDataMap();

private:
	
	string _name;

	/*data types of sensor*/
	dataMap _dataMap;

	
};