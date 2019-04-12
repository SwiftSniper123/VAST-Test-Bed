#include <iostream>
#include "Sensor.h"

/*default constructor*/
Sensor::Sensor()
{
}

/*destructor*/
Sensor::~Sensor()
{
}

/*function to process the input to the LiDAR sensor*/
void Sensor::update(timestamp t, dataMap dataMap)
{
	
}


VComponent::VCType Sensor::getVCType()
{
	return Sensor_Avatar;
}