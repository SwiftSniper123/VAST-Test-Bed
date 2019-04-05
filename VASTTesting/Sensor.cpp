#include <iostream>
#include "Sensor.h"

/*default constructor*/
Sensor::Sensor()
{
}

/*constructor that sets the values of dataT, sensorT, dimensions, and position*/
Sensor::Sensor(dataType _dataType, sensorType _sensorType, Vector3 _dimensions, Vector3 _position)
{
	dataT = _dataType;
	sensorT = _sensorType;
	dimensions = _dimensions;
	position = _position;
}

/*destructor*/
Sensor::~Sensor()
{
}

/*returns sensorT value*/
sensorType Sensor::GetSensorType()
{
	return sensorT;
}

/*returns dataT value*/
dataType Sensor::GetDataType()
{
	return dataT;
}

/*constructor for LiDAR Sensor that utilizes sensor's constructor to store values*/
LiDAR::LiDAR(dataType _dataType, sensorType _sensorType, Vector3 _dimensions, Vector3 _position) 
	: Sensor(_dataType, _sensorType, _dimensions, _position)
{
}

/*function to process the input to the LiDAR sensor*/
void LiDAR::update()
{
	//need to define
	std::cout << "updated sensor reading of " << GetSensorType() << std::endl;
}

/*RADAR::RADAR(dataType _dataType, sensorType _sensorType, Vector3 _dimensions, Vector3 _position) : Sensor(_dataType, _sensorType, _dimensions, _position)
{
}

void RADAR::ProcessInput(dataType pInput)
{
	//need to define
	std::cout << "updated sensor reading of " << GetSensorType() << std::endl;
}

Camera::Camera(dataType _dataType, sensorType _sensorType, Vector3 _dimensions, Vector3 _position) : Sensor(_dataType, _sensorType, _dimensions, _position)
{
}

void Camera::ProcessInput(dataType pInput)
{
	//need to define
	std::cout << "updated sensor reading of " << GetSensorType() << std::endl;
}*/