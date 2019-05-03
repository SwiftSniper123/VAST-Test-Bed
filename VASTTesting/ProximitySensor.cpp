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
	Vector3* sensorVec =((Vector3*)_currentData.at(SENSOR_LOC));
	// square root of( (x2 - x1)^2 + (y2 - y1)^2 )
	double x2minusx1 = localVec->x() - sensorVec->x();
	double y2minusy1 = localVec->y() - sensorVec->y();
	double xtermsquared = pow(x2minusx1, 2);
	double ytermsquared = pow(y2minusy1, 2);
	return new Double(sqrt(xtermsquared + ytermsquared));
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
	// if local values are not nullptr, a scan was accomplished
	if (this->getClosestObstacle() != nullptr)
	{
		// if all local values are filled, report
		if (this->getClosestProximity() != nullptr)
		{
			_owningAV->sensorReporting(CLOSEST_ID, this->getClosestObstacle()->first);
			_owningAV->sensorReporting(CLOSEST_POS, this->getClosestObstacle()->second);
			_owningAV->sensorReporting(CLOSEST_DIST, this->getClosestProximity());
		}
		else // otherwise there was weird data received
		{
			throw UnpredictableBehaviorException(
				"Proximity Sensor cannot transmit.  The problem is likely to be \
				 the type of input ProximitySensor received from the AV map.");
		}
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
	// iterate by size count through Array of obs positions and ids, 
	if (_currentData.find(OBSTACLE_IDS) != _currentData.end() &&
		_currentData.find(OBSTACLE_POS) != _currentData.end() &&
		_currentData.at(OBSTACLE_IDS)->isA(ARRAY_TYPE) &&
		_currentData.at(OBSTACLE_POS)->isA(ARRAY_TYPE) &&
		(((Array*)_currentData.at(OBSTACLE_IDS))->arraySize() ==
		((Array*)_currentData.at(OBSTACLE_POS))->arraySize()))
	{
		Array* obs_ids = ((Array*)_currentData.at(OBSTACLE_IDS));
		Array* obs_poss = ((Array*)_currentData.at(OBSTACLE_POS));
		Vector3* inspectedObstacleLocalPos;
		Double* inspectedObstacleLocalDist;
		int inspectedObstacleQuad = 0;
		int sensorQuad = 0;
		Double* sensorDepthRange = ((Double*)_currentData.at(SENSOR_DEPTH));
		for (int i = 0; i < obs_ids->arraySize(); i++)
		{
			//calculate if they are in the proper quadrant,
			Vector3* thisPos = ((Vector3*)_currentData.at(SENSOR_LOC));
			Vector3* thatPos = obs_poss->at_Vector3(i);
			inspectedObstacleLocalPos = *thatPos - thisPos;
			inspectedObstacleQuad = calculateQuadrant(inspectedObstacleLocalPos);
			sensorQuad = ((Integer*)_currentData.at(SENSOR_QUAD))->value();
			if (sensorQuad == 0 || inspectedObstacleQuad == 0)
			{
				continue;
			}
			else if (sensorQuad == inspectedObstacleQuad)
			{
				//and then measure distance,
				Double* shortestDistance = _lastClosestProximity == nullptr ? new Double(_currentData.at(SENSOR_DEPTH)) : _lastClosestProximity;
				inspectedObstacleLocalDist = findDistance(inspectedObstacleLocalPos);
				
				//set variables,
				if (inspectedObstacleLocalDist->value() < sensorDepthRange->value() && 
					inspectedObstacleLocalDist->value() < shortestDistance->value())
				{
					_lastClosestProximity = inspectedObstacleLocalDist;
					_lastClosestObstacle = new pair<String*, Vector3*>(obs_ids->at_String(i), obs_poss->at_Vector3(i));
				}
			}
		}
		//make the call to _owningAV with reportToAV
		reportToAV();
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
