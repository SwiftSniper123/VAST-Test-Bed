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

	/* Inherited Function from VComponent.  Called by a component 
	external to the Obstacle in order to update data important to the Obstacle.
	time		timestamp for the update
	updateMap	data that changed for this update*/
	void update(timestamp time, dataMap updateMap);

	/* Informs the component that the replication is coming to an end.  This function
	is called by the EventTree to give the component the opportunity to reset data, or
	processes that must be refreshed in a new replication.
	bool	another		A signal that there will be another replication
						after this replication stops.
	string	runID		The id number used for this replication.*/
	virtual void stopReplication(bool another, string runID);

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