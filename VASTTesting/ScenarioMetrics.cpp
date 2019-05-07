#include "VC_HEADERS.h"

ScenarioMetric::ScenarioMetric()
{}

ScenarioMetric::ScenarioMetric(string name, dataMap metricData)
{
	Array* avListInDataMap = ((Array*)metricData[AV_LIST]);
	Array* obsIDsMap = ((Array*)metricData[OBSTACLE_IDS]);
	Array* accMap = ((Array*)metricData[OBSTACLE_ACC]);
	Array* speedMap = ((Array*)metricData[OBSTACLE_VEL]);
	Array* posMap = ((Array*)metricData[OBSTACLE_POS]);

	numAVs = avListInDataMap->arraySize();

	for (int j = 0; j < avListInDataMap->arraySize(); j++)
	{
		for (int n = 0; n < obsIDsMap->arraySize(); n++)
		{
			if (avListInDataMap->at_String(j)->value() == obsIDsMap->at_String(n)->value())
			{
				//found the matching id
				// add to initial map
				_initialMap[n].emplace(ACCELERATION, accMap->at_Double(n));
				_initialMap[n].emplace(MIN_ACCELERATION, new Double());
				_initialMap[n].emplace(AVG_ACCELERATION, new Double());
				_initialMap[n].emplace(MAX_ACCELERATION, new Double());
				_initialMap[n].emplace(AVG_DECELERATION, new Double());
				_initialMap[n].emplace(AVG_SPEED, new Double());
				_initialMap[n].emplace(SPEED, speedMap->at_Double(n));
				_initialMap[n].emplace(POSITION, posMap->at_Double(n));

				// add to current map
				_myMap[n].emplace(ACCELERATION, accMap->at_Double(n));
				_myMap[n].emplace(MIN_ACCELERATION, new Double());
				_myMap[n].emplace(AVG_ACCELERATION, new Double());
				_myMap[n].emplace(MAX_ACCELERATION, new Double());
				_myMap[n].emplace(AVG_DECELERATION, new Double());
				_myMap[n].emplace(AVG_SPEED, new Double());
				_myMap[n].emplace(SPEED, speedMap->at_Double(n));
				_myMap[n].emplace(POSITION, posMap->at_Double(n));
				break;
			}
		}
	}

	//get rid of dangling pointers
	delete avListInDataMap;
	delete obsIDsMap;
	delete accMap;
	delete speedMap;
	delete posMap;
}

void ScenarioMetric::stopReplication(bool another, string runID)
{
	//add publish events to the event tree
	for (int n = 0; n < numAVs; n++)
		getEventTree()->addEvent(this, 0, _myMap[n]);

	// reset the current map with the initial map
	for (int n = 0; n < numAVs; n++)
		_myMap[n].clear();
}

/*
	holds the X Y and Z coordinates for the vehicle
*/
// Not using this metric, emf 5/3/2019
//class XYZCoordinates : public ScenarioMetric
//{
//public:
//
//	XYZCoordinates() { coordinates = new Vector3(0, 0, 0); };
//
//	void calculate()
//	{
//		coordinates = ((Vector3*)_myMap[POSITION]);
//
//		_initialMap[POSITION] = coordinates;
//	}
//
//	VCType getVCType()
//	{
//		return VCType::ScenarioMetric;
//	}
//
//private:
//	Vector3* coordinates;
//};

void ScenarioMetric::update(timestamp t, dataMap updateMap)
{
	Array* avListInDataMap = new Array(updateMap[AV_LIST]);
	Array* obsIDsMap = new Array(updateMap[OBSTACLE_IDS]);
	Array* accMap = new Array(updateMap[OBSTACLE_ACC]);
	Array* speedMap = new Array(updateMap[OBSTACLE_VEL]);
	Array* posMap = new Array(updateMap[OBSTACLE_POS]);

	for (int j = 0; j < avListInDataMap->arraySize(); j++)
	{
		for (int n = 0; n < obsIDsMap->arraySize(); n++)
		{
			if (avListInDataMap->at_String(j)->value() == obsIDsMap->at_String(n)->value())
			{
				//found the matching id
				_myMap[j][ACCELERATION] = accMap->at_Double(n);
				_myMap[j][SPEED] = speedMap->at_Double(n);
				_myMap[j][POSITION] = posMap->at_Double(n);
				break;
			}
		}
	}

	this->calculate();

	delete avListInDataMap;
	delete obsIDsMap;
	delete accMap;
	delete speedMap;
	delete posMap;
}

/*void ScenarioMetric::calculate()
{
	AverageSpeed avgSp;
	AverageAcceleration avgAccel;
	MaximumAcceleration max;
	MinimumAcceleration min;
	AverageDeacceleration avgDecel;

	avgSp.calculate();
	avgAccel.calculate();
	max.calculate();
	min.calculate();
	avgDecel.calculate();
}*/

/*
	calculates the average speed of the chosen vehicle
*/
class AverageSpeed : public ScenarioMetric
{
public:
	//default constructor
	AverageSpeed(string name, dataMap metricData) : ScenarioMetric(name, metricData) { };

	void calculate()
	{
		for (int n = 0; n < numAVs; n++)
		{
			*sumAvgSpeed[n] += ((Double*)_myMap[n][SPEED])->value();
			countSpeedUpdates++;
			_myMap[n][AVG_SPEED] = new Double(sumAvgSpeed[n]->value() / countSpeedUpdates);
		}
	}

	virtual string getName()
	{
		return "Average Speed";
	};

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	vector<Double*> sumAvgSpeed;
	float countSpeedUpdates = 0;
};

/*
	calculates the Maximum Acceleration for the vehicle
*/
class MaximumAcceleration : public ScenarioMetric
{
public:
	//default constructor
	MaximumAcceleration(string name, dataMap metricData) : ScenarioMetric(name, metricData) { };

	void calculate()
	{
		for (int n = 0; n < numAVs; n++)
		{
			if (currentMax[n]->value() < ((Double*)_myMap[n][ACCELERATION])->value())
			{
				currentMax[n]->value(stod(_myMap[n][ACCELERATION]->s_value()));
				_myMap[n][MAX_ACCELERATION] = currentMax[n];
			}
		}
	}

	virtual string getName()
	{
		return "Maximum Acceleration";
	};

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	vector<Double*> currentMax;
};

/*
	calculates the Maximum Acceleration for the vehicle
*/
class MinimumAcceleration : public ScenarioMetric
{
public:
	//default constructor
	MinimumAcceleration(string name, dataMap metricData) : ScenarioMetric(name, metricData)
	{
		for (int n = 0; n < numAVs; n++)
			currentMin[n] = new Double(1000000.0);
	};

	void calculate()
	{
		for (int n = 0; n < numAVs; n++)
		{
			if (currentMin[n]->value() > ((Double*)_myMap[n][ACCELERATION])->value())
			{
				currentMin[n]->value(((Double*)_myMap[n][ACCELERATION])->value());
				_myMap[n][MIN_ACCELERATION] = currentMin[n];
			}
		}
	}

	virtual string getName()
	{
		return "Minimum Acceleration";
	};

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	//set to some unreachable value()
	vector<Double*> currentMin;
};

/*
	calculates the Average Acceleration for the vehicle
*/
class AverageAcceleration : public ScenarioMetric
{
public:
	//default constructor
	AverageAcceleration(string name, dataMap metricData) : ScenarioMetric(name, metricData)
	{
		for (int n = 0; n < numAVs; n++)
			sumAcceleration[n] = new Double();
	};

	void calculate()
	{
		for (int n = 0; n < numAVs; n++)
		{
			if (((Double*)_myMap[n][ACCELERATION])->value() > 0)
			{
				*sumAcceleration[n] += ((Double*)_myMap[n][ACCELERATION])->value();
				countAccelUpdates++;
				_initialMap[n][AVG_ACCELERATION] = new Double(sumAcceleration[n]->value() / countAccelUpdates);
			}
		}
	}

	virtual string getName()
	{
		return "Average Acceleration";
	};

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	vector<Double*> sumAcceleration;

	float countAccelUpdates = 0;
};

/*
	calculates the Average Acceleration for the vehicle
*/
class AverageDeacceleration : public ScenarioMetric
{
public:
	//default constructor
	AverageDeacceleration(string name, dataMap metricData) : ScenarioMetric(name, metricData)
	{
		for (int n = 0; n < numAVs; n++)
			sumDeacceleration[n] = new Double();
	};

	void calculate()
	{
		for (int n = 0; n < numAVs; n++)
		{
			if (((Double*)_myMap[n][ACCELERATION])->value() < 0)
			{
				*sumDeacceleration[n] += ((Double*)_myMap[n][ACCELERATION])->value();
				countDecelUpdates++;
				_initialMap[n][AVG_DECELERATION] = new Double(sumDeacceleration[n]->value() / countDecelUpdates);
			}
		}
	}

	virtual string getName()
	{
		return "Average Deacceleration";
	};

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	vector<Double*> sumDeacceleration;

	float countDecelUpdates = 0;
};
