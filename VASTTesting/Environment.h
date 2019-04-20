#pragma once
#include "Obstacle.h"
#include "VComponent.h"

class Environment : public VComponent
{
	friend class EventTree;
public:
	/* Base class constructor. */
	Environment();

	/* Constructor for setting the object data.*/
	Environment(string name, dataMap environmentData);

	/* Base Environment Destructor, clears data map*/
	~Environment();
	
	/* Pure Virtual Function from VComponent.  Called by a component
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

	dataMap _dataMap;
};

