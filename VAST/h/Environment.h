#pragma once
#include "Obstacle.h"
#include "vector3.h"
#include "VComponent.h"
#include "..\h\TraCIAPI.h"
#include <map>

using std::map;
using std::string;

class Environment : public VComponent
{
public:
	Environment(vector3 _dimensions, int _numObstacles);
	~Environment();
	void addObstacle();
	//void updateAV(Command update);
	void update(timestamp t, dataMap* dataMap) {};
	void update();
private:
	vector3 dimensions;
	int numObstacles;
	Obstacle **obstacles;
	
};
