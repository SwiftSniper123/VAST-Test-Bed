#pragma once
#include "VComponent.h"
#include "VType.h"
#include "EventTree.h"

/*base class for all metrics in VAST*/
class ScenarioMetric : public VComponent
{
public:
	/* default constructor*/
	ScenarioMetric() {};

	/* Constructor to set component data.*/
	ScenarioMetric(string name, dataMap metricData) 
	{
		_myMap = metricData;
	};

	/* returns the statistical metric value as defined by the extending metric class.*/
	virtual void calculate() {};

	/* The inherited update function*/
	virtual void update(timestamp t, dataMap dataMap)
	{
		EventTree *temp = getEventTree();

		_myMap["Acceleration"] = dataMap["Acceleration"];
		_myMap["Velocity"] = dataMap["Speed"];
		_myMap["Position"] = dataMap["Position"];

		calculate();

		temp->addEvent(this, t, databaseMap);
	};

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
		return _myMap;
	}

	//holds the metric information that is needed
	dataMap _myMap;
	dataMap databaseMap;
};
