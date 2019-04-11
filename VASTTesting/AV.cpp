#include <iostream>
#include "AV.h"

/*AV::AV(std::string _vehicleType, float _maxVelocity, float _maxAltitude, Vector3 _initialPosition, int _numSensors)
{
	vehicleType = _vehicleType;
	maxVelocity = _maxVelocity;
	maxAltitude = _maxAltitude;
	initialPosition = _initialPosition;
	sensorN = 0;
	numSensors = _numSensors;
	sensors = new Sensor*[_numSensors];
}

AV::AV(std::string _vehicleType, float _maxVelocity, Vector3 _initialPosition, int _numSensors)
{
	vehicleType = _vehicleType;
	maxVelocity = _maxVelocity;
	initialPosition = _initialPosition;
	sensorN = 0;
	numSensors = _numSensors;
	sensors = new Sensor*[_numSensors];
}*/

/*default constructor*/
AV::AV()
{

}

/*constructor for the AV with a map containing variables and their data types*/
AV::AV(std::map<std::string, VType> AVmap)
{
	_AVmap = AVmap;
	numSensors = 0;
}

/*destructor*/
AV::~AV()
{
}

/*override current map*/
void AV::OverrideMap(std::map<std::string, VType> AVmap)
{
	_AVmap = AVmap;
}

/*add a sensor to the sensor array*/
void AV::AddSensor(dataType _dataT, sensorType _sensorT, Vector3 _dimensions, Vector3 _position)
{
	if (_sensorT== lidar)
		sensors[numSensors] = new LiDAR(_dataT, _sensorT, _dimensions, _position);

	/*if(_sensorT == radar)
		sensors[sensorN] = new RADAR(_dataT, _sensorT, _dimensions, _position);

	if(_sensorT == camera)
		sensors[sensorN] = new Camera(_dataT, _sensorT, _dimensions, _position);*/

	if(numSensors<_maxSensors)
		numSensors++;
}

/*update the sensor readings*/
void AV::update()
{
	for(int num = 0; num < numSensors; num++)
		sensors[num]->update();
}

/*return the AV action using the AVAction class*/
AVAction AV::ReturnAction()
{
	return currentAction;
}

/*void AV::PrintVehicleInfo()
{
	std::cout << "Vehicle Info: " << vehicleType << std::endl;
	std::cout << "Max Velocity: " << maxVelocity << "m/s" << std::endl;
	std::cout << "Max Altitude: " << maxAltitude << "m" << std::endl;
	std::cout << "Initial Position: " << initialPosition.x << "," << initialPosition.y << "," << initialPosition.z << std::endl;
	std::cout << "Number of Sensors" << numSensors << std::endl;
}*/

/*float AV::GetMaxV()
{
	return maxVelocity;
}*/

/*GroundVehicle::GroundVehicle(std::string _vehicleType, float _maxVelocity, float _maxAltitude, Vector3 _initialPosition, int numSensors) 
	: AV(_vehicleType, _maxVelocity, _maxAltitude, _initialPosition, numSensors)
{
}*/

/*constructor for ground vehicle that utilizes the constructor for AV*/
GroundVehicle::GroundVehicle(std::map<std::string, VType> AVmap)
	: AV(AVmap)
{
}