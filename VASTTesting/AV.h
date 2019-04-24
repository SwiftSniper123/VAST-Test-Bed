#pragma once
#include "VComponent.h"

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

	/*return the type of VComponent as AV*/
	virtual VComponent::VCType getVCType();

	/* Overridden from VComponent.  Returns ID name.*/
	virtual string getName();

	/* Overridden from VComponent.  Returns a copy of the internal data map.*/
	virtual dataMap getDataMap();
private:
	string _name = "AV";

	/*AV map that stores all variable information*/
	dataMap _dataMap;
};

