#pragma once
#include "ScenarioMetric.h"

/*
	calculates the Minimum Acceleration for the vehicle
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

	void calculate();

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