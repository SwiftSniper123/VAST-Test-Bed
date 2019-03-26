#pragma once
#include <map>
#include "VType.h"

using std::map;
using std::string;

typedef double timestamp;
typedef map<string, VType*> dataMap;

/*class that all components of VAST inherit from*/
class VComponent
{
public: 
	/* The base class constructor for all VAST components.*/
	VComponent() {};

	/* The virtual update function*/
	virtual void update(timestamp t, dataMap* dataMap) = 0;
};