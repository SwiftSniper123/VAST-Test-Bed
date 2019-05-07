#pragma once
#include "ScenarioMetric.h"

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

	void calculate();

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
