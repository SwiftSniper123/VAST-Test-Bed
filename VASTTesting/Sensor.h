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
	virtual void update(timestamp t, dataMap dataMap);	

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

protected:
	
	string _name;

	/*data types of sensor*/
	dataMap _dataMap;

	
};