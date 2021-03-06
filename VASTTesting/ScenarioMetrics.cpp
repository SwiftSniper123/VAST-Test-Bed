#include "VC_HEADERS.h"

ScenarioMetric::ScenarioMetric()
{}

ScenarioMetric::ScenarioMetric(string name, dataMap metricData)
{
	/*Array* avListInDataMap = ((Array*)metricData[AV_LIST]);
	Array* obsIDsMap = ((Array*)metricData[OBSTACLE_IDS]);
	Array* accMap = ((Array*)metricData[OBSTACLE_ACC]);
	Array* speedMap = ((Array*)metricData[OBSTACLE_VEL]);
	Array* posMap = ((Array*)metricData[OBSTACLE_POS]);

	numAVs = avListInDataMap->arraySize();
	cout << numAVs;
	for (int n = 0; n < numAVs; n++)
	{
		dataMap temp = {};
		//add to initial data map
		temp.emplace(ACCELERATION, new Double());
		temp.emplace(MIN_ACCELERATION, new Double());
		temp.emplace(AVG_ACCELERATION, new Double());
		temp.emplace(MAX_ACCELERATION, new Double());
		temp.emplace(AVG_DECELERATION, new Double());
		temp.emplace(AVG_SPEED, new Double());
		temp.emplace(SPEED, new Double());
		temp.emplace(POSITION, new Double());
		_initialMap.push_back(temp);
		_myMap.push_back(temp);
	}

	//get rid of dangling pointers
	delete avListInDataMap;
	delete obsIDsMap;
	delete accMap;
	delete speedMap;
	delete posMap;*/
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