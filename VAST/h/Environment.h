#pragma once
#include "Obstacle.h"
#include "vector3.h"
#include "VComponent.h"
#include <TraCIAPI.h>

class Environment : public VComponent
{
public:
	Environment(vector3 _dimensions, int _numObstacles);
	~Environment();
	void addObstacle();
	//void updateAV(Command update);
	void update();
private:
	vector3 dimensions;
	int numObstacles;
	Obstacle **obstacles;
	
};

class Client : public TraCIAPI {
public:
	Client() {};
	~Client() {};
};