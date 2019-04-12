#pragma once
#include "Obstacle.h"
#include "VComponent.h"

class Environment : public VComponent
{
	friend class EventTree;
public:
	Environment();
	~Environment();
	
	void update(timestamp t, dataMap dataMap);

	/*return the type of VComponent as AV*/
	virtual VComponent::VCType getVCType();
private:
	Vector3 dimensions;
	int numObstacles;
	dataMap _dataMap;
};

