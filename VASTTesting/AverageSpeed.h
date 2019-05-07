#pragma once
#include "ScenarioMetric.h"

/*
	calculates the average speed of the chosen vehicle
*/
class AverageSpeed : public ScenarioMetric
{
public:
	//default constructor
	AverageSpeed() : ScenarioMetric() {};
	AverageSpeed(string name, dataMap metricData) : ScenarioMetric(name, metricData) { };

	void calculate();

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
