#pragma once
#include "Sensor.h"
#include "VComponent.h"

/*class to represent the action of the AV*/
class AVAction
{
public:
	Vector3 position;
	Vector3 orientation;
};

/*create template class to abstract data type
handles integer, string, float, Vector3, bool*/
class AVDataTypes
{
public:
	/*default constructor*/
	AVDataTypes() { _name = ""; _type = ""; }

	/*constructor that stores the name and data type of variable*/
	AVDataTypes(std::string name, std::string type)
	{
		_name = name;
		_type = type;
	}

	/*set value if integer*/
	void set(int _i)
	{
		i = _i;
	}

	/*set value if string*/
	void set(std::string _str)
	{
		str = _str;
	}
	
	/*set value if float*/
	void set(float _f)
	{
		f = _f;
	}
	
	/*set value if Vector3*/
	void set(Vector3 _v3)
	{
		v3 = _v3;
	}
	
	/*set value if boolean*/
	void set(bool _b)
	{
		b = _b;
	}

	/*variable to store the value if it is an integer*/
	int i;

	/*variable to store the value if it is a string*/
	std::string str;

	/*variable to store the value if it is a float*/
	float f;

	/*variable to store the value if it is a Vector3*/
	Vector3 v3;

	/*variable to store the value if it is a boolean*/
	bool b;

private:
	/*variable to store the name of variable*/
	std::string _name;

	/*variable to store the type of variable*/
	std::string _type;
};

class AV : VComponent
{
public:
	/*default constructor*/
	AV();

	/*constructor for the AV with a map containing variables and their data types*/
	AV(std::map<std::string, VType> AVmap);

	/*constructor for AV that accounts for AVs that can travel in the y-direction*/
	//AV(std::string _vehicleType, float _maxVelocity, float _maxAltitude, Vector3 _initialPosition, int numSensors);

	/*constructor for default AV*/
	//AV(std::string _vehicleType, float _maxVelocity, Vector3 _initialPosition, int numSensors);

	/*destructor*/
	~AV();

	/*override current map*/
	void OverrideMap(std::map<std::string, VType> AVmap);

	/*add a sensor to the sensor array*/
	void AddSensor(dataType _dataT, sensorType _sensorT, Vector3 _dimensions, Vector3 _position);

	/*update the sensor readings*/
	void update(timestamp t, dataMap* dataMap) {};
	void update();
	/*return the AV action using the AVAction class*/
	AVAction ReturnAction();
	//void PrintVehicleInfo();

	/*function to determine the AV's action*/
	virtual void DetermineAction() = 0;
private:
	/*AV map that stores all variable information*/
	std::map<std::string, VType> _AVmap;

	/*
	std::string vehicleType;
	float maxVelocity;
	float currentVelocity;
	//float initialVelocity(maybe previousVelocity);
	float maxAltitude;
	float currentAltitude;
	Vector3 currentPosition;
	Vector3 initialPosition;*/

	/*maximum sensors that can be associated with the AV*/
	int _maxSensors;

	/*number of sensors associated with the AV*/
	int numSensors;

	/*reference to the dynamic array of sensors that are part of the AV*/
	Sensor **sensors;

	/*variable that stores whether an AV can travel in the x, y, and z directions*/
	bool* travel = new bool[3];

	/*stores the action that will be sent to the event tree*/
	AVAction currentAction;
};

/*class that represents a ground vehicle
inherits from AV class*/
class GroundVehicle : public AV
{
public:
	/*constructor for ground vehicle that utilizes the constructor for AV*/
	GroundVehicle(std::map<std::string, VType> AVmap);
};