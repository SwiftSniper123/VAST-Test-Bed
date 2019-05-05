#include "VC_HEADERS.h"

using namespace VASTConstants;

ScenarioMetric::ScenarioMetric()
{}

ScenarioMetric::ScenarioMetric(string name, dataMap metricData)
{
	_initialMap = metricData;	
	_myMap = metricData;

	// add to initial map
	_initialMap.emplace(ACCELERATION, new Double());
	_initialMap.emplace(MIN_ACCELERATION, new Double());
	_initialMap.emplace(AVG_ACCELERATION, new Double());
	_initialMap.emplace(MAX_ACCELERATION, new Double());
	_initialMap.emplace(AVG_DECELERATION, new Double());
	_initialMap.emplace(AVG_SPEED, new Double());
	_initialMap.emplace(SPEED, new Double());
	_initialMap.emplace(POSITION, new Double());

	// add to current map
	_myMap.emplace(ACCELERATION, new Double());
	_myMap.emplace(MIN_ACCELERATION, new Double());
	_myMap.emplace(AVG_ACCELERATION, new Double());
	_myMap.emplace(MAX_ACCELERATION, new Double());
	_myMap.emplace(AVG_DECELERATION, new Double());
	_myMap.emplace(AVG_SPEED, new Double());
	_myMap.emplace(SPEED, new Double());
	_myMap.emplace(POSITION, new Double());
}

void ScenarioMetric::stopReplication(bool another, string runID)
{
	// reset the current map with the initial map
	_myMap.clear();
	_myMap.emplace(ACCELERATION, new Double());
	_myMap.emplace(MIN_ACCELERATION, new Double());
	_myMap.emplace(AVG_ACCELERATION, new Double());
	_myMap.emplace(MAX_ACCELERATION, new Double());
	_myMap.emplace(AVG_DECELERATION, new Double());
	_myMap.emplace(AVG_SPEED, new Double());
	_myMap.emplace(SPEED, new Double());
	_myMap.emplace(POSITION, new Double());
}


/*
	calculates the average speed of the chosen vehicle
*/
class AverageSpeed : public ScenarioMetric
{
public:
	//default constructor
	AverageSpeed() { sumAvgSpeed = new Double(); };

	void calculate()
	{
		*sumAvgSpeed += ((Double*)_myMap[SPEED])->value();
		countSpeedUpdates++;
		_myMap[AVG_SPEED] = new Double(sumAvgSpeed->value() / countSpeedUpdates);
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	Double* sumAvgSpeed = nullptr;
	float countSpeedUpdates = 0;
};

/*
	calculates the Maximum Acceleration for the vehicle
*/
class MaximumAcceleration : public ScenarioMetric
{
public:
	//default constructor
	MaximumAcceleration() { currentMax = new Double(); };

	void calculate()
	{
		if (currentMax->value() < ((Double*)_myMap[ACCELERATION])->value())
		{
			currentMax->value(stod(_myMap[ACCELERATION]->s_value()));
			_myMap[MAX_ACCELERATION] = currentMax;
		}
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	Double* currentMax = nullptr;
};

/*
	calculates the Maximum Acceleration for the vehicle
*/
class MinimumAcceleration : public ScenarioMetric
{
public:
	//default constructor
	MinimumAcceleration() { currentMin = new Double(1000000.0); };

	void calculate()
	{
		if (currentMin->value() > ((Double*)_myMap[ACCELERATION])->value())
		{
			currentMin->value(((Double*)_myMap[ACCELERATION])->value());
			_myMap[MIN_ACCELERATION] = currentMin;
		}
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	//set to some unreachable value()
	Double* currentMin = nullptr;
};

/*
	calculates the Average Acceleration for the vehicle
*/
class AverageAcceleration : public ScenarioMetric
{
public:
	//default constructor
	AverageAcceleration() { sumAcceleration = new Double(); };

	void calculate()
	{
		if (((Double*)_myMap[ACCELERATION])->value() > 0)
		{
			*sumAcceleration += ((Double*)_myMap[ACCELERATION])->value();
			countAccelUpdates++;
			_initialMap[AVG_ACCELERATION] = new Double(sumAcceleration->value() / countAccelUpdates);
		}
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	Double* sumAcceleration = nullptr;

	float countAccelUpdates = 0;
};

/*
	calculates the Average Acceleration for the vehicle
*/
class AverageDeacceleration : public ScenarioMetric
{
public:
	//default constructor
	AverageDeacceleration() { sumDeacceleration = new Double(); };

	void calculate()
	{
		if (((Double*)_myMap[ACCELERATION])->value() < 0)
		{
			*sumDeacceleration += ((Double*)_myMap[ACCELERATION])->value();
			countDecelUpdates++;
			_initialMap[AVG_DECELERATION] = new Double(sumDeacceleration->value() / countDecelUpdates);
		}		
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	Double* sumDeacceleration = nullptr;

	float countDecelUpdates = 0;
};

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
	Array* avListInDataMap = updateMap[AV_LIST];
	for (int i = 0; i < )
	_myMap[ACCELERATION] = updateMap[ACCELERATION];
	_myMap[SPEED] = updateMap[SPEED];
	_myMap[POSITION] = updateMap[POSITION];

	calculate();

	getEventTree()->addEvent(this, t, _myMap);
}

void ScenarioMetric::calculate()
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
}
