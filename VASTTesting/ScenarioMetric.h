#pragma once
#include "VComponent.h"
#include "VType.h"

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
		currentAvgSpd += speed;
		totalSpeeds++;

		return currentAvgSpd / totalSpeeds;
	}

private:
	Double currentAvgSpd = 0.0;
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
		if (acceleration.value() > currentMax.value())
			currentMax = acceleration;

		return currentMax;
	}

	void printMaximum()
	{
		//prints out the maximum value with whatever output type we are using
	}

private:
	Double currentMax = 0.0;
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
		if (acceleration.value() < currentMin.value())
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

	VType calculate(float Speed, int timeStep)
	{
		float temp;

		temp = ((previousSpeed - Speed) / timeStep);
		totalAverages++;

		// these do no inherit the calculate function from the parent 4/25/19 emf
		/*min.calculate(temp);
		max.calculate(temp);
		
		avgSpeed.calculate(Speed);*/
		previousSpeed = Speed;

		return currentAverage / timeStep;
	}

private:
	// these cannot be instantiated in present condition 4/25/19 emf
	/*MinimumAcceleration min;
	MaximumAcceleration max;
	AverageSpeed avgSpeed;*/
	Double currentAverage = 0.0;
	int totalAverages = 0;
	float previousSpeed = 0;
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
