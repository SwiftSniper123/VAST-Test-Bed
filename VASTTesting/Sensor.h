#pragma once
#include "VComponent.h"


class Sensor : public VComponent
{
public:
	/*default constructor*/
	Sensor();

	/*destructor*/
	~Sensor();

	/*pure virtual function that is defined by classes that inherit from sensor
	used to process the data input to the sensor*/
	void update(timestamp t, dataMap dataMap);	

	/*return the type of VComponent as AV*/
	virtual VComponent::VCType getVCType();
private:
	

	/*data types of sensor*/
	dataMap _dataMap;

	
};