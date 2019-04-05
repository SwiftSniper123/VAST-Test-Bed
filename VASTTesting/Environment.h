#pragma once
#include "Obstacle.h"
#include "VComponent.h"

class Environment : public VComponent
{
public:
	Environment();
	~Environment();
	void addObstacle();
	//void updateAV(Command update);
	void update(timestamp t, dataMap* dataMap);
private:
	Vector3 dimensions;
	int numObstacles;
	Obstacle **obstacles;
	
};

