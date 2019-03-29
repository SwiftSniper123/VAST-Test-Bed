#pragma once
#include <map>
#include "vector3.h"
#include "VComponent.h"

using std::map;
using std::string;

enum sensorType { lidar, radar, camera };
enum dataType { Infrared, Image };

class Sensor : public VComponent
{
public:
	/*default constructor*/
	Sensor();

	/*constructor that sets the values of dataT, sensorT, dimensions, and position*/
	Sensor(dataType _dataType, sensorType _sensorType, vector3 _dimensions, vector3 _position);

	/*destructor*/
	~Sensor();

	/*pure virtual function that is defined by classes that inherit from sensor
	used to process the data input to the sensor*/
	void update(timestamp t, dataMap* dataMap) {};
	void update() {};

	/*returns sensorT value*/
	sensorType GetSensorType();

	/*returns dataT value*/
	dataType GetDataType();
private:
	/*x,y, and z dimensions of the sensor*/
	vector3 dimensions;

	/*position of the sensor relative to the AV*/
	vector3 position;

	/*data types of sensor*/
	dataType dataT;

	/*sensor type*/
	sensorType sensorT;

	/*current sensor reading*/
	dataType currentReading;
};

/*3-D LiDAR data class that inherits from Sensor
All point that are hit within field-of-view and range
Array of vector3 coordinates*/
class LiDAR : public Sensor
{
public:
	/*constructor for LiDAR Sensor that utilizes sensor's constructor to store values*/
	LiDAR(dataType _dataType, sensorType _sensorType, vector3 _dimensions, vector3 _position);

	/*function to process the input to the LiDAR sensor*/
	void update();
};

/*class RADAR : public Sensor
{
public:
	RADAR(dataType _dataType, sensorType _sensorType, vector3 _dimensions, vector3 _position);
	void ProcessInput(dataType _dataType);
};

/*2-D Camera class that receives a 2-D image and inherits from Sensor*/
/*class Camera : public Sensor
{
public:
	Camera(dataType _dataType, sensorType _sensorType, vector3 _dimensions, vector3 _position);
	void ProcessInput(dataType _dataType);
};*/
