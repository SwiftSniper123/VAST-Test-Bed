#pragma once
#include "VComponent.h"

class Obstacle : public VComponent
{
public:
	/* Base class constructor.*/
	Obstacle();

	/* Constructor for setting the object data.*/
	Obstacle(string name, dataMap obstacleData);
	
	/* Base class destructor.  Clears data map.*/
	~Obstacle();

	/* Pure Virtual Function from VComponent.  Called by a component 
	external to the Obstacle in order to update data important to the Obstacle.
	time		timestamp for the update
	updateMap	data that changed for this update*/
	void update(timestamp time, dataMap updateMap);

	/*return the type of VComponent as AV*/
	virtual VComponent::VCType getVCType();

	/* Overridden from VComponent.  Returns ID name.*/
	virtual string getName();

	/* Overridden from VComponent.  Returns a copy of the internal data map.*/
	virtual dataMap getDataMap();
private:

	string _name;

	dataMap _dataMap;
};