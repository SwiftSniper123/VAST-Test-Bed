#pragma once
#include "ScenarioMetric.h"

/*
	calculates the Average Acceleration for the vehicle
*/
class AverageAcceleration : public ScenarioMetric
{
public:
	//default constructor
	AverageAcceleration() : ScenarioMetric() {};
	AverageAcceleration(string name, dataMap metricData) : ScenarioMetric(name, metricData)
	{
		for (int n = 0; n < numAVs; n++)
			sumAcceleration[n] = new Double();
	};

	void calculate();

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