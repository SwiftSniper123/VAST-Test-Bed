#include "ProximitySensor.h"

// private functions-------------------------------------------------------------//
int ProximitySensor::calculateQuadrant(Vector3* vec)
{
	int result = 0;
	if (vec->x() > 0)
	{
		if (vec->y() > 0)
		{
			result = 1;
		}
		else
		{
			result = 4;
		}
	}
	else
	{
		if (vec->y() > 0)
		{
			result = 2;
		}
		else
		{
			result = 3;
		}
	}
	return result;
}

distance* ProximitySensor::findDistance(Vector3* localVec)
{
	Vector3* sensorVec = new Vector3(_currentData.at(SENSOR_LOC));
	// square root of( (x2 - x1)^2 + (y2 - y1)^2 )
	return new Double(sqrt(pow(localVec->x() - sensorVec->x(), 2) + pow(localVec->y() - sensorVec->y(), 2)));
}

// public functions-------------------------------------------------------------//
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
	if (initialDataMap.find(SENSOR_LOC) == initialDataMap.end() ||
		initialDataMap.find(SENSOR_QUAD) == initialDataMap.end() ||
		initialDataMap.find(SENSOR_DEPTH) == initialDataMap.end())
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
	_owningAV->sensorReporting(CLOSEST_ID, this->getClosestObstacle()->first);
	_owningAV->sensorReporting(CLOSEST_POS, this->getClosestObstacle()->second);
	_owningAV->sensorReporting(CLOSEST_DIST, this->getClosestProximity());
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
	// iterate by size count through Array of obs positions and ids, 
	if (_currentData.find(OBSTACLE_IDS) != _currentData.end() &&
		_currentData.find(OBSTACLE_POS) != _currentData.end() &&
		_currentData.at(OBSTACLE_IDS)->isA(ARRAY_TYPE) &&
		_currentData.at(OBSTACLE_POS)->isA(ARRAY_TYPE) &&
		((new Array(_currentData.at(OBSTACLE_IDS)))->arraySize() ==
		(new Array(_currentData.at(OBSTACLE_POS)))->arraySize()))
	{
		Array* obs_ids = new Array(_currentData.at(OBSTACLE_IDS));
		Array* obs_poss = new Array(_currentData.at(OBSTACLE_POS));
		Vector3* inspectedObstacleLocalPos;
		Double* inspectedObstacleLocalDist;
		int inspectedObstacleQuad = 0;
		int sensorQuad = 0;
		Double* sensorDepthRange = new Double(_currentData.at(SENSOR_DEPTH));
		for (int i = 0; i < obs_ids->arraySize(); i++)
		{
			//calculate if they are in the proper quadrant,
			inspectedObstacleLocalPos = *(obs_poss->at_Vector3(i)) - Vector3(_currentData.at(SENSOR_LOC));
			inspectedObstacleQuad = calculateQuadrant(inspectedObstacleLocalPos);
			sensorQuad = (new Integer(_currentData.at(SENSOR_QUAD)))->value();
			if (sensorQuad == inspectedObstacleQuad)
			{
				//and then measure distance,
				Double* shortestDistance = _lastClosestProximity == nullptr ? new Double(_currentData.at(SENSOR_DEPTH)) : _lastClosestProximity;
				inspectedObstacleLocalDist = findDistance(inspectedObstacleLocalPos);
				
				//set variables,
				if (inspectedObstacleLocalDist < sensorDepthRange && 
					inspectedObstacleLocalDist < shortestDistance)
				{
					_lastClosestProximity = inspectedObstacleLocalDist;
					_lastClosestObstacle = new pair<String*, Vector3*>(obs_ids->at_String(i), obs_poss->at_Vector3(i));

					//make the call to _owningAV with reportToAV
					reportToAV();
				}
			}
		}
	}	 
}

Array* ProximitySensor::getObstaclesList()
{
	Array* result = nullptr;
	if (_currentData.find(OBSTACLE_IDS) != _currentData.end() && 
		_currentData.at(OBSTACLE_IDS)->isA(ARRAY_TYPE))
	{
		result = new Array(_currentData.at(OBSTACLE_IDS));
	}
	// if OBSTACLE_IDS was not found in the data, ignore this and return nullptr
	return result;
}

pair<String*, Vector3*>* ProximitySensor::getClosestObstacle()
{
	return _lastClosestObstacle;
}

Double* ProximitySensor::getClosestProximity()
{
	return _lastClosestProximity;
}
