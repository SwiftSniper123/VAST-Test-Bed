#pragma once
#include <map>
#include "VType.h"	// std::string

using std::map;

typedef double timestamp;
typedef map<string, VType*> dataMap;

/*class that all components of VAST inherit from*/
class VComponent
{
public: 
	/* The base class constructor for all VAST components.*/
	VComponent() {};

	virtual string getName()
	{
		return "VComponent";
	};

	virtual dataMap getDataMap()
	{
		return dataMap();
	}

	enum VCType { AV_Avatar, Environment_Avatar, Sensor_Avatar, Obstacle_Avatar, Test_Avatar } type;

	virtual VCType getVCType() = 0;
	
	/* The virtual update function*/
	virtual void update(timestamp t, dataMap dataMap) = 0;
};