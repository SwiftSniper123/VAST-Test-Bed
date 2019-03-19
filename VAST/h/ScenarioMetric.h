#pragma once
#include "..\h\VComponent.h"
#include "..\h\VType.h"

/*base class for all metrics in VAST*/
class ScenarioMetric : public VComponent
{
public:
	/* default constructor*/
	ScenarioMetric() {};

	/* returns the statistical metric value as defined by the extending metric 
	class.*/
	virtual VType calculate();
	
	//declares the virtual function for Vcomponent
	void update()
	{
		calculate();
	}
};

/*
	calculates the average speed of the chosen vehicle
*/
class AverageSpeed : public ScenarioMetric
{
public:
	//default constructor
	AverageSpeed() {};

	VType Calculate(float speed)
	{
		currentAvgSpd.value += speed;
		totalSpeeds++;

		return currentAvgSpd.value / totalSpeeds;
	}

private:
	Double currentAvgSpd = 0;
	float totalSpeeds = 0;
};

/*
	calculates the Maximum Acceleration for the vehicle
*/
class MaximumAcceleration : public ScenarioMetric
{
public:
	//default constructor
	MaximumAcceleration() {};

	VType calculate(Double acceleration)
	{
		if (acceleration.value > currentMax.value)
			currentMax = acceleration;

		return currentMax;
	}

	void printMaximum()
	{
		//prints out the maximum value with whatever output type we are using
	}

private:
	Double currentMax = 0;
};

/*
	calculates the Maximum Acceleration for the vehicle
*/
class MinimumAcceleration : public ScenarioMetric
{
public:
	//default constructor
	MinimumAcceleration() {};

	VType calculate(Double acceleration)
	{
		if (acceleration.value < currentMin.value)
			currentMin = acceleration;

		return currentMin;
	}

	void printMinimum()
	{
		//prints out the minimum value with whatever output type we are using
	}

private:
	//set to some unreachable value
	Double currentMin;
};

/*
	calculates the Average Acceleration for the vehicle
*/
class AverageAcceleration : public ScenarioMetric
{
public:
	//default constructor
	AverageAcceleration() {};

	VType calculate(float Speed1, float Speed2, int timeStep)
	{
		float temp;

		temp = ((Speed2 - Speed1) / timeStep);
		totalAverages++;

		min.calculate(temp);
		max.calculate(temp);

		return currentAverage.value / timeStep;
	}

private:
	MinimumAcceleration min;
	MaximumAcceleration max;
	Double currentAverage = 0;
	int totalAverages = 0;
};

/*
	holds the X Y and Z coordinates for the vehicle
*/
class XYZCoordinates : public ScenarioMetric
{
public:

	XYZCoordinates() {};

	VType calculate(double x, double y, double z)
	{
		coordiantes.value(x, y, z);

		return coordiantes;
	}

private:
	Vector3 coordiantes;
};
