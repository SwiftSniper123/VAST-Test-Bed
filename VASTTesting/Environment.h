#pragma once
#include "..\VAST\h\Obstacle.h"
#include "..\VAST\h\VComponent.h"

class Environment : public VComponent
{
public:
	Environment();
	~Environment();
	void addObstacle();
	//void updateAV(Command update);
	void update(timestamp t, dataMap* dataMap);
private:
	vector3 dimensions;
	int numObstacles;
	Obstacle **obstacles;
	
};

