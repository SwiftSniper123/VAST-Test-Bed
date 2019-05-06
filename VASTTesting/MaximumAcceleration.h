#pragma once
#include "ScenarioMetric.h"

/*
	calculates the Maximum Acceleration for the vehicle
*/
class MaximumAcceleration : public ScenarioMetric
{
public:
	//default constructor
	MaximumAcceleration(string name, dataMap metricData) : ScenarioMetric(name, metricData) { };

	void calculate();

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