#include "ProximitySensor.h"

ProximitySensor::ProximitySensor(AV* owningAV, dataMap initialDataMap)
	: Sensor("ProximitySensor", initialDataMap)
{
	if (owningAV == nullptr)
	{
		throw InvalidArgumentException("ProximitySensor cannot be instantiated without an owning AV.");
	}
	if (initialDataMap.empty())
	{
		throw InvalidArgumentException("ProximitySensor cannot be instantiated with an empty datamap of configurations.");
	}
	if (initialDataMap.find(SENSOR_LOC) != initialDataMap.end() ||
		initialDataMap.find(SENSOR_QUAD) != initialDataMap.end() ||
		initialDataMap.find(SENSOR_DEPTH) != initialDataMap.end())
	{
		throw InvalidArgumentException("ProximitySensor cannot scan without the appropriate sensor parameters.");
	}
	_owningAV = owningAV;
	_currentData = initialDataMap;

}

ProximitySensor::~ProximitySensor()
{
	_currentData.clear();
	delete _lastClosestObstacle;
	delete _lastClosestProximity;
}

void ProximitySensor::AVUpdate(timestamp t, dataMap updateDataMap)
{
	// iterate through the incoming map
	for (auto dataIterator = updateDataMap.begin(); dataIterator != updateDataMap.end(); ++dataIterator)
	{
		// if key is found, replace data; if not found, add new pair of key & data
		_currentData.insert(pair<string, VType*>(dataIterator->first, dataIterator->second));
	}

	_lastUpdateTime = t;

	// calculate the distances
	scan();
}

void ProximitySensor::reportToAV()
{
	for (auto dataIterator = _currentData.begin(); dataIterator != _currentData.end(); ++dataIterator)
	{
		_owningAV->sensorReporting(dataIterator->first, dataIterator->second);
	}
}

void ProximitySensor::stopReplication(bool another, string runID)
{
	_currentData = this->getDataMap();

	_lastUpdateTime = -1.0;

	_lastClosestObstacle = nullptr;

	_lastClosestProximity = nullptr;
}

/* Assesses the obstacles and generates the list of obstacles in view, the closest
object, and the closest distance.*/
void ProximitySensor::scan()
{
	// iterate by size count through Array of obs positions and ids, calculate if they are in 
	// the proper quadrant, and then measure distance, set variables, make the call to _owningAV 
	// with reportToAV 
}

//{
//	static const string SENSOR_LOC = "sensor_location";
//	static const string SENSOR_QUAD = "sensor_viewquadrant";
//	static const string SENSOR_DEPTH = "sensor_viewdepth";
//	static const string OBSTACLE_IDS = "obstacle_ids";
//	static const string OBSTACLE_POS = "obstacle_pos";
//	static const string CLOSEST_ID = "closest_obs";
//}