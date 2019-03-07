#pragma once
#include "Obstacle.h"
#include "vector3.h"
#include "VComponent.h"
#include "..\h\TraCIAPI.h"

class Environment : public VComponent
{
public:
	Environment(std::string simPath, vector3 dimensions, int numObstacles, int port);
	~Environment();
	void addObstacle();
	virtual void InitializeSimulation() = 0;
	//void updateAV(Command update);
	void update();
private:
	vector3 _dimensions;
	int _numObstacles;
	Obstacle **_obstacles;
	int _port;
	std::string _simPath;
	
};

class Client : public TraCIAPI {
public:
	Client() {};
	~Client() {};
};