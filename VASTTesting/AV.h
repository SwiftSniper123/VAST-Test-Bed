#pragma once
#include "Sensor.h"
#include "VComponent.h"

class AV : public VComponent
{
public:
	/*default constructor*/
	AV();

	/*destructor*/
	~AV();

	/*update the sensor readings*/
	void update(timestamp t, dataMap dataMap);

	/*return the type of VComponent as AV*/
	virtual VComponent::VCType getVCType();

	
private:
	/*AV map that stores all variable information*/
	dataMap* _AVmap;

};

