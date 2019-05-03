#include "ScenarioMetric.h"

using namespace std;

void ScenarioMetric::calculate()
{
	AverageSpeed avgSp;
	AverageAcceleration avgAccel;
	MaximumAcceleration max;
	MinimumAcceleration min;
	AverageDeacceleration avgDecel;
	XYZCoordinates coor;

	avgSp.calculate();
	avgAccel.calculate();
	max.calculate();
	min.calculate();
	avgDecel.calculate();
	coor.calculate();

	return;
}


/*
	calculates the average speed of the chosen vehicle
*/
class AverageSpeed : public ScenarioMetric
{
public:
	//default constructor
	AverageSpeed() {};

	void calculate()
	{
		currentAvgSpd.value += stod(_myMap["Speed"]->s_value);
		totalSpeeds++;

		databaseMap["AverageSpeed"] = &(currentAvgSpd.value / totalSpeeds);

		return;
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
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

	void calculate()
	{
		if (currentMax.value < stod(_myMap["Acceleration"]->s_value))
		{
			currentMax.value = stod(_myMap["Accelertation"]->s_value);
		}
		databaseMap["MaxAcceleration"] = &currentMax;

		return;
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
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

	void calculate()
	{
		if (currentMin.value > stod(_myMap["Acceleration"]->s_value))
		{
			currentMin.value = stod(_myMap["Accelertation"]->s_value);
		}
		databaseMap["MinAcceleration"] = &currentMin;

		return;
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	//set to some unreachable value
	Double currentMin = 1000000.0;
};

/*
	calculates the Average Acceleration for the vehicle
*/
class AverageAcceleration : public ScenarioMetric
{
public:
	//default constructor
	AverageAcceleration() {};

	void calculate()
	{
		if (stod(_myMap["Acceleration"]->s_value > 0))
		{
			averageAcceleration.value += stod(_myMap["Acceleration"]->s_value);
			totalAccelerations++;
		}
		databaseMap["AverageAcceleration"] = &(averageAcceleration.value / totalAccelerations);

		return;
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	Double averageAcceleration = 0.0;

	float totalAccelerations = 0;
};

/*
	calculates the Average Acceleration for the vehicle
*/
class AverageDeacceleration : public ScenarioMetric
{
public:
	//default constructor
	AverageDeacceleration() {};

	void calculate()
	{
		if (stod(_myMap["Acceleration"]->s_value < 0))
		{
			averageDeacceleration.value += stod(_myMap["Acceleration"]->s_value);
			totalDeaccelerations++;
		}
		databaseMap["AverageDeacceleration"] = &(averageDeacceleration.value / totalDeaccelerations);

		return;
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	Double averageDeacceleration = 0.0;

	float totalDeaccelerations = 0;

};

/*
	holds the X Y and Z coordinates for the vehicle
*/
class XYZCoordinates : public ScenarioMetric
{
public:

	XYZCoordinates() {};

	void calculate()
	{
		coordiantes = _myMap["Position"]->s_value;

		databaseMap["Position"] = &coordiantes;

		return;
	}

	VCType getVCType()
	{
		return VCType::ScenarioMetric;
	}

private:
	Vector3 coordiantes;
};