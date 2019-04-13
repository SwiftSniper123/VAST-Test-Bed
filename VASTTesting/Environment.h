#pragma once
#include "Obstacle.h"
#include "VComponent.h"

class Environment : public VComponent
{
public:
	Environment();
	void Envrionemnt(std::map<std::string, VType> _EnvMap);
	~Environment();
	void addObstacle();
	//void updateAV(Command update);
	void update(timestamp t, dataMap* dataMap);
private:
	std::map<std::string, VType> _EnvMap;
	/*
	int numObstacles;
	Obstacle **obstacles;
	int _env_obstacle_port;
	string _exe_location;
	Vector3 _env_bounds;*/
};

