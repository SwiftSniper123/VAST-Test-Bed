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

	/* Cooperates with internal Sensor component to update locally rather than use
	the EventTree and update system of communication.*/
	virtual void sensorReporting(string key, VType* data);

protected:
	string _name = "AV";

	/*AV map that stores all variable information*/
	dataMap _dataMap;
};

