#include "Sensor.h"

/*default constructor*/
Sensor::Sensor()
{
}

Sensor::Sensor(string name, dataMap sensorData)
{
	_name = name;
	_dataMap = sensorData;
}

/*destructor*/
Sensor::~Sensor()
{
	_dataMap.clear();
}

/*function to process the input to the LiDAR sensor*/
void Sensor::update(timestamp t, dataMap dataMap)
{
	
}

void Sensor::stopReplication(bool another, string runID)
{

}

VComponent::VCType Sensor::getVCType()
{
	return Sensor_Avatar;
}

string Sensor::getName()
{
	return _name + "Sensor";
}

dataMap Sensor::getDataMap()
{
	return _dataMap;
}