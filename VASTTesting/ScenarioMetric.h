#pragma once
#include "VComponent.h"

static const string ACCELERATION = "Acceleration";
static const string MIN_ACCELERATION = "MinAcceleration";
static const string AVG_ACCELERATION = "AvgAcceleration";
static const string MAX_ACCELERATION = "MaxAcceleration";
static const string AVG_DECELERATION = "AvgDeceleration";
static const string AVG_SPEED = "AvgSpeed";
static const string SPEED = "Speed";
static const string POSITION = "Position";

using std::string;

/*base class for all metrics in VAST*/
class ScenarioMetric : public VComponent
{
public:
	/* default constructor*/
	ScenarioMetric();

	/* Constructor to set component data.  This constructor guarantees that all keys are 
	present in the datamap as initialized zero values.*/
	ScenarioMetric(string name, dataMap metricData);

	/* returns the statistical metric value as defined by the extending metric class.*/
	virtual void calculate();

	/* The inherited update function*/
	void update(timestamp t, dataMap dataMap);

	/* Informs the component that the replication is coming to an end.  This function
	is called by the EventTree to give the component the opportunity to reset data, or
	processes that must be refreshed in a new replication.
	bool	another		A signal that there will be another replication
						after this replication stops.
	string	runID		The id number used for this replication.*/
	virtual void stopReplication(bool another, string runID);

	/* Overridden from VComponent.  Returns ID name.*/
	virtual string getName()
	{
		return "Scenario Metric";
	};

	/* Overridden from VComponent.  Returns a copy of the internal data map.*/
	virtual dataMap getDataMap()
	{
		// this is an empty map, the default return from ScenarioMetric
		return _initialMap;
	}

	//holds the metric information that is needed
	dataMap _myMap;
	dataMap _initialMap;
};
